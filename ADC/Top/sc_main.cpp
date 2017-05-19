
# include "muxA0_to_A15.h"
# include "B_reg.h"
# include "srcs.h"
# include "mux_shp.h"
# include "sample.h"
# include "drive_conv_port.h"
///# include "sar.h"

int sc_main(int argc, char* argv[]){
	
	sc_clock clk("clk", 2, SC_MS);
	sc_signal<bool> ADC12CLK_DE;
	sca_tdf::sca_signal<bool> ADC12CLK_TDF;
		
/// BLoco de Ports e Signals que interagem com AMS /*/
	sc_vector < sc_signal <double> > srcs_wave;							/// Vetor de ligação entre SRSC e MUX
	srcs_wave.init(16);													/// Inicialização do vetor 
	sc_vector < sc_signal <bool> > srcs_step;							/// Sinal para o MUX quando SRCS produzir sinal
	srcs_step.init(16);
	sc_signal <double> outpu_mux;										/// Saída do Mux 
	sc_signal <double> outpu_sample;										/// Saída do Sample and hold
/// Fin
								
/// Drive apra converção de portas entre domínios 
	drive_conv_port drive_conv_port_1("drive_conv_port_1");
	drive_conv_port_1.adc12clk_tdf (ADC12CLK_TDF);
	drive_conv_port_1.adc12clk_de (ADC12CLK_DE);
/// Fin

/// Banco de registradores	
	Ban_Reg Ban_Reg_("Ban_Reg_");
	Ban_Reg_.shift_channel(clk);
/// Fin

/// Fonte de sinal senoidal
	srcs srcs_("SRCs");
	srcs_.out_tdf_de(srcs_wave[5]);
	srcs_.producer(srcs_step[5]);	
/// Fin

/// Mux de entrada de ADC12-MSP430		
	muxA0_to_A15 muxA0_to_A15_("muxA0_to_A15_");
	muxA0_to_A15_.socket.bind( Ban_Reg_.target1 );
	muxA0_to_A15_.outpu_mux(outpu_mux);
	muxA0_to_A15_.input_srcs.bind(srcs_wave);								/// Ligação dos Vetores 
	muxA0_to_A15_.consumer.bind(srcs_step);								/// Ligação dos Vetores 
/// Fin

/// MUX de seleção de modo de amostragem de sample and hold
	mux_shp mux_shp_1("mux_shp_1");
	mux_shp_1.socket.bind( Ban_Reg_.target2 );
	mux_shp_1.ADC12CLK (ADC12CLK_DE);
/// Fin

/// Sample and hold 
	sample sample_1 ("sample_1");
	sample_1.in_src(outpu_mux);
	sample_1.out(outpu_sample);
	sample_1.ctrl_lp(ADC12CLK_TDF);
/// Fin

/// Core SAR 
///	sar sar_1 ("sar_1");
///	sar_1.adc12clk();
///	sar_1.convert();
///	sar_1.sample_port();
///	sar_1.sample_port_out();
/// Fin

	sca_trace_file* tfa = sca_create_tabular_trace_file("testbench");  	/// Open trace file
///	sca_trace(tfa, clk, "clk");            								/// Define which signal to trace
///	sca_trace(tfa, srcs_wave[5], srcs_wave[5].basename());            	/// Define which signal to trace
	sca_trace(tfa, ADC12CLK_TDF, "ADC12CLK_TDF");            			/// Define which signal to trace
	sca_trace(tfa, outpu_mux, "outpu_mux");        						/// Define which signal to trace
	sca_trace(tfa, outpu_sample, "outpu_sample");            			/// Define which signal to trace
	
	sc_start(1.0, SC_MS);                     
	
	sca_close_tabular_trace_file(tfa);         /// Close trace file

return 0;
}
