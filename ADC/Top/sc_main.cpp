
# include "muxA0_to_A15.h"
# include "B_reg.h"
# include "srcs.h"
# include "mux_shp.h"
# include "sample.h"
# include "drive_conv_port.h"
# include "sar.h"
# include <systemc.h>

int sc_main(int argc, char* argv[]){
	
	sc_clock clk("clk", 1, SC_MS);										///Clock para controle de banco de registradores
	sc_time delay (0.2, SC_US);

	sc_clock ADC12CLK("ADC12CLK",delay , SC_US);						/// Clock pra core do SAR 

	sc_clock klsps_200("klsps_200",20.0 , SC_US);						/// Clock pra core do SAR 
		
	sc_signal<bool> sampcon_de;
	sca_tdf::sca_signal<bool> sampcon_tdf;
		
/// BLoco de Ports e Signals que interagem com AMS /*/
	sc_vector < sc_signal <double> > srcs_wave;							/// Vetor de ligação entre SRSC e MUX
	srcs_wave.init(16);													/// Inicialização do vetor 
	sc_vector < sc_signal <bool> > srcs_step;							/// Sinal para o MUX quando SRCS produzir sinal
	srcs_step.init(16);
	sc_signal <double> outpu_mux;										/// Saída do Mux 
	sc_signal <double> sample_sig;											/// Saída do Sample and hold
///	sc_signal <sc_bv <12> > dig_result_sig;											/// Saída do Sample and hold
	sc_signal < double > dig_result_sig;											/// Saída do Sample and hold
	
/// Fin
								
/// Drive apra converção de portas entre domínios 
	drive_conv_port drive_conv_port_1("drive_conv_port_1");
	drive_conv_port_1.sampcon_tdf (sampcon_tdf);
	drive_conv_port_1.sampcon_de (sampcon_de);
/// Fin

/// Banco de registradores	
	Ban_Reg Ban_Reg_("Ban_Reg_");
	Ban_Reg_.shift_channel(clk);
/// Fin

/// Fonte de sinal senoidal
	srcs srcs_("SRCs");													/// fonte instanciada no canal 5
	srcs_.out_tdf_de(srcs_wave[5]);
	srcs_.producer(srcs_step[5]);	
/// Fin

/// Mux de entrada de ADC12-MSP430		
	muxA0_to_A15 muxA0_to_A15_("muxA0_to_A15_");
	muxA0_to_A15_.socket.bind( Ban_Reg_.target_mux_A0_A15 );
	muxA0_to_A15_.outpu_mux(outpu_mux);
	muxA0_to_A15_.input_srcs.bind(srcs_wave);							/// Ligação dos Vetores 
	muxA0_to_A15_.consumer.bind(srcs_step);								/// Ligação dos Vetores 
/// Fin

/// MUX de seleção de modo de amostragem de sample and hold
	mux_shp mux_shp_1("mux_shp_1");
	mux_shp_1.socket.bind( Ban_Reg_.target_mux_SHP );
	mux_shp_1.sampcon_port(sampcon_de);
	mux_shp_1.klsps_200(klsps_200);
	
/// Fin

/// Sample and hold 
	sample sample_1 ("sample_1");
	sample_1.in_src(outpu_mux);
	sample_1.out(sample_sig);
	sample_1.ctrl_lp(sampcon_tdf);
	
/// Fin

/// Core SAR 
	sar sar_1 ("sar_1");
	sar_1.socket.bind( Ban_Reg_.target_sar );
	sar_1.adc12clk(ADC12CLK);
	sar_1.convert(sampcon_de);
	sar_1.sample_port_in(sample_sig);
	sar_1.sample_port_out(dig_result_sig);
/// Fin

	sca_trace_file* tfa = sca_create_tabular_trace_file("testbench");  	/// Open trace file
	sca_trace(tfa, clk, "clk");            								/// Define which signal to trace
	sca_trace(tfa, ADC12CLK, "ADC12CLK");            								/// Define which signal to trace
	sca_trace(tfa, klsps_200, "klsps_200");            								/// Define which signal to trace
	sca_trace(tfa, sampcon_tdf, "sampcon_tdf");            			/// Define which signal to trace
	

	sca_trace(tfa, srcs_wave[5], srcs_wave[5].basename());            	/// Define which signal to trace
	sca_trace(tfa, outpu_mux, "outpu_mux");        						/// Define which signal to trace
	sca_trace(tfa, sample_sig, "sample_sig");            			/// Define which signal to trace
	sca_trace(tfa, dig_result_sig, "dig_result_sig");            			/// Define which signal to trace
	
	
	
	sc_start(3.0, SC_MS);                     
	
	sca_close_tabular_trace_file(tfa);         /// Close trace file

return 0;
}
