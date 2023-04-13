#include <stm32f446xx.h>
#include <string.h>

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"
#include "usart.h"
#include "sim.h"
#include "screen.h"
#include "power.h"

struct USART_Handler usart1		= { .buffer[0] = '\0', .size = 0, .timeOut = 0, .command[0] = '\0' };
struct USART_Handler *SCREEN	= &usart1;

struct USART_Handler uart4		= { .buffer[0] = '\0', .size = 0, .timeOut = 0, .command[0] = '\0' };
struct USART_Handler *SIM		= &uart4;

uint8_t SIM_Init_OK[]	= "Initialisation OK\r\n";	// Initialization was successful
uint8_t IDENTITY[]		= "XXXXXXXXXXXXXXX";		// Unique serial number

int main(void){
//	GPIOs initialization
	PA5_Init();			// Configure LED
	PC13_Init();		// Configure BUTTON

//	Timers initialization
	TIM2_Init();		// Configure Timer 2 used for USART1 time out
	TIM4_Init();		// Configure Timer 4 used for UART4  time out

//	USARTs initialization
	USART1_Init();		// Configure USART1 used for communication with the SCREEN module
	UART4_Init();		// Configure UART4  used for communication with the SIM module

//	Wait 30 seconds for modules to start
	setTimeout(TIM4, TIMEOUT_INIT);			// Set wake up delay
	launchTimer(TIM4);						// Launch timer
	waitForTimeOut(SIM);					// Wait for modules to fully initialize

//	Timers delay configuration
	setTimeout(TIM2, TIMEOUT_EXTENDED);		// Set the time allowed for the SCREEN module to respond
	setTimeout(TIM4, TIMEOUT_DEFAULT);		// Set the time allowed for the SIM module to respond

//	SIM module configuration
	SIM_configure();						// Configure echo and text mode then erase all SMS in SIM

//	Generate serial number
	SIM_setIdentity(IDENTITY);				// Create a unique serial number

//	Create and send an SMS notifying the end of the initialization phase
	createMessage(SCREEN, SIM_Init_OK, IDENTITY, (uint8_t *)"\0");
	SIM_sendSMS(SCREEN->buffer);			// Send SMS identifying the box

//	Creates Go To Page accueil command for SCREEN
	createMessage(SCREEN, (uint8_t *)GTP_accueil, (uint8_t *)GTP_END_COMMAND, (uint8_t *)"\0");
	SCREEN_sendCommand(SCREEN->buffer);		//	Display accueil page in SCREEN

	while(1){
		enterNormalSleep();	// Enter sleep mode

//		Any interruption cancel sleep mode and continue the program
//		Before re-entering sleep mode, it executes modules commands

		if(SCREEN->size){						// If SCREEN module has send a command
			SCREEN_executeCommand();			// Execute SCREEN
		}

		if(SIM->size){							// If SIM module has send a command
			SIM_readSMS();						// Read the received SMS
			SIM_executeCommand();				// Execute SIM command
		}
	}
}
