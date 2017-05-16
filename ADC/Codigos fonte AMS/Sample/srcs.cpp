
#include "srcs.h"
#include <cstdlib> // for std::rand
#include <cmath>   // for M_PI, std::sin, std::sqrt, and std::log

// Método de inicialização

void srcs::initialize(){		
	ampl = 5.0;
	freq = 1e3;
	data = 0.0; 
	t = 0.0; 
}

// Método de alteração de parâmetros de fonte

void srcs::shift(double a, double f){	 
	ampl = a;freq = f;
}

// Set atributos do modelo TDF 

void srcs::set_attributes(){ 
	out_tdf.set_timestep(1.0, SC_NS);
}

// Método de alteração de parâmetros de fonte

void srcs::processing(){
	if (ctrl_in.read() == true){
		ctrl_out.write(true);
	}else if(ctrl_in.read() == false){
		ctrl_out.write(false);
		}

	t = out_tdf.get_time().to_seconds();       			/// Get current time of the sample
	out_tdf.write(ampl * sin(2.0 * M_PI * freq * t)); 	/// escrita de valor na saída 
	///out_tdf.write(5);								/// para teste de carga e escar de capacitor

}
