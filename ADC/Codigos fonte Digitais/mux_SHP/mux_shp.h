
/**
 * \file mux_shp.h
 * \brief mux file.
 * 
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 * 
 * \version 1.0-dev
 * 
 * \date 15/05/2017
 * 
 * \defgroup main Main
 * \{
 */

///-----------------------------------------
/// Módulo Mux_SHP TLM 2.0, ADC12-MSP430F249
///-----------------------------------------
/**
 * Esse módulo é controlador pelo trecho SHP do registrador ADC12CTL1 (bit 9)   
 * Quando SHP = 1 Sample and Hold ativo e amostra o sinal com o ADC12CLK,
 * quando SAMPCON = 0 Sample desativado.
 * 
 */
# include <systemc.h>
# include "tlm_utils/simple_initiator_socket.h"
# include "defines.h"													/// para ter acesso as definições de registradores

SC_MODULE(mux_shp){

///configurações TLM para comunicação com a plataforma	
	tlm_utils::simple_initiator_socket<mux_shp> socket;		   			/// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm::tlm_command CMD;												///	Comando TLM Read ou Write
	tlm::tlm_generic_payload trans;										/// Declaração do objeto de transação;
	sc_time delay;														/// Delay de comunicação desejada
    type_reg address; 													/// Endereço de registrador
	type_reg data;
/// Fin
		
	sc_out  <bool> ADC12CLK;
	
/// BLoco de variaveis
	sc_bv <1> SHP;
	sc_bv <1> SAMPCON;
	
/// BLoco de eventos
	sc_event samp_event;
/// Fin
	
	void tlm_communication();

	void Adc12clk();

	SC_CTOR(mux_shp)
	: SHP(0), SAMPCON(false)
	, socket("mux_shp"), ADC12CLK("ADC12CLK")
	, CMD(tlm::TLM_READ_COMMAND)
	, address (0x0)
	, delay (10.0, SC_NS)
	{
		SC_THREAD(tlm_communication);
		
		SC_THREAD(Adc12clk);
///			dont_initialize();
	}

};

void mux_shp::Adc12clk(){

	while(true){
	///	cout << '\t' << "fora" << '\n';
		while(SAMPCON==1){
	///		cout << '\t' << "dentro" << '\n';
			ADC12CLK.write(1);
			wait(6, SC_US); 	
			ADC12CLK.write(0); 	
			wait(6, SC_US); 	
		}
	wait(samp_event);
	}
}

void mux_shp::tlm_communication(){

	cout << '\n' << '\t' <<  "COMUNICAÇÂO INICADA:" << '\n' <<'\n';
	sc_bv < 16 > tmp;
	CMD = tlm::TLM_READ_COMMAND;
	
	trans.set_data_length(16);										///
	trans.set_streaming_width( 16 ); 								/// = data_length to indicate no streaming
	trans.set_byte_enable_ptr(0); 									/// 0 indicates unused	
	trans.set_dmi_allowed( false ); 								/// Mandatory initial value
	trans.set_data_ptr((unsigned char*)(&data) );				
	address = ADC12CTL1;

	while (true){

		if (CMD == tlm::TLM_READ_COMMAND) {

			trans.set_command(CMD);										///
			trans.set_address(address);									///	
			trans.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );	/// Clear the response status

			socket->b_transport( trans, delay );  						/// Blocking transport call

			if ( trans.is_response_error() ){							///Initiator obliged to check response status and delay
				SC_REPORT_ERROR("TLM-2.0", trans.get_response_string().c_str());
			}

			tmp = data;	
			SHP [0] = tmp[9]; 											/// Obtenção do bit de habilitação SHP de amostragen 

			/// cout << "trans = { " << (CMD ? 'W' : 'R') << ", "<< address
			/// << " } , data = " << tmp << " at time " << sc_time_stamp()
			///	<< " delay = " << delay << '\n';
			///cout << " ADC12CTL1 -- SHP = " << SHP << '\n';

			SAMPCON = SHP;												/// SAMPCON 1 ou 0
			samp_event.notify();
			wait(delay); 												/// Realize the delay annotated onto the transport call
			}
		}
}
