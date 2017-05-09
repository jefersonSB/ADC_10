
#include "Top_reg.h"
#include "b_reg.h"

#include <systemc-ams.h>

//typedef sc_uint <16> typ_reg;

int sc_main(int argn, char* argc[]){  // SystemC main program
sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
	
	sc_clock clk("clk", 1.0 , SC_MS);
	
	sc_signal <typ_reg > ADC10_A;

	sc_signal <bool> request;
			
	Top_reg Top_reg_("Top_reg");
	Top_reg_.read(request);
	Top_reg_.ADC10_A_out(ADC10_A);
	
	b_reg b_re_("b_re_");
	b_re_.request(request);
	b_re_.ADC10_A_in(ADC10_A);
	b_re_.clk(clk);

	
	sc_trace_file* tfd = sc_create_vcd_trace_file("traces");
	sc_trace(tfd, ADC10_A, "ADC10_A");            // Define which signal to trace
	sc_trace(tfd, clk, "clk");            				// Define which signal to trace
	sc_trace(tfd, request, "request");            				// Define which signal to trace


	sc_start(30, SC_MS);                     
	sc_close_vcd_trace_file(tfd);

	return 0;                                  // Exit with return code 0
}

