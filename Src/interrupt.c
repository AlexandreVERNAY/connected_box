#include <stm32f446xx.h>
#include "main.h"
#include "interrupt.h"

extern uint8_t *buffer;
extern uint8_t size;
extern uint8_t timeOut;

void EXTI15_10_IRQHandler(void){
/*
 * Interruption function for PC13 (Button pressed)
 */
//	[7.4.6]GPIO port output data register
	GPIOA->ODR	|= GPIO_ODR_OD5;	// Turn on the user LED

//	[17.4.6]TIMx event generation register
	TIM2->EGR 	|= TIM_EGR_UG; 		// Reset TIM2 counter and prescaler registers
//	[17.4.1]TIMx control register 1
    TIM2->CR1	|= TIM_CR1_CEN; 	// Enable TIM2 counter

//	[10.3.6]Pending register
	EXTI->PR	|= EXTI_PR_PR13;	// Clear PC13 pending interruption
}

void TIM2_IRQHandler(void){
/*
 * Interruption function for TIM2 (SIM module response time out)
 */
//	[7.4.6]GPIO port output data register
	GPIOA->ODR	&= ~GPIO_ODR_OD5;	// Turn off the user LED

	timeOut = 1;					// Set flag time out has been reached

//	[17.4.5]TIMx status register
	TIM2->SR	&= ~TIM_SR_CC1IF;	// Clear TIM2 pending interruption
}

void USART2_IRQHandler(void){
	GPIOA->ODR	|= GPIO_ODR_OD5;	// Turn on the user LED for debugging
}

void UART4_IRQHandler(void){
/*
 * Interruption function for UART4 (New byte received from SIM module)
 */
	buffer[size] = UART4->DR;		// Fetch SIM module data in buffer
	size++;							// Select next byte of buffer
}
