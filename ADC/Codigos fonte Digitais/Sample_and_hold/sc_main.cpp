

# include "sample_and_hold.h"
# include <systemc.h>

int sc_main(int argn, char* argc[]){  // SystemC main program
sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
	
	sc_clock clk("clk", 1.0 , SC_MS);
	
	sc_signal < double >  x;
	sc_signal < double >  y;
	
	
	sample_and_hold sample_and_hold_("sample_and_hold_");
	sample_and_hold_.input(x);
	sample_and_hold_.output(y);
	sample_and_hold_.clk(clk);
	
		
	
	sc_start(17, SC_NS);	

return 0;
}
