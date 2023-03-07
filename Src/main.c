#include <stm32f446xx.h>

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"
#include "usart.h"
#include "sim.h"
#include "screen.h"
#include "power.h"

struct USART_HANDLER usart2 = { .buffer[0] = '\0', .size = 0, .timeOut = 0, .command[0] = '\0' };
struct USART_HANDLER *SCREEN = &usart2;

struct USART_HANDLER uart4 = { .buffer[0] = '\0', .size = 0, .timeOut = 0, .command[0] = '\0' };
struct USART_HANDLER *SIM = &uart4;

int main(void){
//	STM32 initialization
	PA5_Init();
	PC13_Init();
	TIM2_Init();
	TIM4_Init();
	USART2_Init();
	UART4_Init();

//	Wait 30 seconds for modules to start
	setTimeout(TIM4, 30000);
	launchTimer(TIM4);
	waitForTimeOut(SIM);

//	Timers delay configuration
	setTimeout(TIM4, 1000);
	setTimeout(TIM2, 5000);

//	SIM module configuration
	SIM_configure();

	while(1){
		enterStandbyMode();					// Interrupt will exit Standby mode

		if(SCREEN->size){					// If SCREEN module send a command
			SCREEN_commandInterpreter();
		}

		if(SIM->size){						// If SIM module send a command
			SIM_readSMS();
		}
	}

	return 0;
}
