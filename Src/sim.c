#include <stm32f446xx.h>
#include <string.h>
#include "main.h"
#include "sim.h"
#include "timer.h"
#include "usart.h"
#include "screen.h"

extern struct USART_Handler *SIM;

uint8_t AT_ECHO_MODE_OFF[]	= "ATE0\r\0";						// Disable echo mode for SIM module

uint8_t AT_TEXT_MODE_ON[]	= "AT+CMGF=1\r\0";					// Enable text mode for SIM module

uint8_t AT_GET_IDENTITY[]	= "AT+CGSN\r\0";					// Obtain the IMEI of the SIM module

uint8_t AT_SMS_ERASE_ALL[]	= "AT+CMGD=,4\r\0";					// Erase all SMS stored in SIM module
uint8_t AT_SMS_READ_FIRST[]	= "AT+CMGR=0,0\r\0";				// Read first SMS Message
uint8_t AT_SMS_WRITE[] 		= "AT+CMGS=\"+33784961263\"\r\0";	// Write an SMS to a phone number in text mode
uint8_t AT_SMS_END_TEXT[]	= "\x1A\0";							// End of text character for SMS

uint8_t CODE_GOOD[]			= "code oui";						// Response if the user code is good
uint8_t CODE_WRONG[]		= "code non";						// Response if the user code is wrong

void SIM_sendCommand(uint8_t *ATcommand){
/*
 * Send AT command to SIM module over UART 4
 */
	launchTimer(TIM4);					// Limit listening time to the SIM module
	USART_TX(UART4, ATcommand);			// Send command to SIM module
	waitForTimeOut(SIM);				// Wait for the SIM module's response to be entirely received
}

void SIM_configure(void){
/*
 * Configure SIM module
 */
	SIM_sendCommand(AT_ECHO_MODE_OFF);	// Disable echo mode for SIM module
	SIM_sendCommand(AT_TEXT_MODE_ON);	// Enable text mode for SIM module
	SIM_sendCommand(AT_SMS_ERASE_ALL);	// Erase all SMS stored in SIM module
}

void SIM_setIdentity(uint8_t *IDENTITY){
/*
 * Generate serial number
 */
	SIM_sendCommand(AT_GET_IDENTITY);				// Ask the IMEI of the SIM module
	SIM_extractCommand(LINE_1);						// Extract IMEI from SIM module response
	strcpy((char*)IDENTITY, (char*)SIM->command);	// Save IMEi into IDENTITY variable
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

	for(; (*buff != '\0'); (++buff, ++comm)){
		if(*buff != '\r')				// While buffer end of line isn't reached
			*comm = *buff;				// Copy buffer content to command
		else{
			*comm = '\0';				// Set end of string in the command
			break;						// Exit for loop
		}
	}
}

void SIM_sendSMS(uint8_t *AT_SMS_Text){
/*
 * Send SMS with the SIM module to a predefined number
 */
	SIM_sendCommand(AT_SMS_ERASE_ALL);	// Erase all messages stored in SIM module
	SIM_sendCommand(AT_SMS_WRITE);		// Specify the phone number to SIM module
	SIM_sendCommand(AT_SMS_Text);		// Send SMS text data to SIM module
	setTimeout(TIM4, TIMEOUT_EXTENDED);	// Allow longer response time for SIM module after sending the SMS
	SIM_sendCommand(AT_SMS_END_TEXT);	// Transmit end of text character to SIM module
	setTimeout(TIM4, TIMEOUT_DEFAULT);	// Set time out delay to default
}

void SIM_readSMS(void){
/*
 * Read the first SMS from the SIM module and delete it
 */
	SIM_sendCommand(AT_SMS_READ_FIRST);	// Read SMS Message
	SIM_extractCommand(LINE_4);			// Extract command from SMS
	SIM_sendCommand(AT_SMS_ERASE_ALL);	// Erase all messages stored in SIM
}

void SIM_executeCommand(void){
/*
 * Verify and execute command from SIM command buffer
 */
	strcpy((char*)SIM->buffer, "");	// Empty the buffer

	switch(SIM->command[0]){												// Depending on the command type
		case COMMAND_CODE_VALIDITY:											// User code validity response
			if(strcmp((char*)SIM->command, (char*)CODE_GOOD) == 0){			// If the code is valid
				createMessage(SIM,(uint8_t *)GTP_depose, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
				SCREEN_sendCommand(SIM->buffer);							// Display depos page on SCREEN
				return;
			}

			else if(strcmp((char*)SIM->command, (char*)CODE_WRONG) == 0){	// If the code is not valid
				createMessage(SIM, (uint8_t *)GTP_saisie, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
				SCREEN_sendCommand(SIM->buffer);							// Display saisie page on SCREEN
				return;														// Exit function
			}
		break;

		case COMMAND_GO_TO_PAGE:											// Change SCREEN page
			if(strcmp((char*)SIM->command, (char*)GTP_accueil) == 0){		// Command to load main page
				createMessage(SIM, (uint8_t *)GTP_accueil, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
				SCREEN_sendCommand(SIM->buffer);							// Display accueil page on SCREEN
				return;														// Exit function
			}

			else if(strcmp((char*)SIM->command, (char*)GTP_adtp) == 0){		// Command to load settings page
				createMessage(SIM, (uint8_t *)GTP_adtp, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
				SCREEN_sendCommand(SIM->buffer);							// Display adtp page on SCREEN
				return;														// Exit function
			}
		break;
	}

//	If the command is unknown
	createMessage(SIM, (uint8_t *)GTP_erreur, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
	SCREEN_sendCommand(SIM->buffer);										// Display erreur page on SCREEN
}
