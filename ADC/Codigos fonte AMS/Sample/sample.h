# include <systemc-ams.h>
#include <cmath>   // for M_PI, std::sin, std::sqrt, and std::log

SC_MODULE(sample) {

	sca_tdf::sca_in<double> in_src;										/// entrada da fonte src TDF
	sca_tdf::sca_out<double> out;										/// 
	sca_tdf::sca_in<bool> ctrl_lp;										/// entrada da fonte src TDF

	sca_eln::sca_tdf_rswitch key;										/// Chave 
	
	sca_eln::sca_node n1,n2,n3,n4;										/// electrical nodes
	sca_eln::sca_node_ref gnd;
	
	sca_eln::sca_c CI;													/// capacitancia de entrada 
	sca_eln::sca_r RI;													/// resistencia de entrada interna do MUX
	sca_eln::sca_r RS;													/// resistencia da fonte externa 
	
	sca_eln::sca_tdf::sca_vsource vin_src;								/// TDF to voltage converter
	sca_eln::sca_tdf::sca_vsink vout;									/// voltage to TDF converter
		
	double freq_cut;
	
	SC_CTOR(sample)
		: CI("CI"),RI("RI"),vin_src("vin_src"), vout("vout")
		, RS ("RS"), key("key")
		
	{
		double Rs = 0e3; 												/// choose fixed R
		double Ri = 2e3; 												/// choose fixed R
///		double C = 1/(2*M_PI*Ri*freq_cut);								/// and compute C relative to it
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

		cout << "5*Tau " << t << endl;
		cout << "Frequência de Corte:  " << freq_c << endl;
		cout << "tsample >= " << tsample << endl;
		
		vout.outp(out);
		vout.p(n4); 								///vout.tdf_voltage(out);
		vout.n(gnd);
	}
};
