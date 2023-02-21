#include <stm32f446xx.h>
#include "main.h"
#include "interrupt.h"

void EXTI15_10_IRQHandler(void){
/*
 * Interruption function for PC13
 */
//	[7.4.6]GPIO port output data register
	GPIOA->ODR	|= GPIO_ODR_OD5;	// Turn on the user LED

//	[17.4.6]TIMx event generation register
	TIM2->EGR 	|= TIM_EGR_UG; 		// Reset TIM2
//	[17.4.1]TIMx control register 1
    TIM2->CR1	|= TIM_CR1_CEN; 	// Enable TIM2 counter

//	[10.3.6]Pending register
	EXTI->PR	|= EXTI_PR_PR13;	// Clear PC13 pending interuption
}

void TIM2_IRQHandler(void){
/*
 * Interruption function for TIM2
 */
//	[7.4.6]GPIO port output data register
	GPIOA->ODR	&= ~GPIO_ODR_OD5;	// Turn off the user LED
//	[17.4.5]TIMx status register
	TIM2->SR	&= ~TIM_SR_CC1IF;	// Clear TIM2 pending interuption
}
