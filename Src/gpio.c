#include <stm32f446xx.h>
#include "main.h"
#include "gpio.h"

void PA5_Init(void){
/*
 * Configure GPIO Port A Pin 5 (LED)
 */
//	[6.3.10]RCC AHB1 peripheral clock enable register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;					// Clock initialization for Port A

//	[7.4.1]GPIO port mode register
	GPIOA->MODER |=  (OUTPUT_MODE << GPIO_MODER_MODE5_Pos);	// Set Port A Pin 5 to output mode

//	[7.4.2]GPIO port output type register
	GPIOA->OTYPER &= ~(PUPDR_UP << GPIO_OTYPER_OT5_Pos);	// Set Port A Pin 5 to open-drain

//	[7.4.6]GPIO port output data register
	GPIOA->ODR	&= ~(1 << GPIO_ODR_OD5_Pos);				// Set Port A Pin 5 to LOW level (off)
}

void PC13_Init(void){
/*
 * Configure GPIO Port C Pin 13 (BUTTON)
 */
//	[6.3.10]RCC AHB1 peripheral clock enable register
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;					// Clock initialization for Port C

//	[7.4.1]GPIO port mode register
	GPIOC->MODER |= (INPUT_MODE << GPIO_PUPDR_PUPD13_Pos);	// Set Port C Pin 13 to output mode

//	[7.4.4]GPIO port pull-up/pull-down register
	GPIOC->PUPDR |= (PUPDR_UP << GPIO_PUPDR_PUPD13_Pos);	// Set Port C Pin 13 to open-drain

//	[6.3.14]RCC APB2 peripheral clock enable register
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;					// Enable System configuration controller clock

//	[8.2.6]SYSCFG external interrupt configuration register 4
	SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC;			// Select Port C Pin 13 as source for EXTI external interrupt

//	[10.3.1]Interrupt mask register
	EXTI->IMR |= EXTI_IMR_IM13;								// Disable interrupt request mask on EXTI line 13

//	[10.3.3]Rising trigger selection register
	EXTI->FTSR &=  EXTI_FTSR_TR13;							// Disable EXTI line 13 trigger on falling edge

//	[10.3.4]Falling trigger selection register
	EXTI->RTSR |= ~EXTI_RTSR_TR13;							// Enable EXTI line 13 trigger on rising edge

//	[6.3.14]RCC APB2 peripheral clock enable register
	RCC->APB2ENR &= ~RCC_APB2ENR_SYSCFGEN;					// Disable System configuration controller clock

//	Cortex M4 [4.3.2]Interrupt set-enable register x (NVIC_ISERx)
//	NVIC_EnableIRQ(EXTI15_10_IRQn);							// Enable interrupt for Port C Pin 13
}
