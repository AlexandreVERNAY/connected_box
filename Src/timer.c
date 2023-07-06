#include <stm32f446xx.h>
#include "main.h"
#include "timer.h"

void TIMx_Init(TIM_TypeDef *TIMx){
/*
 * Configure the Timer to one-pulse mode with 1 second delay
 */
//	[17.4.1]TIMx control register 1
	TIMx->CR1 &= ~TIM_CR1_CEN;			// Disable Timer
	TIMx->CR1 |= TIM_CR1_OPM;			// Enable one-pulse mode

//	[17.4.4]TIMx DMA/Interrupt enable register
	TIMx->DIER |= TIM_DIER_CC1IE;		// Enable Capture/Compare Timer interrupt

//	[17.4.5]TIMx status register
	TIMx->SR &= ~TIM_SR_UIF;			// Clear Update Interrupt Flag

//	[17.4.6]TIMx event generation register
	TIMx->EGR |= TIM_EGR_UG;			// Reset Timer prescaler counter

//	[17.4.11]TIMx prescaler
	TIMx->PSC = 15999;					// Set prescaler value for 1 ms (15 999  = (16 000 000 Hz/1 000 Hz) - 1)

//	[17.4.12]TIMx auto-reload register
	TIMx->ARR = 1000;					// Set Timer to interrupt after 1000 ms
}

void TIM2_Init(void){
/*
 * Configure Timer 2 used for USART1 time out
 */
//	[6.3.13]RCC APB1 peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable Clock for Timer 2

//	Configure Timer 2 to default configuration
	TIMx_Init(TIM2);					// Initialize Timer 2

//	Cortex M4 [4.3.2]Interrupt set-enable register x (NVIC_ISERx)
	NVIC_EnableIRQ(TIM2_IRQn);			// Enable global Interrupt for Timer 2
}

void TIM4_Init(void){
/*
 * Configure Timer 4 used for UART4 time out
 */
//	[6.3.13]RCC APB1 peripheral clock enable register
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;	// Enable Clock for Timer 4

//	Configure Timer 4 to default configuration
	TIMx_Init(TIM4);					// Initialize Timer 2

//	Cortex M4 [4.3.2] Interrupt set-enable register x (NVIC_ISERx)
	NVIC_EnableIRQ(TIM4_IRQn);			// Enable global Interrupt for Timer 4
}

void setTimeout(TIM_TypeDef *TIMx, uint16_t delay){
/*
 * Configure the Timer delay with the given value in ms
 */
//	[17.4.12]TIMx auto-reload register
	TIMx->ARR = delay;					// Set Timer to interrupt after X ms

//	[17.4.6]TIMx event generation register
	TIMx->EGR 	|= TIM_EGR_UG; 			// Reset Timer prescaler counter registers
}

void launchTimer(TIM_TypeDef *TIMx){
/*
 * Launch the timer
 */
//	[17.4.6]TIMx event generation register
	TIMx->EGR 	|= TIM_EGR_UG; 			// Reset Timer prescaler counter registers

//	[17.4.1]TIMx control register 1
	TIMx->CR1	|= TIM_CR1_CEN; 		// Enable Timer counter
}
