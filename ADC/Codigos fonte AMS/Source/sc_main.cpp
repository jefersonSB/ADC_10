
# include "sine.h"
//# include "muxA0_to_A15.h"
//#include "auto_.h"
//# include "tb.h"

int sc_main(int argn, char* argc[]){  // SystemC main program

sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
	sc_clock CLK("CLK", 1.0, SC_MS );
//	sca_tdf::sca_signal	<typedata>	analog_nod_in;		// Voltage source input
	sca_tdf::sca_signal	<typedata>	out_tdf;
	sc_core::sc_signal 	<typedata> 	out_tdf_de;	
	
//	sc_signal <sc_dt::sc_bv 	<4> > 		sel_bit_in;
//	sc_signal <sc_dt::sc_bv 	<4> > 		sig_monitor;

	
	sine src_1("src_1");
	src_1.out_tdf		(out_tdf);
	src_1.out_tdf_de	(out_tdf_de);

	/*
	
	tb tb_1 ("tb_1");
	tb_1.clk(CLK);
	tb_1.ADC10INCHx_in(sel_bit_in);

	
	//sc_signal <sc_dt::sc_bv 	<4> > sel_bit_ou;
	//sc_signal <sc_dt::sc_bv 	<2> > mod_op;
	//sc_signal<bool> ADC10_ON;
	
	
/*	
	sca_trace_file* tfa =                      // Open trace file
	sca_create_tabular_trace_file("testbench");
	sca_trace(tfa, analog_nod_in, "analog_nod_in");            // Define which signal to trace
	sca_trace(tfa, analog_nod_out, "analog_nod_out");            // Define which signal to trace
	//	sc_trace_file *tfd;
	//	tfd = sc_create_vcd_trace_file("traces");
	//sc_trace(tfd, analog_nod_out, "analog_nod_out");            // Define which signal to trace

*/
	sca_trace_file* tfa =                      // Open trace file
	sca_create_tabular_trace_file("testbench");
	sca_trace(tfa, out_tdf, "out_tdf");            // Define which signal to trace
	sca_trace(tfa, out_tdf_de, "out_tdf_de");            // Define which signal to trace
	sca_trace(tfa, CLK, "CLK");            // Define which signal to trace

	sc_start(10.0, SC_MS);                     
	//sc_close_vcd_trace_file(tfd);
	sca_close_tabular_trace_file(tfa);         // Close trace file
	
	return 0;                                  // Exit with return code 0
}
