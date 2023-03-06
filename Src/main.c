#include <stm32f446xx.h>

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"
#include "usart.h"
#include "sim.h"
#include "screen.h"

uint8_t button = 0;

struct USART_HANDLER usart2 = { .buffer = {'*'}, .size = 0, .timeOut = 0, .command = {'*'} };
struct USART_HANDLER *SCREEN = &usart2;

struct USART_HANDLER uart4 = { .buffer = {'*'}, .size = 0, .timeOut = 0, .command = {'*'} };
struct USART_HANDLER *SIM = &uart4;

extern uint8_t ATE0[];
extern uint8_t CMGF[];
extern uint8_t CMGD[];

void waitForButtonPressed(){
	button = 0;					// Reset button flag
	GPIOA->ODR |= GPIO_ODR_OD5;	// Turn on  the user LED for debugging
	while(!button);
}

int main(void){
//	STM32 initialization
	PA5_Init();
	PC13_Init();
	TIM2_Init();
	TIM4_Init();
	USART2_Init();
	UART4_Init();

//	Waits 30 seconds for modules initialization
	setTimeout(TIM4, 30000);
	launchTimer(TIM4);
	waitForTimeOut(SIM);

//	Timers time out delay configuration
	setTimeout(TIM4, 1000);
	setTimeout(TIM2, 5000);

//	SIM module configuration
	SIM_sendCommand(ATE0);
	SIM_sendCommand(CMGF);
	SIM_sendCommand(CMGD);

	while(1){
		waitForButtonPressed();		// The SCREEN module wants to confirm a code

		launchTimer(TIM2);
		waitForTimeOut(SCREEN);
		SCREEN_commandInterpreter();

		waitForButtonPressed();		// The SIM module has received an SMS

		SIM_readSMS();
		SIM_commandInterpreter();
	}

	return 0;
}
