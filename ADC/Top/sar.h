
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
# include <math.h>


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
	///	double Vr_n, Vr_p; 												/// Tensão de referencia
	sc_in  < double > sample_port_in; 									/// Entrada de amostra
///	sc_out < sc_bv < 12 > > sample_port_out; 							/// Saida para verificação em octave
	sc_out < double > sample_port_out; 							/// Saida para verificação em octave

/// Fin
	
	sc_event start_conv_ev; 											/// Notificação de entrada de amostra
	sc_event adc12clk_ev;

	
/// BLoco de variaveis
	type_reg ADC12MEMx;													/// Endereço de registrador de memória a ser escrito
	sc_bv   < 4  > CSTARTADDx;
	sc_bv   < 1  > ADC12ON;
	
	type_reg 	Memory_result;											/// Resultado a ser escrito no registrador ADC12MEMx comrrespondente
	double 		sample_value;											/// Valor de tensão amostrada
	sc_bv <12>  dig_result;												/// resultado da conversão

	double n_bit;
	double resol;
	double result_memory;
	sc_uint <32> tmp_ref;
	sc_uint <32> tmp_result;
/// Fin

/// Bloco de metodos 
	void tlm_communication();
	void binary_search();												/// Função de conversão 
	void read_sample ();
	void sync_adc12ck ();
	

/// Fin

	SC_CTOR(sar)
	/// parametros TLM	
		: socket("SAR") 
		, delay (1.0, SC_NS)
		, address (0x0)
		, data (0x0)
	/// Fin
	/// portas	
		, sample_port_out ("sample_port_out")
		, sample_port_in ("sample_port_in")
		, convert ("convert")
		, adc12clk ("adc12clk")
	/// Fin
	/// parametros para uso do núcleo SAR	
		, tmp_ref (0x5)
		, n_bit (4095)
		, tmp_result(0x0)	
		, Memory_result(0x0)
		, sample_value (0.0)
		, dig_result("0000000000000000")
		, result_memory (0.0)

	/// Parametros de Registradores 	
		, ADC12MEMx (0X0)
		, ADC12ON ("0")
		, CSTARTADDx ("0000")
	/// Fin
	{
		resol = (tmp_ref/n_bit);

		cout << '\n' << '\t' << "tmp_ref " <<tmp_ref<<  '\n';
		cout << '\n' << '\t' << "n_bit " <<n_bit<<  '\n';
		cout << '\n' << '\t' << "resol " <<resol<<  '\n';
		cout << '\n' << '\t' << "tmp_ref " <<tmp_ref<<  '\n';
		
		SC_THREAD(tlm_communication);
		SC_THREAD(binary_search);

		SC_METHOD(sync_adc12ck)
			sensitive << adc12clk.pos();
			dont_initialize();	
		
		SC_METHOD(read_sample);
		sensitive << (convert.neg());
			dont_initialize();	
	}
};


void sar::sync_adc12ck (){
	adc12clk_ev.notify();
}

void sar::binary_search(){
		
		
	while(true){
		wait (start_conv_ev);

	///	cout << '\n' << '\t' << "COMVERT_START " << " sample_value:" << sample_value <<'\n';	
	
		for(int i = 11; i>= 0; i-- ){
			wait (adc12clk_ev);

			dig_result[i] = 1;
			tmp_ref = dig_result;
	///		sample_value = 2.5;
	///		cout << '\n' << '\t' << "tmp_ref " << dig_result << '\n';	

			if (sample_value >= tmp_ref*resol){
				dig_result[i] = 1;
	///			cout <<'\n' << "sample_value >= tmp_ref  " << sample_value << "  "<< tmp_ref*resol <<'\n';
			}else if (sample_value < tmp_ref*resol){
	///			cout <<'\n' << "sample_value < tmp_ref  " << sample_value << "  "<< tmp_ref*resol <<'\n';
				dig_result[i] = 0;
			}
		}			
		tmp_result = dig_result;
		result_memory = tmp_result*resol;
	///	cout << '\n' << '\t' << "result_memory " << result_memory << '\n';
	///	cout << '\n' << '\t' << "dig_result " << dig_result << '\n';
		sample_port_out.write(result_memory);
		dig_result = 0;													/// limpando o valor da tensão de referencia 
	} 
}

void sar::read_sample(){

///	cout << '\n' << '\t' << "Read_sample: ADC12ON = " << ADC12ON << '\n';
	
	if (ADC12ON == 1){
		sample_value = sample_port_in.read();
		start_conv_ev.notify();
	} else{
		cout << '\n' << '\t' << "ADC12ON =" << ADC12ON << "Conversão desativada" << '\n';
	}
}

void sar::tlm_communication(){

	cout << '\n' << '\t' <<  "COMUNICAÇÂO INICADA: SAR" << '\n' <<'\n';
	sc_bv < 16 > tmp;
	CMD = tlm::TLM_READ_COMMAND;
	
	trans.set_data_length(16);										///
	trans.set_streaming_width( 16 ); 								/// = data_length to indicate no streaming
	trans.set_byte_enable_ptr(0); 									/// 0 indicates unused	
	trans.set_dmi_allowed( false ); 								/// Mandatory initial value
	trans.set_data_ptr((unsigned char*)(&data) );				

	while (true){
			
		address = ADC12CTL0;	

		if (CMD == tlm::TLM_READ_COMMAND) {

			trans.set_command(CMD);										///
			trans.set_address(address);									///	
			trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );	/// Clear the response status
			socket->b_transport( trans, delay );  						/// Blocking transport call
				
				if ( trans.is_response_error() ){						///Initiator obliged to check response status and delay
					SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
				}
				
			tmp = data;	
			ADC12ON[0]  = tmp[4]; 										/// Obtenção do bit de habilitação de converção 
				wait(delay); 											/// Realize the delay annotated onto the transport call
			/**
			cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
			 << " } , data = " << tmp << " at time " << sc_time_stamp()
			 << " delay = " << delay << '\n';
			cout << " ADC12CTL0 -- ADC12ON = " << ADC12ON << '\n';
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
					cout <<"Memory_result_in.notify("<< Memory_result  <<")" << '\n';	
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
