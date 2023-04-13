#include <stm32f446xx.h>
#include "main.h"
#include "power.h"

void enterNormalSleep(void){
/*
 * Enter normal sleep
 */
	__WFI();	// Wait For Interrupt
}
