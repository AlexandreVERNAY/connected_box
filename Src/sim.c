#include <stm32f446xx.h>
#include "main.h"
#include "sim.h"
#include "timer.h"
#include "usart.h"

extern uint8_t *buffer;

extern uint8_t *command;

void SIM_sendCommand(uint8_t *ATcommand){
/*
 * Send AT command to SIM module over UART 4
 */
	launchTimer();						// Limit the response time of the SIM module
	USART_TX(UART4, ATcommand);			// Send Command to SIM module
	waitForTimeOut();					// Wait for message to be entirely received
}

void extractCommand(uint8_t commandLine){
/*
 * Extract SIM module command from UART reception buffer
 */
	uint8_t currLine = 0;				// Current line in buffer

	uint8_t *buff = buffer;				// Pointer to current byte of buffer
	uint8_t *comm = command;			// Pointer to current byte of command

	for(; *buff != '\0'; ++buff){		// Limits action to buffer size
		if(*buff == '\n'){				// When the current byte of buffer is a line feed
			currLine++;					// A new line has been reached
			if(currLine == commandLine){// Check if the current line is the one containing the command
				++buff;					// Select the first byte representing the command in buffer
				break;					// Exit for loop
			}
		}
	}

	for(; *buff != '\0'; ++buff, ++comm){
		if(*buff != '\r')				// While buffer end of line isn't reached
			*comm = *buff;				// Copy buffer content to command
		else{
			*comm = '\0';				// Set end of string in the command
			break;						// Exit for loop
		}
	}
}
