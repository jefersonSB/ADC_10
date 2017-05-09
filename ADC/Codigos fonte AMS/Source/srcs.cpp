
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
	out_tdf_de.set_timestep(1.0, SC_US);
}

// Método de alteração de parâmetros de fonte

void srcs::processing(){

	if (shift_pram.read() == true){
		shift(3.0, 0.3e3);
	}else if(shift_pram.read() == false){
		shift(5.0, 1e3);
		}

	producer.write(true);									// notificação de produção de amostra
	t = out_tdf_de.get_time().to_seconds();       			// Get current time of the sample
	out_tdf_de.write(ampl * sin(2.0 * M_PI * freq * t)); 	// escrita de valor na saída 
	producer.write(false);									// notificação de fim produção de amostra	
}
