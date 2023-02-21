#include <stm32f446xx.h>

#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "interrupt.h"

int main(void){
	PA5_Init();
	PC13_Init();
	TIM2_Init();

	return 0;
}
