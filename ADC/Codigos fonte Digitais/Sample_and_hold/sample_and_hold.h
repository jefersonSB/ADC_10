


SC_MODULE (sample_and_hold){
	
	// portas de sinais analógicos 
	sc_in < double > input; 		// entrada de sinal analogico do MUX  
	sc_out < double > output;		// saida de sinal amostrado para o Core do conversor
	
	// portas de registradores
	sc_in <bool> ADC10CLK;				
	sc_in < bool > SAMPCON;
	
	// variaveis
	
	
	// metodos 
	void amostrador();
	
	SC_CTOR(sample_and_hold){
	
	
		SC_METHOD(amostrador);
			sensitive << ADC10CLK.pos();
			
		SC_THREAD(x);
			sensitive << ;
	}
}

void sample_and_hold::(amostrador){
	
	
	

}
