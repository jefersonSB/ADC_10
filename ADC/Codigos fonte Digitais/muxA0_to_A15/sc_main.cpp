
#include "muxA0_to_A15.h"
#include "tb.h"

int sc_main(int argn, char* argc[]){  // SystemC main program
//sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );

	sc_clock CLK("CLK", 1.0, SC_MS );
	
	sc_signal <sc_dt::sc_bv <4> > sel_bit_in;
	sc_signal <double> outpu_analog_source;
	
	sc_vector <sc_signal <double> > in_a_src;
	in_a_src.init(15);
	
	
	mux_A0_to_A15 mux_1 ("1_mux");
 	mux_1.ADC10INCHx (sel_bit_in);
	mux_1.input_analog_source.bind(in_a_src);
	mux_1.outpu_analog_source (outpu_analog_source);
	
	tb tb_1 ("tb_1");
	tb_1.clk(CLK);
	tb_1.ADC10INCHx(sel_bit_in);

	
	sc_trace_file* tfd =                      			// Open trace file
	sc_create_vcd_trace_file("traces");
	sc_trace(tfd, sel_bit_in, "sel_bit_in");            // Define which signal to trace
	sc_trace(tfd, CLK, "CLK");            				// Define which signal to trace
	
/*	

	sca_trace_file* tfa =                      // Open trace file
	sca_create_tabular_trace_file("testbench");
	sca_trace(tfa, out_tdf, "out_tdf");            // Define which signal to trace
	sca_trace(tfa, out_tdf_de, "out_tdf_de");            // Define which signal to trace
	sca_trace(tfa, CLK, "CLK");            // Define which signal to trace
*/
	sc_start(17.0, SC_MS);                     
	sc_close_vcd_trace_file(tfd);
	//sca_close_tabular_trace_file(tfa);         // Close trace file
	
	return 0;                                  // Exit with return code 0
}
