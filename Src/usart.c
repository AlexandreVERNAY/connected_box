#include <stm32f446xx.h>
#include "main.h"
#include "usart.h"

void USART2_Init(void){
/*
 * Configure USART2 using PA2(TX) and PA3(RX)
 */
//	[6.3.13]Peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;		// Enable USART2 clock

//	[7.4.1]GPIO port mode register
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER2_Pos);	// Set Alternate Function mode for Pin PA2
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER3_Pos);	// Set Alternate Function mode for Pin PA3

//	[7.4.9]GPIO alternate function low register
	GPIOA->AFR[0] |= (GPIO_AFR7 << GPIO_AFRL_AFSEL2_Pos);	// Set Alternate Function Port 7 for Pin PA2
	GPIOA->AFR[0] |= (GPIO_AFR7 << GPIO_AFRL_AFSEL3_Pos);	// Set Alternate Function Port 7 for Pin PA3

//	[25.6.3]Baud rate register
	USART2->BRR  = 139;							// Set baud rate (139 = 16 000 000 Hz/115200 bit/s)

//	[25.6.4]Control register 1
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;	// Enable Transmitter and Receiver for USART2
	USART2->CR1 |= USART_CR1_UE;				// Enable USART2

	USART2->CR1 |= USART_CR1_RXNEIE;			// Enable interrupt for Received data is ready to be read
	NVIC_EnableIRQ(USART2_IRQn);				// Enable USART2 global Interrupt
}

void UART4_Init(void){
/*
 * Configure UART4 using PA0(TX) and PA1(RX)
 */
//	[6.3.13]Peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;		// Enable UART4 clock

//	[7.4.1]Port mode register
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER0_Pos);	// Set Alternate Function mode for Pin PA0
	GPIOA->MODER |= (ALTERN_MODE << GPIO_MODER_MODER1_Pos);	// Set Alternate Function mode for Pin PA1

//	[7.4.9]GPIO alternate function low register
	GPIOA->AFR[0] |= (GPIO_AFR8 << GPIO_AFRL_AFSEL0_Pos);	// Set Alternate Function Port 8 for Pin PA0
	GPIOA->AFR[0] |= (GPIO_AFR8 << GPIO_AFRL_AFSEL1_Pos);	// Set Alternate Function Port 8 for Pin PA1

//	[25.6.3]Baud rate register
	UART4->BRR  = 139;							// Set baud rate (139 = 16 000 000 Hz/115200 bit/s)

//	[25.6.4]Control register 1
	UART4->CR1 |= USART_CR1_TE | USART_CR1_RE;	// Enable Transmitter and Receiver for UART4
	UART4->CR1 |= USART_CR1_UE;					// Enable UART4

	UART4->CR1 |= USART_CR1_RXNEIE;				// Enable interrupt for Received data is ready to be read
	NVIC_EnableIRQ(UART4_IRQn);					// Enable UART4 global Interrupt
}

void USART_TX(USART_TypeDef *USART, uint8_t *data){
/*
 * Transmit data over selected USART
 */
	for(; (*data != '\0'); data++){				// For every byte of data
		USART->DR = *data;						// Send data over USART
		while ((USART->SR & USART_SR_TXE)==0);	// Wait for transmission to complete
	}
}

void waitForTimeOut(struct USART_HANDLER *usart){
/*
 * Wait for Timer time out
 */
	while(!usart->timeOut);						// Wait for time out to be reached

	usart->buffer[usart->size] = '\0';			// Set end of string in the buffer
	usart->timeOut = 0;							// Reset flag time out
	usart->size = 0;							// Reset buffer size
}
