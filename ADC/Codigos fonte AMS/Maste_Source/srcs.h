
#ifndef SRCS_H
#define SRCS_H

#include <systemc-ams.h>                   

SCA_TDF_MODULE(srcs){
	
/// Bloco de portas/*/
	sca_tdf::sc_out < double>  out_tdf_de;								/// Porta convertida para dominio Discréto
	sca_tdf::sc_out < bool> producer;									/// Geração estímulo para o MUX ao produzir sinal
///		sca_tdf::sc_in	<bool>   shift_pram;							/// Entrada para mudar parâmetros de fontes
	///Fim/*/
		
///parametros
	double ampl, freq, t, data;           								///Amplitude, Frequência,
	///Fim/*/

/// Bloco de Metodos
	void set_attributes();                								/// Define atributos relacionados ao modelo TDF 
	void initialize(); 													/// Inicializa estado 
	void processing();                     								/// Decrive o comportamento no domínio do tempo 
	void shift (double a, double f);
	///Fim/*/

/// Bloco construtor
	SCA_CTOR(srcs)
		: out_tdf_de("out_tdf_de")
		, producer ("producer")
		///, shift_pram("shift_pram")
	{}
	///Fim/*/
	
};
#endif /// SIN_SOURCE_WITH_NOISE_H 
