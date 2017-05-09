
# ifndef MUX_A0_TO_A15_H
# define MUX_A0_TO_A15_H

#include <systemc.h>

/*/ Mdulo Mux de entrada para fontes analógicas /*/

SC_MODULE(mux_A0_to_A15){

/*/bloco de portas/*/

	sc_in <sc_bv < 4 > > ADC10INCHx;			// Segmento de registrador de seleção de canal(Table 27-8. ADC10MCTL0) 
	sc_out <double> outpu_mux;					// Saída de tenção para o Semple and Hold
	sc_vector < sc_in < double > > input_srcs;	// Vetor portas de entrada A0 to A15 para conexão de fonte 
	sc_vector < sc_in < bool > > consumer;		// Vetor de entrada para sensitividades de sinal de fonte  
	/*/Fim/*/
	
/*/ váriáveis internas de trabalho/*/

	sc_uint <4> tmp; 		// Armazena o canal anterior 
	sc_bv <4> channel;		// Armazena o canal atual 
	/*/Fim /*/

/*/Bloco de Métodos/*/

	void read_bit_sel();	// Função de leitura de bits de seleção de canal (ADC10INCHx)  
	void read_channel();	// Função de leitura de entrada analógica para o canal selecionado
	/*/Fim /*/
		
/*/Bloco Construtor de SC/*/

	SC_CTOR(mux_A0_to_A15)
		: ADC10INCHx ("ADC10INCHx"), input_srcs ("Input_Srcs",16)	// Inicialização de variáveis 	
		, consumer("Consumer",16), outpu_mux ("Outpu_mux")
		, channel ("0000"), tmp (0x00)
	{
		SC_METHOD(read_bit_sel);				
			sensitive << ADC10INCHx;
			dont_initialize();
		
		SC_METHOD(read_channel);
			for (int i=0;i<16;i++){	sensitive << consumer[i]; } 	 // Torna o metodo senssitivo a qualquer que seja as entradas
			dont_initialize();										 // onde uma fonte análógica pode ser ligada

		cout << '\n'<< '\t'<< "Canais MUX_A0_to_A15"
		<< '\t'<< "Lista Sensitiva do MUX"<< endl;
		
		for (int i=0;i<16;i++){
			cout << '\t'<< input_srcs[i].basename()					
			<< '\t'<< '\t'<< consumer[i].basename()<< endl;			
		}
	}
	/*/Fim /*/
};

void mux_A0_to_A15::read_bit_sel(){

	cout << " Canal Anterior: "<< tmp << " Atual: " << channel << endl;
	
	if (tmp != ADC10INCHx.read()){
		channel = ADC10INCHx.read();
		tmp = channel;
		cout <<"ctr_chanel.notify("<< tmp <<")" << endl <<endl;	
	}
}

void mux_A0_to_A15::read_channel(){
		
	outpu_mux.write(input_srcs[tmp].read()*0.5);
}

#endif
