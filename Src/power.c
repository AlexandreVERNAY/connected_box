#include <stm32f446xx.h>
#include "main.h"
#include "power.h"

void enterStandbyMode(void){
/*
 * Configure low power mode to standby mode
 */
	PWR->CR |= PWR_CR_LPDS;		// Select Deep Sleep
	PWR->CR |= PWR_CR_PDDS;		// Select Standby mode
	__WFI();					// Wait For Interrupt
}
