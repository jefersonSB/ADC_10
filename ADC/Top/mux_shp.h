
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
		
	sc_out  <bool> sampcon_port;										/// porta de saida de sinal sampcom para blocos internos
	sc_in  <bool> klsps_200;											/// porta de saida de sinal sampcom para blocos internos
	
/// BLoco de variaveis
	sc_bv <1> SHP;														/// Escolha de modo de operação 
	sc_bv <1> SAMPCON;													/// variavel de disparo do switch do sample and hold

/// BLoco de eventos
	sc_event samp_event;												/// Evento para disparar o switch do sample and hold
/// Fin

/// BLoco Metodos 
	void tlm_communication();
	void write_sampcon();
/// Fin

	SC_CTOR(mux_shp)
	: SHP(0), SAMPCON(false)
	, socket("mux_shp"), sampcon_port("sampcon_port")
	, CMD(tlm::TLM_READ_COMMAND)
	, address (0x0)
	, delay (1.0, SC_NS)
	, klsps_200 ("klsps_200")
	{
		SC_THREAD(tlm_communication);
		
		SC_THREAD(write_sampcon);
			sensitive << klsps_200.pos()<< klsps_200.neg();
			dont_initialize();
	}
};

void mux_shp::write_sampcon(){

	while(1){
		if (SHP == 1){
			sampcon_port.write(1);
			wait();
			sampcon_port.write(0);
			wait();
		}
		wait(1.0, SC_NS);

	}
///	while(true){
///		wait(samp_event);
///		if (SAMPCON == 1){
///			sampcon_port.write(1);
///		}else{
///			sampcon_port.write(0);
///		}
	///}
}

void mux_shp::tlm_communication(){

	cout << '\n' << '\t' <<  "COMUNICAÇÂO INICADA: MUX_SHP" << '\n' <<'\n';
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
			///samp_event.notify();
			wait(delay); 												/// Realize the delay annotated onto the transport call
		}
	}
}
