

#include <systemc-ams>

SCA_TDF_MODULE (drive_conv_port){
	
	sca_tdf::sca_out < bool >  adc12clk_tdf;
	sca_tdf::sc_in < bool >  adc12clk_de;

	void set_attributes();                	/// Define atributos relacionados ao modelo TDF 
	void processing();                      /// Decrive o comportamento no domínio do tempo 

	
	SCA_CTOR(drive_conv_port)
	: adc12clk_tdf ("adc12clk_tdf")
	, adc12clk_de ("adc12clk_de")
	{}
};

void drive_conv_port::set_attributes(){ 
	adc12clk_tdf.set_timestep(1.0, SC_NS);
}

void drive_conv_port::processing(){

	if (adc12clk_de.read() == true){
		adc12clk_tdf.write(true);
	}else if(adc12clk_de.read() == false){
		adc12clk_tdf.write(false);
		}
}
