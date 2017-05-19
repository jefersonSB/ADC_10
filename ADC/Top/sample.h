
/**
 * \file sample.h
 * \brief sample file.
 * 
 * \author Jeferson Santos Barros <jeferson.s.barros@posgrad.ufsc.br>
 * 
 * \version 1.0-dev
 * 
 * \date 15/05/2017
 * 
 * \defgroup main Main
 * \{
 */

///------------------------------------------
/// Modulo Sample and Hold ADC12-MSP430F249 |
///------------------------------------------
/**
 * Esse módulo é controlador por SAMPCOM em dois modos (Estendido e Pulso)   
 * Quando SAMPCON = 1 Sample and Hold ativo e amostra o sinal com o ADC12CLK,
 * quando SAMPCON = 0 Sample desativado. SAMPCON vem do blo MUX_SHP
 * 
 * Resolução (LSB) = Vmax/(2^n) = 5V/4095 = 10,101 mV 
 * A tensão VC do do capacitor CI deve ser carregada dentro de 1/2 LSB 
 * da tensão de fonte VS para uma conversão precisa de 12 bits.
 * 
*/

# include <systemc-ams.h>
#include <cmath>  

SC_MODULE(sample) {

	sc_in<double> in_src;												/// entrada do sinal de da fonte pós MUX
	sc_out<double> out;													/// saida do sinal de da fonte amostrado
	sca_tdf::sca_in<bool> ctrl_lp;										/// entrada clk para o switch

	sca_eln::sca_tdf_rswitch key;										/// switch ideal 
	
	sca_eln::sca_node n1,n2,n3,n4;										/// electrical nodes
	sca_eln::sca_node_ref gnd;
	
	sca_eln::sca_c CI;													/// capacitancia de entrada 
	sca_eln::sca_r RI;													/// resistencia de entrada interna do Sample
	sca_eln::sca_r RS;													/// resistencia da fonte externa 
	
	sca_eln::sca_de::sca_vsource vin_src;								/// Fonte de tensão vinda do MUX
	sca_eln::sca_de::sca_vsink vout;									/// Tensão de saída amostrada 
		
	
	SC_CTOR(sample)
		: CI("CI"),RI("RI"),vin_src("vin_src"), vout("vout")
		, RS ("RS"), key("key")
		
	{
		double Rs = 0e3; 												/// choose fixed R
		double Ri = 2e3; 												/// choose fixed R
		double C = 40e-12;
		
		vin_src.inp(in_src);											///  
		vin_src.p(n1); 													/// 
		vin_src.n(gnd); 

		RS.value = Rs;
		RS.n(n1);
		RS.p(n2);
		
		RI.value = Ri;
		RI.n(n2);
		RI.p(n3);

		key.p(n4);
		key.n(n3);
		key.ctrl(ctrl_lp);
		key.ron = 0e3;
		key.roff = 1e16;

		CI.value = C;
		CI.p(n4);
		CI.n(gnd);

		double t = 5*((Rs+Ri)*C);											
		double tsample =((Rs+Ri)*(9.011)*C + 800e-9);					/// especificação de tempo de amostrage em relação a Rs
		double freq_c = 1/(2*M_PI*(Ri+Rs)*C); 

		cout <<'\n' << '\t' << "Parâmetros Sample: " << t << endl;
		cout <<'\n' << '\t' << "5*Tau " << t << endl;
		cout << '\t' << "Frequência de Corte:  " << freq_c << endl;
		cout <<  '\t' << "tsample >= " << tsample << endl;
		
		vout.outp(out);
		vout.p(n4); 													///vout.tdf_voltage(out);
		vout.n(gnd);
	}
};
