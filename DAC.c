// put implementations for functions, explain how it works
// Connor Fritz and Tatiana Larina 4/21/18

#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
//Initializes PB0-3
void DAC_Init(void){
	volatile unsigned int delay;
	SYSCTL_RCGCGPIO_R |= 0x02;					  //Start clocks for Port B 
	delay = 4;												//wait
	delay --;
	delay --;
	delay --;
	delay --;
	delay --;
	//PB 0-5 are DAC outputs
	GPIO_PORTB_AMSEL_R &= ~0x0F; 			//Disable analog on PB0-3
	GPIO_PORTB_PCTL_R &= ~0x000FFFFF; //enable regular GPIO
	GPIO_PORTB_DIR_R |= 0x0F;  				//outputs on PB0-3
	GPIO_PORTB_AFSEL_R &= ~0x0F; 			//regular function on PB0-3
	GPIO_PORTB_DEN_R |= 0x0F;    			//Enable digital on PB0-3
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
// Initializes PB0-3
void DAC_Out(uint32_t data){
	
	GPIO_PORTB_DATA_R &= ~0x0F;
	GPIO_PORTB_DATA_R = (data&0x0F);
	
}
