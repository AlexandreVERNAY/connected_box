#include <stm32f446xx.h>
#include "main.h"
#include "timer.h"

void TIM2_Init(void){
/*
 * Configure Timer 2 used for timeout
 */
//	[6.3.13]RCC APB1 peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable clock for Timer 2

//	[17.4.11]TIMx prescaler
	TIM2->PSC = 15999;    		// Set prescaler value for 1 ms

//	[17.4.12]TIMx auto-reload register
	TIM2->ARR = 1000;			// Set Timer 2 to interrupt after 1000 ms

//	[17.4.4]TIMx DMA/Interrupt enable register
	TIM2->DIER |= TIM_SR_CC1IF;	// Enable Timer 2 interrupt generation

//	[17.4.5]TIMx status register
    TIM2->SR &= ~TIM_SR_UIF;	// Clear UIF update interrupt flag

//	[17.4.6]TIMx event generation register
    TIM2->EGR |= TIM_EGR_UG;	// Reset Timer 2 counter and prescaler registers

//	[17.4.1]TIMx control register 1
    TIM2->CR1 &= ~TIM_CR1_CEN;	// Disable Timer 2
    TIM2->CR1 |= TIM_CR1_OPM;	// Enable one-pulse mode

	NVIC_EnableIRQ(TIM2_IRQn);	// Enable interrupt for Timer 2
}

void setTimeout(uint16_t delay){
/*
 * Configure and launch the timer for the given delay in ms
 */
//	[17.4.12]TIMx auto-reload register
	TIM2->ARR = delay;			// Set TIMx to interrupt after X ms

//	[17.4.6]TIMx event generation register
	TIM2->EGR 	|= TIM_EGR_UG; 	// Reset TIMx counter and prescaler registers
}

void launchTimer(void){
//	[7.4.6]GPIO port output data register
	GPIOA->ODR	|= GPIO_ODR_OD5;// Turn on the user LED for debugging

//	[17.4.6]TIMx event generation register
	TIM2->EGR 	|= TIM_EGR_UG; 	// Reset Timer 2 counter and prescaler registers

//	[17.4.1]TIMx control register 1
	TIM2->CR1	|= TIM_CR1_CEN; // Enable Timer 2 counter
}
