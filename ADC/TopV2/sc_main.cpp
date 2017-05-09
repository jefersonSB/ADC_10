
#include "muxA0_to_A15.h"
#include "tb.h"
#include "srcs.h"

#include <systemc-ams.h>

int sc_main(int argn, char* argc[]){  // SystemC main program
	
sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );

	sc_clock CLK("CLK", 1.0, SC_MS );
	
	/*/ BLoco de Ports e Signals que interagem com AMS /*/
		
		sc_signal <sc_bv <4> > ADC10INCHx_sig;		// Seleção de calanal
		sc_signal <bool> shift_pr_sig;

	/*/ BLoco de Ports e Signals que interagem com AMS /*/
		sc_vector < sc_signal <double> > srcs_wave;			// Vetor de ligação entre SRSC e MUX
		srcs_wave.init(16);									// Inicialização do vetor 
		sc_vector < sc_signal <bool> > srcs_step;			// Sinal para o MUX quando SRCS produzir sinal
		srcs_step.init(16);
		sc_signal <double> outpu_mux;						

	/*/ Início módulos SytemC/*/

		tb tb_1 ("tb_1");
		tb_1.clk (CLK);
		tb_1.ADC10INCHx (ADC10INCHx_sig);
		tb_1.shift_param(shift_pr_sig);
					
		mux_A0_to_A15 mux_1 ("1_mux");
		mux_1.ADC10INCHx (ADC10INCHx_sig);
		mux_1.outpu_mux(outpu_mux);
		mux_1.input_srcs.bind(srcs_wave);			// Ligação dos Vetores 
		mux_1.consumer.bind(srcs_step);				// Ligação dos Vetores 
	
	/*/ Fin de Istancias módulos SytemC/*/
	
	
	/*/ Início Istancias módulos SytemC-AMS/*/
	
		srcs srcs_("SRCs");
		srcs_.out_tdf_de(srcs_wave[5]);
		srcs_.producer(srcs_step[5]);	
		srcs_.shift_pram(shift_pr_sig);
	/*/ Fin de Istancias módulos SytemC-AMS/*/
	
	
	
	sca_trace_file* tfa = sca_create_tabular_trace_file("testbench");  	// Open trace file
	sca_trace(tfa, CLK, "CLK");            								// Define which signal to trace
	//sca_trace(tfa, sel_bit_in , "sel_bit_in");            			// Define which signal to trace
	sca_trace(tfa, srcs_wave[5], srcs_wave[5].basename());            	// Define which signal to trace
	sca_trace(tfa, outpu_mux, "outpu_analog_source");        			 // Define which signal to trace

	sc_start(19.0, SC_MS);                     
	
	sca_close_tabular_trace_file(tfa);         // Close trace file
	
	return 0;                                  // Exit with return code 0
}
