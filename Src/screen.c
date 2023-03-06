#include <stm32f446xx.h>
#include "main.h"
#include "screen.h"
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
 * Indicates if the SCREEN buffer contains a known command
 */
	switch(SCREEN->buffer[0]){
		case 0x17:							// Verify the code entered by the user on the screen
			extractCode();					// Extract code from buffer to command
			SIM_sendSMS(SCREEN->command);	// Send SCREEN command via SIM module
		break;
	}
}

void extractCode(void){
/*
 * Extract command from SCREEN module buffer
 */
	uint8_t *buff = SCREEN->buffer;			// Pointer to first byte in buffer
	uint8_t *comm = SCREEN->command;		// Pointer to first byte in command

	++buff;									// Ignores first byte containing command type

	for(; *buff != '\0'; ++buff, ++comm){	// While buffer end of string isn't reached
		*comm = (*buff);					// Copy buffer content to command
	}
	*comm = '\0';							// Set end of string character
}
