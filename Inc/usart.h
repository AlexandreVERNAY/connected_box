#ifndef USART_H_
#define USART_H_

void USART2_Init(void);
void UART4_Init(void);

void USART_TX(USART_TypeDef *USART, uint8_t *data);
void waitForTimeOut(void);


#endif /* USART_H_ */
