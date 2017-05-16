
#include "sample.h"
#include "srcs.h"

/// o clock máximo de operação do adc é 200kHZ / referente a 200Ksps
/// t = 1/200*1e3 = 5us;

int sc_main(int argc, char* argv[]){
	
	sc_clock clk("clk", 10, SC_US); 									

/// BLoco de Ports e Signals que interagem com AMS /*/

	sca_tdf::sca_signal<double> in_src;
	sca_tdf::sca_signal<double> out;
	sca_tdf::sca_signal<bool> ctrl_;
	
	sample sample_1("Sample_1");
	sample_1.in_src(in_src);
	sample_1.out(out);
	sample_1.ctrl_lp(ctrl_);

	srcs srcs_("SRCs");
	srcs_.out_tdf(in_src);
	srcs_.ctrl_in(clk);
	srcs_.ctrl_out(ctrl_);
	
	sca_trace_file* tfa = sca_create_tabular_trace_file("testbench");  	/// Open trace file
	sca_trace(tfa, in_src, "in_src");        							/// Define which signal to trace
	sca_trace(tfa, out, "out_");            							/// Define which signal to trace
	sca_trace(tfa, clk, "clk");            								/// Define which signal to trace
	
	sc_start(1.0, SC_MS);                     
	
	sca_close_tabular_trace_file(tfa);         							/// Close trace file

return 0;
}
