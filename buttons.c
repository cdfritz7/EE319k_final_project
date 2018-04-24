

#include <stdint.h>
#include "tm4c123gh6pm.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t EdgeInterruptFlag;
uint32_t player1button;
uint32_t player2button;
// global variable visible in Watch window of debugger
// increments at least once per button press
void buttons_Init(void){  
  player1button = 0;
  player2button = 0;	
	EdgeInterruptFlag = 0;
  SYSCTL_RCGCGPIO_R |= 0x00000002; // (a) activate clock for port B
  //FallingEdges = 0;             // (b) initialize counter
  GPIO_PORTB_DIR_R &= ~0x30;    // (c) make PB4 PB5 in (built-in button)
  GPIO_PORTB_AFSEL_R &= ~0x30;  //     disable alt funct on PB4 PB5
  GPIO_PORTB_DEN_R |= 0x30;     //     enable digital I/O on PB4 PB5 
  //GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PB4 PB5 as GPIO
  GPIO_PORTB_AMSEL_R = 0;       //     disable analog functionality on Pb
  GPIO_PORTB_PDR_R |= 0x30;     //     enable weak pull-up on PB4 PB5
  GPIO_PORTB_IS_R &= ~0x30;     // (d) PB4 PB5 is edge-sensitive
  GPIO_PORTB_IBE_R &= ~0x30;    //     PB4 PB5 is not both edges
  GPIO_PORTB_IEV_R |= 0x30;    //     PB4 PB5 rising edge event
  GPIO_PORTB_ICR_R = 0x30;      // (e) clear flag4
  GPIO_PORTB_IM_R |= 0x30;      // (f) arm interrupt on PB4 PB5 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x0000A000; // (g) priority 5
  NVIC_EN0_R = 0x02;      // (h) enable interrupt 30 in NVIC
  EnableInterrupts();           // (i) Clears the I bit
}

void GPIOPortB_Handler(void){
	if((GPIO_PORTB_RIS_R & 0x10) == 0x10){	//checks if player1 button pressed
		player1button = 0x01;
		GPIO_PORTB_ICR_R = 0x10;      // acknowledge flag4
	}
	if((GPIO_PORTB_RIS_R & 0x20) == 0x20){	//checks if player2 button pressed
		player2button = 0x01;
		GPIO_PORTB_ICR_R = 0x20;      // acknowledge flag4
	}
  
  
}
