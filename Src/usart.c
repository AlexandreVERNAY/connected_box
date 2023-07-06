#include <stm32f446xx.h>
#include <string.h>
#include "main.h"
#include "usart.h"

extern uint8_t *USART_messageBuffer;

void USART1_Init(void){
/*
 * Configure USART1 using PA9(TX) and PA10(RX)
 */
//	[6.3.14]RCC APB2 peripheral clock enable register
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;					// Enable USART1 Clock

//	[7.4.1]GPIO port mode register
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER9_Pos);	// Set Alternate Function mode for PA9
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER10_Pos);// Set Alternate Function mode for PA10

//	[7.4.10]GPIO alternate function high register
	GPIOA->AFR[1] |= (GPIO_AFR7 << GPIO_AFRH_AFSEL9_Pos);	// Set Port 7 for PA9  Alternate Function mode
	GPIOA->AFR[1] |= (GPIO_AFR7 << GPIO_AFRH_AFSEL10_Pos);	// Set Port 7 for PA10 Alternate Function mode

//	[25.6.3]Baud rate register
	USART1->BRR  = 139;										// Set Baud rate (139 = 16 000 000 Hz/115200 bit/s)

//	[25.6.4]Control register 1
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;				// Enable Transmitter and Receiver for USART1
	USART1->CR1 |= USART_CR1_UE;							// Enable USART1
	USART1->CR1 |= USART_CR1_RXNEIE;						// Enable Interrupt for data reception

//	Cortex M4 [4.3.2]Interrupt set-enable register
	NVIC_EnableIRQ(USART1_IRQn);							// Enable USART1 global Interrupt
}

void UART4_Init(void){
/*
 * Configure UART4 using PA0(TX) and PA1(RX)
 */
//	[6.3.13]RCC APB1 peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;					// Enable UART4 Clock

//	[7.4.1]Port mode register
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER0_Pos);	// Set Alternate Function mode for PA0
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER1_Pos);	// Set Alternate Function mode for PA1

//	[7.4.9]GPIO alternate function low register
	GPIOA->AFR[0] |= (GPIO_AFR8 << GPIO_AFRL_AFSEL0_Pos);	// Set Port 8 for PA0 Alternate Function mode
	GPIOA->AFR[0] |= (GPIO_AFR8 << GPIO_AFRL_AFSEL1_Pos);	// Set Port 8 for PA1 Alternate Function mode

//	[25.6.3]Baud rate register
	UART4->BRR  = 139;										// Set Baud rate (139 = 16 000 000 Hz/115200 bit/s)

//	[25.6.4]Control register 1
	UART4->CR1 |= USART_CR1_TE | USART_CR1_RE;				// Enable Transmitter and Receiver for UART4
	UART4->CR1 |= USART_CR1_UE;								// Enable UART4
	UART4->CR1 |= USART_CR1_RXNEIE;							// Enable Interrupt for data reception

//	Cortex M4 [4.3.2]Interrupt set-enable register
	NVIC_EnableIRQ(UART4_IRQn);								// Enable UART4 global Interrupt
}

void USART_TX(USART_TypeDef *USART, uint8_t *data){
/*
 * Transmit data over selected USART
 */
	for(; (*data != '\0'); ++data){							// For every byte of data
		USART->DR = *data;									// Send data over USART
		while((USART->SR & USART_SR_TXE)==0);				// Wait for transmission to complete
	}
}

void waitForTimeOut(struct USART_Handler *usart){
/*
 * Wait for Timer time out
 */
	while(!usart->timeOut);									// Wait for time out to be reached

	usart->buffer[usart->size] = '\0';						// Set end of string in the buffer
	usart->timeOut = 0;										// Reset flag time out
	usart->size = 0;										// Reset buffer size
}

void createMessage(uint8_t *headMessage, uint8_t *message, uint8_t *tailMessage){
/*
 * Store message to send in USART Handler buffer
 */
	strcpy((char*)USART_messageBuffer, (char*)headMessage);	// Copy first part into buffer
	strcat((char*)USART_messageBuffer, (char*)message);		// Copy message into buffer
	strcat((char*)USART_messageBuffer, (char*)tailMessage);	// Copy last part into buffer
}
