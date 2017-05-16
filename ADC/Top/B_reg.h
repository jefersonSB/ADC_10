
#ifndef BAN_REG_H
#define BAN_REG_H

#include "defines.h"


SC_MODULE(Ban_Reg){
	
///configurações TLM para comunicação com a plataforma	
	tlm_utils::simple_target_socket<Ban_Reg> target;   // Soquete TLM-2, padrão para 32-bits de largura, protocolo base
/// Fin

/// Portas de de comunicação etre blocos  
	sc_in < bool > shift_channel;
/// Fin

	type_reg b_reg[SIZE];

	void shift_reg(); 	/// Método para mudança de valores de registradores

	SC_CTOR(Ban_Reg):target("target")
	{
		/// Registrar callback para chamada de método de interface b_transport de entrada
		target.register_b_transport(this, &Ban_Reg::b_transport);
{
		b_reg[ ADC12CTL0   ] = 0b0000000000010010; 
		b_reg[ ADC12CTL1   ] = 0b1111000001000000;
		b_reg[ ADC12IFG    ] = 0b0000000000000000;
		b_reg[ ADC12IE 	   ] = 0b0000000000000000;
		b_reg[ ADC12IV 	   ] = 0b0000000000000000;
		
		b_reg[ ADC12MEM0   ] = 0b0000000000000000;
		b_reg[ ADC12MEM1   ] = 0b0000000000000000;
		b_reg[ ADC12MEM2   ] = 0b0000000000000000;
		b_reg[ ADC12MEM3   ] = 0b0000000000000000;
		b_reg[ ADC12MEM4   ] = 0b0000000000000000;
		b_reg[ ADC12MEM5   ] = 0b0000000000000000;
		b_reg[ ADC12MEM6   ] = 0b0000000000000000;
		b_reg[ ADC12MEM7   ] = 0b0000000000000000;
		b_reg[ ADC12MEM8   ] = 0b0000000000000000;
		b_reg[ ADC12MEM9   ] = 0b0000000000000000;
		b_reg[ ADC12MEM10  ] = 0b0000000000000000;
		b_reg[ ADC12MEM11  ] = 0b0000000000000000;
		b_reg[ ADC12MEM12  ] = 0b0000000000000000;
		b_reg[ ADC12MEM13  ] = 0b0000000000000000;
		b_reg[ ADC12MEM14  ] = 0b0000000000000000;
		b_reg[ ADC12MEM15  ] = 0b0000000000000000;
	
		b_reg[ ADC12MCTL0  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL1  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL2  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL3  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL4  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL5  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL6  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL7  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL8  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL9  ] = 0b0000000000000000;
		b_reg[ ADC12MCTL10 ] = 0b0000000000000000;
		b_reg[ ADC12MCTL11 ] = 0b0000000000000000;
		b_reg[ ADC12MCTL12 ] = 0b0000000000000000;
		b_reg[ ADC12MCTL13 ] = 0b0000000000000000;
		b_reg[ ADC12MCTL14 ] = 0b0000000000000000;
		b_reg[ ADC12MCTL15 ] = 0b0000000000000000;
}

	///	cout << '\n'<<'\t'<<'\t' << "INICIALIZAÇÃO DE BANCO DE REGISTRADORES:" << '\n'<< '\n';
	///	for (int i=0x080; i < SIZE; i++ ){
	///		cout << '\t' << "b_reg[" << i << "]" << "DATA : " << b_reg[ i ] << '\n';
	///	}
			
		SC_THREAD(shift_reg);
			sensitive << shift_channel;
			dont_initialize();
	}

	virtual void b_transport (tlm::tlm_generic_payload &trans, sc_time &delay){
		
		tlm::tlm_command 	CMD = trans.get_command();				/// Obter comando TLM READ ou WRITE
		sc_dt::uint64		adr = trans.get_address();				/// Obter endereço de memória para leitura ou escrita  	
		unsigned char*   	ptr = trans.get_data_ptr();				///
		unsigned int     	len = trans.get_data_length();			/// Obter comprimento da palavra binária dados
		unsigned int     	wid = trans.get_streaming_width();		/// 
		unsigned char*   	byt = trans.get_byte_enable_ptr();		/// 

		if (adr >= SIZE) { /// Check for storage address overflow
			trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
			return;
		}
		if (byt) { /// Target unable to support byte enable attribute
			trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
			return;
		}
		if (wid < len) { /// Target unable to support streaming width attribute
			trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
			return;
		}

		if (CMD == tlm::TLM_WRITE_COMMAND){  /// Execute command
			memcpy(&b_reg[adr], ptr, len);
		}else if (CMD == tlm::TLM_READ_COMMAND){
			memcpy(ptr, &b_reg[adr], len);
		}

	trans.set_response_status( tlm::TLM_OK_RESPONSE ); /// Successful completion

	}
};

///*
void Ban_Reg::shift_reg(){

	b_reg[0x08F] = 0b0000000010000101;
	wait();
	b_reg[0x08F] = 0b0000000010000101;
	wait();
	b_reg[0x08F] = 0b0000000010000111;
	wait();
	b_reg[0x08F] = 0b0000000010000111;
	wait();
	b_reg[0x08F] = 0b0000000010000111;
	wait();
	b_reg[0x08F] = 0b0000000010000111;
	wait();
	b_reg[0x08F] = 0b0000000010000101;
	wait();
	b_reg[0x08F] = 0b0000000010000101;
	wait();
	b_reg[0x08F] = 0b0000000010001111;
	wait();
	b_reg[0x08F] = 0b0000000010001111;
	wait();
	b_reg[0x08F] = 0b0000000010000101;
	wait();
	b_reg[0x08F] = 0b0000000010001111;
	wait();

	
}
///*/

#endif

