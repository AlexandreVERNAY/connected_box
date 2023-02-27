#include <stm32f446xx.h>
#include <stdlib.h>

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"
#include "usart.h"
#include "sim.h"

uint8_t *buffer	= NULL;								// UART reception buffer
uint8_t size	= 0;								// buffer's size
uint8_t timeOut	= 0;								// Timer 2 time out

uint8_t *command= NULL;								// Module command string

uint8_t SMS[]	= "SMS text\0";						// SMS text example

uint8_t ATE0[]	= "ATE0\r\0";						// Disable echo mode in SIM module

uint8_t CMGF[]	= "AT+CMGF=1\r\0";					// Enable text mode for SIM module
uint8_t CMGD[]	= "AT+CMGD=,4\r\0";					// Erase all messages stored in SIM

uint8_t CMGR[] 	= "AT+CMGR=0,0\r\0";				// Read SMS Message

uint8_t CMGS[] 	= "AT+CMGS=\"+33784961263\"\r\0";	// Write SMS to GSM number in text Mode
uint8_t EoLSMS[]= { '\x1A', '\0' };					// End of line SMS character

int main(void){
	PA5_Init();
	PC13_Init();
	TIM2_Init();

	USART2_Init();
	UART4_Init();

	buffer	= malloc(64*sizeof(uint8_t));
	command	= malloc(16*sizeof(uint8_t));

	setTimeout(30000);
	launchTimer();
	waitForTimeOut();

	setTimeout(500);
	SIM_sendCommand(ATE0);
	extractCommand(LINE_1);

	SIM_sendCommand(CMGF);
	extractCommand(LINE_1);

	SIM_sendCommand(CMGR);
	extractCommand(LINE_2);

	SIM_sendCommand(CMGS);
	SIM_sendCommand(SMS);
	setTimeout(4000);
	SIM_sendCommand(EoLSMS);
	extractCommand(LINE_1);

	return 0;
}
