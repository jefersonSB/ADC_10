		
# ifndef TOP_REG_H
# define TOP_REG_H

# include <systemc.h>	
		

typedef sc_bv <16> typ_reg;

SC_MODULE(Top_reg){
	
	// In_Out_Ports
	sc_in <bool> read;				// Data read request entry
	sc_out < typ_reg >	ADC10_A_out;			
	
	//Variables 
	typ_reg ADC10_A[10];			// Vetor de conteúdo de registradores
	sc_bv <16> tmp;
	// Methods
	void init_state_reg();			// 
	void update_reg();		//

	
	SC_CTOR(Top_reg): ADC10_A_out("ADC10_A_out"), read("read"),tmp("0000000000000000")
	{

	SC_METHOD(init_state_reg)

	SC_THREAD(update_reg)
		sensitive << read.pos();
		dont_initialize();
	} 
	~Top_reg(){}	
};


void Top_reg::init_state_reg(){
	cout << '\n'<< '\t'<< '\t'<<'\t'<<"START_STATE_TOP" << '\n';
	{	// Registradores ADC10_A
		ADC10_A[0] = 0x0001; ADC10_A[1] = 0x0002; ADC10_A[2] = 0x0003; ADC10_A[3] = 0x0004; 
		ADC10_A[4] = 0x0005; ADC10_A[5] = 0x0006; ADC10_A[6] = 0x0007; ADC10_A[7] = 0x0008;
		ADC10_A[8] = 0x0009; ADC10_A[9] = 0x000A;
	}
	
		for (int i=0;i<10; i++){
		cout << '\n'<<'\t'<<"MEMORY_CONTENTS" << '\t' <<'\t'<< 	ADC10_A[i] << '\n';
	}
}

void Top_reg::update_reg(){	

int i = 0;

	while(true){
		cout << '\n' << '\t'<< "Top_reg::update_reg()"<< '\n';
		cout << '\n' << '\t'<<"ADC10_A_out.write(ADC10_A[i])  " << '\t' << ADC10_A[i] << '\n';
		ADC10_A_out.write(ADC10_A[i]);
		++i;
		if (i == 10){i = 0;}
		wait();
	}
}
/*
void Top_reg::init_state_reg(){

	ADC10_A = {ADC10CTL0, ADC10CTL1, ADC10CTL2, ADC10LO, ADC10HI,
			   ADC10MCTL0, ADC10MEM0, ADC10IE, ADC10IFG, ADC10IV}
}
*/
# endif


			
				

				
