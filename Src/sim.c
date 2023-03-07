#include <stm32f446xx.h>
#include "main.h"
#include "sim.h"
#include "timer.h"
#include "usart.h"
#include "screen.h"

extern struct USART_HANDLER *SIM;

uint8_t ATE0[]	= "ATE0\r\0";						// Disable echo mode for SIM module

uint8_t CMGF[]	= "AT+CMGF=1\r\0";					// Enable text mode for SIM module
uint8_t CMGD[]	= "AT+CMGD=,4\r\0";					// Erase all messages stored in SIM

uint8_t CMGR[] 	= "AT+CMGR=0,0\r\0";				// Read SMS Message

uint8_t CMGS[] 	= "AT+CMGS=\"+33784961263\"\r\0";	// Write an SMS to a mobile phone number in text mode
uint8_t EoTSMS[]= { '\x1A', '\0' };					// End of text character for SMS

void SIM_configure(void){
/*
 * Configure SIM module
 */
	SIM_sendCommand(ATE0);
	SIM_sendCommand(CMGF);
	SIM_sendCommand(CMGD);
}

void SIM_sendCommand(uint8_t *ATcommand){
/*
 * Send AT command to SIM module over UART 4
 */
	launchTimer(TIM4);			// Limit listening time to the SIM module
	USART_TX(UART4, ATcommand);	// Send command to SIM module
	waitForTimeOut(SIM);		// Wait for SIM response to be entirely received
}

void SIM_sendSMS(uint8_t *SMS){
/*
 * Send SMS with the SIM module to a predefined number
 */
	SIM_sendCommand(CMGD);		// Erase all messages stored in SIM module
	SIM_sendCommand(CMGS);		// Specify the phone number to SIM module
	SIM_sendCommand(SMS);		// Send SMS data to SIM module
	setTimeout(TIM4, 5000);		// Allow longer response time for SIM module after sending the SMS
	SIM_sendCommand(EoTSMS);	// Transmit end of text character to SIM module
	setTimeout(TIM4, 1000);		// Reset time out delay to default
}

void SIM_readSMS(void){
/*
 * Read the first SMS from the SIM module and delete it
 */
	SIM_sendCommand(CMGR);		// Read SMS Message
	SIM_extractCommand(LINE_4);	// Extract command from SMS
	SIM_sendCommand(CMGD);		// Erase all messages stored in SIM
	SIM_commandInterpreter();	// Execute action from SMS
}

void SIM_extractCommand(uint8_t commandLine){
/*
 * Extract SIM module command from SIM module buffer
 */
	uint8_t currLine = 0;				// Current line in buffer

	uint8_t *buff = SIM->buffer;		// Pointer to current byte of buffer
	uint8_t *comm = SIM->command;		// Pointer to current byte of command

	for(; *buff != '\0'; ++buff){		// Limit action to buffer size
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

void SIM_commandInterpreter(void){
/*
 * Verify and execute command from SIM command buffer
 */
	uint8_t page0[] = "page 0\xFF\xFF\xFF";
	uint8_t page2[] = "page 2\xFF\xFF\xFF";

	switch(SIM->command[0]){			// Depending on the command type (first byte in command)
		case 0x4E:						// First letter is 'N' as in NON
			SCREEN_sendCommand(page0);	// Send command to SCREEN to display page 0
		break;

		case 0x4F:						// First letter is 'O' as in OUI
			SCREEN_sendCommand(page2);	// Send command to SCREEN to display page 2
		break;
	}
}
