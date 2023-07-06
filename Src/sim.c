#include <stm32f446xx.h>
#include <string.h>
#include "main.h"
#include "sim.h"
#include "timer.h"
#include "usart.h"
#include "screen.h"

extern struct USART_Handler *SIM;
extern struct USART_Handler *SCREEN;

extern uint8_t *USART_messageBuffer;

void SIM_sendCommand(uint8_t *ATcommand){
/*
 * Send AT command to SIM module over UART 4
 */
	uint8_t tryCount = 0;											// Counter of transmission attempt made
	uint8_t sucess = 0;												// Flag indicating whether the last transmission was successful or not

	while((tryCount < NB_TRY) && (sucess == 0)){
		launchTimer(TIM4);											// Limit listening time to the SIM module
		USART_TX(UART4, ATcommand);									// Send command to SIM module
		waitForTimeOut(SIM);										// Wait for the SIM module's response to be entirely received

		sucess = errorDetection();									// Determine whether the transmission was successful
		tryCount++;													// Another transmission attempt has just been made
	}

	if(sucess == 0){
		createMessage((uint8_t *)GTP_adtp, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display erreur page on SCREEN
	}
}

uint8_t errorDetection(void){
/*
 * Return 1 if SIM returned a ERROR or return 0 if no error is detected
 */
	SIM_extractCommand(SIM->response, LINE_1);						// Extract SIM module response from last command
	if(strcmp((char*)SIM->response, (char*)SIM_ERROR) == 0){
		return 0;													// An error occurred
	}
	else{
		return 1;													// No error detected
	}
}

void SIM_configure(void){
/*
 * Configure echo and text mode then erase all SMS in SIM
 */
	SIM_sendCommand((uint8_t*)AT_ECHO_MODE_OFF);					// Disable echo mode for SIM module
	SIM_sendCommand((uint8_t*)AT_TEXT_MODE_ON);						// Enable text mode for SIM module
	SIM_sendCommand((uint8_t*)AT_SMS_ERASE_ALL);					// Erase all SMS stored in SIM module
}

void SIM_setIdentity(uint8_t *IDENTITY){
/*
 * Generate a unique serial number
 */
	SIM_sendCommand((uint8_t*)AT_GET_IDENTITY);						// Ask the IMEI of the SIM module
	SIM_extractCommand(SIM->command, LINE_1);						// Extract IMEI from SIM module response
	strcpy((char*)IDENTITY, (char*)SIM->command);					// Save IMEi into IDENTITY variable
}

uint8_t* extractLine(uint8_t commandLine){
	uint8_t currLine = 0;											// Current line in buffer
	uint8_t *buff = SIM->buffer;									// Pointer to current byte of buffer

	while((currLine != commandLine) && (*buff != '\0')){			// Check if the current line is the one containing the command
		if(*buff == '\n'){											// When the current byte of buffer is a line feed
			currLine++;												// A new line has been reached
		}
		++buff;														// Select next character in buffer
	}

	return buff;													// Select the first byte representing the command in buffer
}

void copyLine(uint8_t *dest, uint8_t *source){
	uint8_t *buff = source;
	uint8_t *comm = dest;

	while((*buff != '\r') && (*buff != '\0')){						// While buffer end of line isn't reached
		*comm = *buff;												// Copy buffer content to command
		++buff;														// Select next character in buffer
		++comm;														// Select next character in buffer
	}
	*comm = '\0';													// Set end of string in the command
}

void SIM_extractCommand(uint8_t *dest, uint8_t commandLine){
/*
 * Extract SIM module command from SIM module buffer
 */
	uint8_t *buff = extractLine(commandLine);						// Create pointer to first character in desired line
	copyLine(dest, buff);											// Copy line into dest string
}

void SIM_sendSMS(uint8_t *AT_SMS_Text){
/*
 * Send SMS with the SIM module to a predefined number
 */
	SIM_sendCommand((uint8_t*)AT_SMS_ERASE_ALL);					// Erase all messages stored in SIM module
	SIM_sendCommand((uint8_t*)AT_SMS_WRITE);						// Specify the phone number to SIM module
	SIM_sendCommand((uint8_t*)AT_SMS_Text);							// Send SMS text data to SIM module
	setTimeout(TIM4, TIMEOUT_EXTENDED);								// Allow longer response time for SIM module
	SIM_sendCommand((uint8_t*)AT_SMS_END_TEXT);						// Transmit end of text character to SIM module
	setTimeout(TIM4, TIMEOUT_DEFAULT);								// Set time out delay to default
}

void SIM_readSMS(void){
/*
 * Read the first SMS from the SIM module and delete it
 */
	SIM_sendCommand((uint8_t*)AT_SMS_READ_FIRST);					// Read SMS Message
	SIM_extractCommand(SIM->command, LINE_4);						// Extract command from SMS
	SIM_sendCommand((uint8_t*)AT_SMS_ERASE_ALL);					// Erase all messages stored in SIM
}

void SIM_executeCommand(void){
/*
 * Verify and execute command from SIM command buffer
 */
	uint8_t commandKnown = 0;										// Indicates if the command is part of command set

	strcpy((char*)SIM->buffer, "");									// Empty the buffer

	if(strncmp((char*)SIM->command, (char*)COMMAND_CODE_VALIDITY, 4) == 0){
		commandKnown = codeValidity();								// Check if the command is know
	}

	if(strncmp((char*)SIM->command, (char*)COMMAND_GO_TO_PAGE, 4) == 0){
		commandKnown = goToPage();									// Check if the command is known
	}

	if(!commandKnown){												//	If the command is unknown
		createMessage((uint8_t *)GTP_erreur, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display erreur page on SCREEN
	}
}

uint8_t  codeValidity(void){
/*
 * Command containing user code validity
 */
	if(strcmp((char*)SIM->command, (char*)CODE_GOOD) == 0){			// If the code is valid
		createMessage((uint8_t *)GTP_depose, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display depos page on SCREEN
		return 1;													// Return valid code value
	}

	else if(strcmp((char*)SIM->command, (char*)CODE_WRONG) == 0){	// If the code is not valid
		createMessage((uint8_t *)GTP_erreur_saisie, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display saisie page on SCREEN
		return 1;													// Return valid code value
	}

	return 0;														// Return invalid code value
}

uint8_t goToPage(void){
/*
 * Command to change page displayed on SCREEN module
 */
	if(strcmp((char*)SIM->command, (char*)GTP_accueil) == 0){		// Command to load main page
		createMessage((uint8_t *)GTP_accueil, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display accueil page on SCREEN
		return 1;													// Return valid page value
	}

	else if(strcmp((char*)SIM->command, (char*)GTP_termine) == 0){	// Command to load settings page
		createMessage((uint8_t *)GTP_termine, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
		SCREEN_sendCommand(USART_messageBuffer);					// Display adtp page on SCREEN
		return 1;													// Return valid page value
	}

	return 0;														// Return invalid page value
}
