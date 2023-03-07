#include <stm32f446xx.h>
#include "main.h"
#include "screen.h"
#include "timer.h"
#include "usart.h"
#include "sim.h"

extern struct USART_HANDLER *SCREEN;

void SCREEN_sendCommand(uint8_t *command){
/*
 * Send command to SCREEN module over USART 2
 */
	USART_TX(USART2, command);
}

void SCREEN_commandInterpreter(void){
/*
 * Verify and execute the command from the SCREEN module
 */
	launchTimer(TIM2);						// Limit listening time to the SCREEN module
	waitForTimeOut(SCREEN);					// Wait for the SCREEN module's response to be entirely received

	switch(SCREEN->buffer[0]){				// Depending on the command type (first byte in command)
		case 0x17:							// Verify the code entered by the user on the screen
			SCREEN_extractCode();			// Extract code from buffer to command
			SIM_sendSMS(SCREEN->command);	// Send the code via SIM module
		break;
	}
}

void SCREEN_extractCode(void){
/*
 * Extract user code from SCREEN module buffer
 */
	uint8_t *buff = SCREEN->buffer;			// Pointer to first byte in buffer
	uint8_t *comm = SCREEN->command;		// Pointer to first byte in command

	++buff;									// Ignore the first byte containing command type

	for(; *buff != '\0'; ++buff, ++comm){	// While buffer end of string isn't reached
		*comm = *buff;						// Copy buffer content to command
	}
	*comm = '\0';							// Set end of string character
}
