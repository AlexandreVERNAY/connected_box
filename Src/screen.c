#include <stm32f446xx.h>
#include <string.h>
#include "main.h"
#include "screen.h"
#include "timer.h"
#include "usart.h"
#include "sim.h"

extern struct USART_Handler *SCREEN;

uint8_t CODE_CONFIRM_CODE[]		= "Confirmez vous ce code ?\r\n";
uint8_t CODE_CONFIRM_CHOICE[]	= "\r\nEnvoyez code suivi de oui pour valider, non pour refuser.";

void SCREEN_sendCommand(uint8_t *command){
/*
 * Send command to SCREEN module over USART1
 */
	USART_TX(USART1, command);
}

void SCREEN_executeCommand(void){//uint8_t *currPage
/*
 * Verify and execute the command from the SCREEN module
 */
	launchTimer(TIM2);		// Limit listening time to the SCREEN module
	waitForTimeOut(SCREEN);	// Wait for the SCREEN module's response to be entirely received
	SCREEN_extractCode();	// Extract code from buffer to command

	switch(SCREEN->buffer[0]){					// Depending on the command type (first byte in command)
		case COMMAND_VERIFY_CODE:				// Verify the code entered by the user on the screen
			createMessage(SCREEN, CODE_CONFIRM_CODE, SCREEN->command, CODE_CONFIRM_CHOICE);
			SIM_sendSMS(SCREEN->buffer);		// Send the code via SIM module
		break;
	}
}

void SCREEN_extractCode(void){
/*
 * Extract user code from SCREEN module buffer
 */
	uint8_t *buff = SCREEN->buffer;				// Pointer to first byte in buffer
	uint8_t *comm = SCREEN->command;			// Pointer to first byte in command

	++buff;										// Ignore the first byte containing command type

	for(; (*buff != '\0'); (++buff, ++comm)){	// While buffer end of string isn't reached
		*comm = *buff;							// Copy buffer content to command
	}
	*comm = '\0';								// Set end of string character
}
