
/**
 * -----------------------------------
 * Núcleo do conversor tópologia SAR |
 * -----------------------------------
 * 
 * Este Módulo recebe o sinal vindo do Sample and Hold em FIFO
 * 
 * Esse Módulo depende dos registradores: ADC12CTL0, ADC12CTL1
 * 
 * ADC12CTL0: ADC12ON(4) = 1 habilita SAR, ADC12ON = 0 Desabilita SAR 
 * 
 * SAMPCO(Convert) - Esse bit vem do bloco MUX_SHP. Esse deve ser sensitivo a Convert.neg();
 * AVSS
 * BUSY
 * 
 * ADC12CTL1, [CSTARTADDx(15-12)]01A2h Esse registrador aponta para os um registrador ADC12MCTLx 
 * que está sendo convertido. Ele retorna um valor Hexa 0x0h a 0xfh (0 a 15) conrrespondente
 * a ADC12MCTL0 a 15, O que signigfica que o resultado da conversão  deve ser quadado no 
 * ADC12MEMx retornado por CSTARTADDx.
 * 
 * Entradas VR−, VR+, Convert, ADC12CLK
 * 
 * Resultado de saída NADC = 4095 * [ (Vin - (VR-) ]/[ (VR+) - (VR-) ]
 * 
 */
 
# ifndef SAR_H
# define SAR_H

# include "defines.h"
# include <systemc.h>

SC_MODULE (sar){
	
///configurações TLM para comunicação com a plataforma	
	tlm_utils::simple_initiator_socket<sar> socket;   					/// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm::tlm_command CMD;												///	Comando TLM Read ou Write
	tlm::tlm_generic_payload trans;										/// Declaração do objeto de transação;
	sc_time delay;														/// Delay de comunicação desejada
    type_reg address; 													/// Endereço de registrador
	type_reg data;	
/// Fin

/// Bloco de portas 
	sc_in < bool  > convert; 											/// Entrada de gatilho de inicio de conversão (senssitive.neg)
	sc_in < bool  > adc12clk;											/// Entrada de clock adc12
	///	sc_in < doube > Vr_n, Vr_p; 									/// Tensão de referencia
	sc_in  < doble > sample_port; 										/// Entrada de amostra
	sc_out < doble > sample_port_out; 									/// Saida para verificação em octave
/// Fin
	
	sc_event start_conv; 												/// Notificação de entrada de amostra
	
/// BLoco de variaveis
	type_reg ADC12MEMx;													/// Endereço de registrador de memória a ser escrito
	type_reg Memory_result;												/// Resultado a ser escrito no registrador ADC12MEMx comrrespondente
	sc_bv < 12 > sample_value;											/// Valor de tensão amostrada
	sc_bv <12> dig_result;												/// resultado da conversão

	double qbi = 4095;
	double resol;
	double result_memory = 0;

	sc_uint <32> tmp_ref;
	sc_uint <32> tmp_result;



/// Fin

/// Bloco de metodos 
	void tlm_communication();
	void binary_search();												/// Função de conversão 
	void read_sample ();

/// Fin

	SC_CTOR(sar)
		: socket("SAR") 
		, Convert (0)
		, ADC12MEMx (0)
		, Memory_result(0)
		, ADC12ON (0)
		, start_conv ("start_conv")
		, sample_value (0)
		, dig_result(0)
		, resol = ((ref/qbi))
		, tmp_ref (0)
		, tmp_result(0)
		, sample_port_out ("sample_port_out")
	{

		SC_METHOD(read_sample);
		sensitive << convert.neg();
			dont_initialize();

		SC_METHOD(Binary_search);
		sensitive << start_conv;
			dont_initialize();
	}
};

void sar::binary_search(){
	cout << '\n' << '\t' << "COMVERT_START " << result_dig << '\n';
	double ref; ref = 5;
		
	for(int i = 11; i>= 0; i-- ){

		wait(adc12clk.pos());
		
		result_dig[i] = 1;
		tmp_ref = result_dig;
///		cout << '\n' << '\t' << "tmp_ref " << result_dig << '\n';	

		if (sample_value >= tmp_ref*resol){
			result_dig[i] = 1;
///			cout <<'\n' << "sample >= tmp_ref  " << sample_value << "  "<< tmp_ref*resol <<'\n';
		}else if (sample_value < tmp_ref*resol){
///			cout <<'\n' << "sample < tmp_ref  " << sample_value << "  "<< tmp_ref*resol <<'\n';
			result_dig[i] = 0;
		}
	}			

	tmp_result = result_dig;
	result_memory = tmp_result*resol;
	cout << '\n' << '\t' << "result_memory " << result_memory << '\n';
} 

void sar::read_sample(){
	
	if (ADC12ON == 1){
		sample_value = sample_port.read();
		start_conv.notify()
	} else(){
		cout << '\n' << '\t' << "ADC12ON =" << ADC12ON << "Conversão desativada" << '\n';
	}
}

void sar::tlm_communication(){

	cout << '\n' << '\t' <<  "COMUNICAÇÂO INICADA:" << '\n' <<'\n';
	sc_bv < 16 > tmp;
	CMD = tlm::TLM_READ_COMMAND;
	
	trans.set_data_length(16);										///
	trans.set_streaming_width( 16 ); 								/// = data_length to indicate no streaming
	trans.set_byte_enable_ptr(0); 									/// 0 indicates unused	
	trans.set_dmi_allowed( false ); 								/// Mandatory initial value
	trans.set_data_ptr((unsigned char*)(&data) );				

	while (true){
			
		address = ADC12CTL0 ;
	
		if (CMD == tlm::TLM_READ_COMMAND) {

			trans.set_command(CMD);											///
			trans.set_address(address);										///	
			trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );		/// Clear the response status

			socket->b_transport( trans, delay );  							/// Blocking transport call
				
				if ( trans.is_response_error() ){								///Initiator obliged to check response status and delay
					SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
				}
				
			tmp = data;	
			ADC12ON[0]  = tmp[4]; 										/// Obtenção do bit de habilitação de converção 
				wait(delay); 											/// Realize the delay annotated onto the transport call
			/**	
			cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
			 << " } , data = " << tmp << " at time " << sc_time_stamp()
			 << " delay = " << delay << '\n';
			cout << " ADC12CTL0 -- ADC12ON = " << ADC12ON << " ENC = " << ENC << '\n';
			*/ 

			if (ADC12ON == 1){ 		///Conversão ativada
				
				address = ADC12CTL1;
				trans.set_address(address);
				trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );		/// Clear the response status
				socket->b_transport( trans, delay );  							/// Blocking transport call
					
					if ( trans.is_response_error() ){								///Initiator obliged to check response status and delay
						SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
					}
				
				tmp = data;			
				CSTARTADDx.range(3,0) = tmp.range(15,12);
				ADC12MEMx = CSTARTADDx ;
				
				if (Memory_result != ADC12MEMx){
					Memory_result = ADC12MEMx;
					cout <<"Memory_result.notify("<< Memory_result  <<")" << '\n\n';	
				}
				address = Memory_result;
				wait(delay); 											/// Realize the delay annotated onto the transport call
				/**
				cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
					 << " } , data = " << tmp << " at time " << sc_time_stamp()
					 << " delay = " << delay << '\n';
				cout << "ADC12CTL1 -- CSTARTADDx = " << ADC12MCTLx-128  << '\n';
				*/
			}
				
		}
	}
}
# endif
