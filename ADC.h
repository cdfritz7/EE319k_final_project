// put prototypes for public functions, explain what it does
// Connor Fritz and Tatiana Larina
//4/22/18

#include <stdint.h>
#include "tm4c123gh6pm.h"


// ADC0 initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void);



//------------ADC_In0------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
// Cannnot use PD3 for sequencer
uint32_t ADC_In0(uint32_t data[2]);
