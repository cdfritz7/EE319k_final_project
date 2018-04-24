#include <stdint.h>
#include "tm4c123gh6pm.h"

extern uint32_t player1button;
extern uint32_t player2button;


void buttons_Init(void);

void GPIOPortB_Handler(void);