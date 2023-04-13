#ifndef TIMER_H_
#define TIMER_H_

void TIMx_Init(TIM_TypeDef *TIMx);

void TIM2_Init(void);
void TIM4_Init(void);

void setTimeout(TIM_TypeDef *TIMx, uint16_t delay);
void launchTimer(TIM_TypeDef *TIMx);


#endif /* TIMER_H_ */
