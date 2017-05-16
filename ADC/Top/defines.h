
# ifndef DEFINES_H
# define DEFINES_H

#include <systemc.h>

#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#define SIZE 0x01A9 /// Tamaho do bano de registradores
typedef sc_uint<16> type_reg;

#define ADC12CTL0	0x01A0 
#define	ADC12CTL1	0x01A2 
#define	ADC12IFG	0x01A4
#define	ADC12IE		0x01A6 
#define	ADC12IV		0x01A8 
	
#define	ADC12MEM0	0x0140 
#define	ADC12MEM1	0x0142 
#define	ADC12MEM2	0x0144 
#define	ADC12MEM3	0x0146 
#define	ADC12MEM4	0x0148 
#define	ADC12MEM5	0x014A 
#define	ADC12MEM6	0x014C 
#define	ADC12MEM7	0x014E 
#define	ADC12MEM8	0x0150 
#define	ADC12MEM9	0x0152 
#define	ADC12MEM10	0x0154 
#define	ADC12MEM11  0x0156 
#define	ADC12MEM12  0x0158 
#define	ADC12MEM13  0x015A 
#define	ADC12MEM14  0x015C 
#define	ADC12MEM15  0x015E
 
#define	ADC12MCTL0  0x080 
#define	ADC12MCTL1  0x081 
#define	ADC12MCTL2  0x082 
#define	ADC12MCTL3  0x083 
#define	ADC12MCTL4  0x084 
#define	ADC12MCTL5  0x085 
#define	ADC12MCTL6  0x086 
#define	ADC12MCTL7  0x087 
#define	ADC12MCTL8  0x088 
#define	ADC12MCTL9	0x089 
#define ADC12MCTL10 0x08A 
#define	ADC12MCTL11 0x08B 
#define	ADC12MCTL12 0x08C 
#define	ADC12MCTL13 0x08D 
#define	ADC12MCTL14 0x08E 
#define	ADC12MCTL15 0x08F 

#endif

