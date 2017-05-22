

#include <systemc-ams>

SCA_TDF_MODULE (drive_conv_port){
	
	sca_tdf::sca_out < bool >  sampcon_tdf;
	sca_tdf::sc_in < bool >  sampcon_de;

	void set_attributes();                	/// Define atributos relacionados ao modelo TDF 
	void processing();                      /// Decrive o comportamento no domínio do tempo 

	
	SCA_CTOR(drive_conv_port)
	: sampcon_tdf ("sampcon_tdf")
	, sampcon_de ("sampcon_de")
	{}
};

void drive_conv_port::set_attributes(){ 
	sampcon_tdf.set_timestep(1.0, SC_NS);
}

void drive_conv_port::processing(){

	if (sampcon_de.read() == true){
		sampcon_tdf.write(true);
	}else if(sampcon_de.read() == false){
		sampcon_tdf.write(false);
		}
}
