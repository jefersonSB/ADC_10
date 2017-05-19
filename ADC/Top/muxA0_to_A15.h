/**
 * 
 * Esse MUX funciona como uma chave analógica 
 * Faz uso dos registradores abaixo para controlar suas rotinas
 * 
 * ADC12CTL1, [CSTARTADDx(15-12)]01A2h Esse registrador aponta para os um registrador ADC12MCTLx 
 * que deve ser convertido. Ele retorna um valor Hexa 0x0h a 0xfh (0 a 15) conrrespondente a ADC12MCTL0 a 15   
 *
 * ADC12MCTLx, [EOS(7), INCHx(3-0)](080h a 08Fh em passos de 1) Seu valor só pode se alterar quando ENC = 0
 * EOS = 0 continuar a converter a sequência de canais; EOS = 1 fim da sequência; 
 * INCHx = de 0000 a 0111: canais de entradas de fontes externas. 1010 Sensor de temperatura
 * de 1011 a 1111 = (AVCC – AVSS) / 2.
 * 
 * ADC12CTL0, [ADC12ON(4), ENC(1)] address (01A0h)
 * ADC12ON = 0 conversão desabilitada, = 1 habilitada. Começar a ler entrada do MUX			
 * ENC = 1 conversão habilitada, = 0 desabilitada. Começar a transmitir para saída do MUX
 */

#ifndef MUXA0_TO_A15_H
#define MUXA0_TO_A15_H

#include "defines.h"
#include "tlm_utils/simple_initiator_socket.h"

/// Initiator module generating generic payload transactions
SC_MODULE(muxA0_to_A15){	

///configurações TLM para comunicação com a plataforma	
	tlm_utils::simple_initiator_socket<muxA0_to_A15> socket;   			/// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm::tlm_command CMD;												///	Comando TLM Read ou Write
	tlm::tlm_generic_payload trans;										/// Declaração do objeto de transação;
	sc_time delay;														/// Delay de comunicação desejada
    type_reg address; 												/// Endereço de registrador
/// Fin

	type_reg ADC12MCTLx;
	sc_uint < 4  > channel; 
	sc_bv   < 4  > CSTARTADDx;
	sc_bv   < 4  > INCHx;
	sc_uint < 1  > EOS;
	sc_bv   < 1  > ADC12ON;
	sc_bv   < 1  > ENC;
	 
///configurações de portas de comunicação entre blocos 

	sc_out <double> outpu_mux;											/// Saída de sinal para Sample
	sc_vector < sc_in < double > > input_srcs;							/// Entrada de sinal analógico de fonte externa
	sc_vector < sc_in < bool > > consumer;								/// Entrada de sinal de de fonte (Metodo sensitive)
	
/// Fin

	type_reg data;		
																		/// ADC12MCTLx, [EOS(7), SREFx(6-4), INCHx(3-0)] Só alterar quando ENC = 0
/// Bloco de metodos 
	void tlm_communication();
	void read_channel();
/// Fin
	
	SC_CTOR(muxA0_to_A15)
	: socket("muxA0_to_A15")
	, CMD(tlm::TLM_READ_COMMAND)
	, address (0x0)
	, delay (10.0, SC_NS), channel(11)
	, input_srcs ("input_srcs", 16)
	, consumer("consumer", 16)
	{
		SC_THREAD(tlm_communication);
	///	dont_initialize();
		
		SC_METHOD(read_channel);
			for (int i=0;i<16;i++){	sensitive << consumer[i]; } 	 	/// Torna o metodo senssitivo a qualquer que seja as entradas
			dont_initialize();										 	/// onde uma fonte análógica pode ser ligada
			cout << '\n'<< '\t'<< "Canais MUX_A0_to_A15" << '\t'<< "Lista Sensitiva do MUX"<< endl;
			for (int i=0;i<16;i++){
				cout << '\t'<< input_srcs[i].basename() << '\t'<< '\t'<< consumer[i].basename()<< endl;			
			}
	}
};
///*
void muxA0_to_A15::read_channel(){
	
	if (ENC == 1){
		outpu_mux.write(input_srcs[channel].read());
	}
}
///*/

void muxA0_to_A15::tlm_communication(){

	cout << '\n' << '\t' <<  "COMUNICAÇÂO INICADA:" << '\n' <<'\n';
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

			trans.set_command(CMD);											///
			trans.set_address(address);										///	
			trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );		/// Clear the response status

			socket->b_transport( trans, delay );  							/// Blocking transport call
				
				if ( trans.is_response_error() ){								///Initiator obliged to check response status and delay
					SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
				}
				
			tmp = data;	
			ADC12ON [0] = tmp[4]; 											/// Obtenção do bit de habilitação de converção 
			ENC [0]		= tmp[1];											/// Obtenção do bit 
			
			wait(delay); 													/// Realize the delay annotated onto the transport call
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
				socket->b_transport( trans, delay );  // Blocking transport call
					
					if ( trans.is_response_error() ){								///Initiator obliged to check response status and delay
						SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
					}
				
				tmp = data;			
				CSTARTADDx.range(3,0) = tmp.range(15,12);
				ADC12MCTLx = CSTARTADDx ;
				ADC12MCTLx +=128;
		
				wait(delay); 			/// Realize the delay annotated onto the transport call
				/**
				cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
					 << " } , data = " << tmp << " at time " << sc_time_stamp()
					 << " delay = " << delay << '\n';
				cout << "ADC12CTL1 -- CSTARTADDx = " << ADC12MCTLx-128  << '\n';
				*/

				address = ADC12MCTLx;
				trans.set_address(address);
				trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );		/// Clear the response status
				socket->b_transport( trans, delay );  							/// Blocking transport call
				
					if ( trans.is_response_error() ){								///Initiator obliged to check response status and delay
						SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
					}

				tmp = data;			
				EOS[0] = tmp[7];
				INCHx = tmp.range(3,0);

				wait(delay); 			/// Realize the delay annotated onto the transport call
				/**
				cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
					 << " } , data = " << tmp << " at time " << sc_time_stamp()
					 << " delay = " << delay << '\n';
				cout << "ADC12MCTLx  --  EOS = " << EOS << " INCHx =  "<< channel <<'\n';
				*/
				if (channel != INCHx){
					channel = INCHx;
					cout <<"ctr_chanel.notify("<< channel  <<")" << endl <<endl;	
				}
				
			}
		}
	}
}
#endif
