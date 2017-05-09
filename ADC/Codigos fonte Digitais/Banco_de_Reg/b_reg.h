
#ifndef B_REG_H
#define B_REG_H

# include  <systemc.h>

typedef sc_bv <16> typ_reg;
typedef sc_uint <8>  type_address;

SC_MODULE(b_reg){
		
	// In_Out_Ports

	sc_out <bool> request;				// Data read request entry
	sc_in < typ_reg> ADC10_A_in;
	sc_in <bool> clk;
	
	//Variables 
	type_address  address[10];		// Vetor de endereço de Memória de registrador
	typ_reg ADC10_A[10];			// Vetor de conteúdo de registradores
	
	typ_reg *ADC10CTL0;				//00h ADC10CTL0 ADC10_A Control 0 register Read/write 0000h Section 27.3.1
	typ_reg *ADC10CTL1;				//02h ADC10CTL1 ADC10_A Control 1 register Read/write 0000h Section 27.3.2
	typ_reg *ADC10CTL2;				//04h ADC10CTL2 ADC10_A Control 2 register Read/write 1000h Section 27.3.3
	typ_reg *ADC10LO;				//06h ADC10LO ADC10_A Window Comparator Low Threshold register Read/write 0000h Section 27.3.9
	typ_reg *ADC10HI;				//08h ADC10HI ADC10_A Window Comparator High Threshold register Read/write FF03h Section 27.3.7
	typ_reg *ADC10MCTL0;			//0Ah ADC10MCTL0 ADC10_A Memory Control register Read/write 00h Section 27.3.6
	typ_reg *ADC10MEM0;				//12h ADC10MEM0 ADC10_A Conversion Memory register Read/write undefined Section 27.3.4
	typ_reg *ADC10IE;				//1Ah ADC10IE ADC10_A Interrupt Enable register Read/write 0000h Section 27.3.11
	typ_reg *ADC10IFG;				//1Ch ADC10IFG ADC10_A Interrupt Flag register Read/write 0000h Section 27.3.12
	typ_reg *ADC10IV;				//1Eh ADC10IV ADC10_A Interrupt Vector register Read/write 0000h Section 27.3.13
	
	
	// Methods
	void init_state_reg_address();
	void request_reg();
	void hear_reg();
	
	SC_CTOR(b_reg):request("request"),ADC10_A_in("ADC10_A_in"),clk("clk")
	{
		SC_METHOD(init_state_reg_address);	
		
//		SC_CTHREAD(request_reg, clk.neg());	

		SC_THREAD(request_reg);	
			sensitive << clk.neg();
			dont_initialize();


		SC_THREAD(hear_reg);	
			sensitive << ADC10_A_in;
			dont_initialize();
	}
};

void b_reg::hear_reg(){

int i =0;

	while(true){

		cout << '\n' << '\n' << '\t'<< '\t'<< "OUVINDO REGISTRADOR" << '\t';
		ADC10_A[i] = ADC10_A_in.read();
		cout << '\n'<< '\t'<< "ADC10_A Register" << '\t'<< ADC10_A[i] << '\n';
		i++;
		if (i == 10){i = 0;}
		wait();
	}
}

void b_reg::request_reg(){

	while(true){
		cout << '\n' << '\t'<< "Requisição"<< '\n';
		request.write(true);
		wait(0.05,SC_MS);
		request.write(false);
		wait(0.05,SC_MS);
	}
}

void b_reg::init_state_reg_address(){
	cout << '\n'<< '\t'<<'\t'<< '\t'<<'\t'<<"INITIAL_MIRROR_ESTATE" << '\n';
	
	{	// Registradores ADC10_A
		ADC10_A[0] = 0x0000; ADC10_A[1] = 0x0000; ADC10_A[2] = 0x0000; ADC10_A[3] = 0x0000; 
		ADC10_A[4] = 0x0000; ADC10_A[5] = 0x0000; ADC10_A[6] = 0x0000; ADC10_A[7] = 0x0000;
		ADC10_A[8] = 0x0000; ADC10_A[9] = 0x0000;
	}
	
	{  // Endereços de registradores 
		address[0] = 0x00; address[1] = 0x02; address[2] = 0x04; address[3] = 0x06; 
		address[4] = 0x08; address[5] = 0x0A; address[6] = 0x12; address[7] = 0x1A;
		address[8] = 0x1C; address[9] = 0x1E;
	}

	{  // Padronização de Nomes de registradores com Data Sheet
		ADC10CTL0 = & ADC10_A[0]; ADC10CTL1 = &ADC10_A[1]; ADC10CTL2 = &ADC10_A[2];
		ADC10LO = &ADC10_A[3]; ADC10HI = &ADC10_A[4]; ADC10MCTL0 = &ADC10_A[5]; 
		ADC10MEM0 = &ADC10_A[6]; ADC10IE = &ADC10_A[7]; ADC10IFG = &ADC10_A[8];
		ADC10IV= &ADC10_A[9];
	}
		
	for (int i=0;i<10; i++){
		cout << '\n' << '\t'<< "REGISTERS_ADDRESS"<<'\t'<< address[i]<< 
		'\t'<< "MEMORY_CONTENTS"<<'\t'<< ADC10_A[i]<<'\n';
	}
	
	cout << '\n' << '\t' << '\t'<< "PONTEIROS DE REGISTRADORES ADC10_A" << 	'\n';
	cout << '\t' << "ADC10CTL0" << '\t' <<	*ADC10CTL0 <<'\n';
	cout << '\t' << "ADC10CTL1" << '\t' <<	*ADC10CTL1 <<'\n';	
	cout << '\t' << "ADC10CTL2" << '\t' <<	*ADC10CTL2 <<'\n';
	cout << '\t' << "ADC10LO  " << '\t' <<	*ADC10LO <<'\n'	;
	cout << '\t' << "ADC10HI  " <<'\t'  << 	*ADC10HI <<'\n';
	cout << '\t' << "ADC10MCTL0"<<'\t'  <<   *ADC10MCTL0 <<'\n';
	cout << '\t' << "ADC10MEM0" <<'\t'  <<   *ADC10MEM0 <<'\n';
	cout << '\t' << "ADC10IE "  << '\t' <<	*ADC10IE <<'\n';
	cout << '\t' << "ADC10IFG " << '\t' <<	*ADC10IFG <<'\n';
	cout << '\t' << "ADC10IV   "<< '\t' <<	*ADC10IV <<'\n';
}


#endif
