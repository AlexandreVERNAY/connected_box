#include <stm32f446xx.h>
#include "main.h"
#include "interrupt.h"
#include "usart.h"

extern struct USART_HANDLER *SCREEN;
extern struct USART_HANDLER *SIM;

void EXTI15_10_IRQHandler(void){
/*
 * Interruption function for PC13 (Button pressed)
 */
//	[7.4.6]GPIO port output data register
	GPIOA->ODR ^= GPIO_ODR_OD5;		// Toggle the user LED for debugging
//	[10.3.6]Pending register
	EXTI->PR	|= EXTI_PR_PR13;	// Clear PC13 pending interruption
}

void TIM2_IRQHandler(void){
/*
 * Interruption function for TIM2 (SCREEN module response time out)
 */
	SCREEN->timeOut = 1;			// Set SCREEN module time out flag

//	[17.4.5]TIMx status register
	TIM2->SR	&= ~TIM_SR_CC1IF;	// Clear TIM2 pending interruption
}

void TIM4_IRQHandler(void){
/*
 * Interruption function for TIM4 (SIM module response time out)
 */
	SIM->timeOut = 1;				// Set SIM module time out flag

//	[17.4.5]TIMx status register
	TIM4->SR	&= ~TIM_SR_CC1IF;	// Clear TIM4 pending interruption
}

void USART2_IRQHandler(void){
/*
 * Interruption function for USART2 (New byte received from SCREEN module)
 */
	SCREEN->buffer[SCREEN->size] = USART2->DR;	// Fetch SCREEN module data in SCREEN buffer
	SCREEN->size++;								// Select next byte in SCREEN buffer
}

void UART4_IRQHandler(void){
/*
 * Interruption function for UART4 (New byte received from SIM module)
 */
	SIM->buffer[SIM->size] = UART4->DR;			// Fetch SIM module data in SIM buffer
	SIM->size++;								// Select next byte in SIM buffer
}
