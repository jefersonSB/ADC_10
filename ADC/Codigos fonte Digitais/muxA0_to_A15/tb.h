		
# ifndef TB_H
# define TB_H

# include <systemc.h>			

SC_MODULE(tb){
	
	sc_out	<sc_dt::sc_bv <4> > ADC10INCHx;	// Channel selection input  4btis
	//sc_out	<sc_dt::sc_bv <2> > ADC10CONSEQx_in;	// Channel selection input  4btis
	sc_in <bool> clk;
	sc_out <bool> shift_param;
	
	void _write();
	
		SC_CTOR(tb)
		{
			SC_CTHREAD(_write, clk.pos())
		} 
		~tb(){}	
};

void tb::_write(){

shift_param.write (false);
		
		ADC10INCHx.write("0000");	wait();
		ADC10INCHx.write("0001");	wait();
		ADC10INCHx.write("0010");	wait();
		ADC10INCHx.write("0011");	wait();

shift_param.write (true);
		ADC10INCHx.write("0100");	wait();
		ADC10INCHx.write("0101");	wait();
		ADC10INCHx.write("0101");	wait();
		ADC10INCHx.write("0101");	wait();
		
		ADC10INCHx.write("1000");	wait();
		ADC10INCHx.write("1001");	wait();
		ADC10INCHx.write("1010");	wait();
		ADC10INCHx.write("1011");	wait();
		
shift_param.write (false);
		ADC10INCHx.write("1100");	wait();
		ADC10INCHx.write("0101");	wait();
		ADC10INCHx.write("0101");	wait();
		ADC10INCHx.write("1111");	wait();
shift_param.write (true);
		
		ADC10INCHx.write("0000");	wait();
		ADC10INCHx.write("0000");	wait();

	//	*/
};
# endif
			
				

				
