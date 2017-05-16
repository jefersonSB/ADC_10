

#include "muxA0_to_A15.h"
#include "B_reg.h"
#include "srcs.h"

int sc_main(int argc, char* argv[]){
	
	
	sc_clock clk("clk", 2, SC_MS);

/// BLoco de Ports e Signals que interagem com AMS /*/
	sc_vector < sc_signal <double> > srcs_wave;							/// Vetor de ligação entre SRSC e MUX
	srcs_wave.init(16);													/// Inicialização do vetor 
	sc_vector < sc_signal <bool> > srcs_step;							/// Sinal para o MUX quando SRCS produzir sinal
	srcs_step.init(16);
	sc_signal <double> outpu_mux;							

	srcs srcs_("SRCs");
	srcs_.out_tdf_de(srcs_wave[5]);
	srcs_.producer(srcs_step[5]);	
	
	Ban_Reg Ban_Reg_("Ban_Reg_");
	Ban_Reg_.shift_channel(clk);
		
	muxA0_to_A15 initiator_("initiator_");
	initiator_.socket.bind( Ban_Reg_.target );
	initiator_.outpu_mux(outpu_mux);
	initiator_.input_srcs.bind(srcs_wave);								/// Ligação dos Vetores 
	initiator_.consumer.bind(srcs_step);								/// Ligação dos Vetores 


	sca_trace_file* tfa = sca_create_tabular_trace_file("testbench");  	/// Open trace file
	sca_trace(tfa, srcs_wave[5], srcs_wave[5].basename());            	/// Define which signal to trace
	sca_trace(tfa, outpu_mux, "outpu_mux");        						/// Define which signal to trace
	sca_trace(tfa, clk, "clk");            								/// Define which signal to trace
	
	sc_start(19.0, SC_MS);                     
	
	sca_close_tabular_trace_file(tfa);         // Close trace file

return 0;
}
