// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/28/2018 
// Student names: Connor Fritz and Tatiana Larina
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "tm4c123gh6pm.h"


// ADC0 initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
	volatile uint32_t delay;
	SYSCTL_RCGCADC_R |= 0x00000001; //Activate ADC0
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; //Activate clock for Port E
	delay = SYSCTL_RCGCGPIO_R;	//allow time for clock to stabilize
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTE_DIR_R &= ~0x30; //make PE4 PE5 input
	GPIO_PORTE_AFSEL_R |= 0x30; //enable alternate function on PE4 PE5
	GPIO_PORTE_DEN_R &= ~0x30;
	
	GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
	GPIO_PORTE_AMSEL_R |= 0x30; 	//enable analog functionality
	ADC0_PC_R &= ~0xF;							//clear max sample rate field
	ADC0_PC_R = 0x1;								//sets the max sampling rate to 125kHz, (0x3 = 250k, 0x5 = 500k, 0x7 = 1mil)
	ADC0_SSPRI_R = 0x3210; 					//sequencer 3 is lowest priority
	ADC0_ACTSS_R &= ~0X0004;				//disables sequencer 2 while we configure it
	ADC0_EMUX_R &= ~0X0F00;					//(for sequencer 2) to specify software start triggering
	ADC0_SSMUX2_R = 0x0089;     		//set channels for ss2
																	//write channels number 5 to bits 0-3, we sample channel 5
	ADC0_SSCTL2_R = 0x0060;					//specifies mode of ADC sample, analog voltage on ADC pin (TS0 = 0), no differential sampling (D0 = 0)
																	//busy wait synchronization (IE0 = 1), only one sample (END0 = 1, means this sample is end of sequence)
	ADC0_IM_R &= ~0x0004;						//disable ss2 interrupts (we don't want them with software start)
	ADC0_ACTSS_R |= 0x0004;					//enable sequencer 2
}


//------------ADC_In0------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 2 12-bit result of ADC conversion
// measures from PD2, analog channel 5
// Cannnot use PD3 for sequencer
//data[0] is ADC8 (PE5) 0 to 4095
//data[1] is ADC9 (PE4) 0 to 4095
void ADC_In0(uint32_t data[2]){  
	ADC0_PSSI_R = 0x0004;		//initiate ss2
	while((ADC0_RIS_R&0x04)==0){};	//wait for conversion to be done
	data[1] = ADC0_SSFIFO2_R&0xFFF; //read first result
	data[0] = ADC0_SSFIFO2_R&0xFFF;	//read second result
	ADC0_ISC_R = 0x0004;						//ack
}

