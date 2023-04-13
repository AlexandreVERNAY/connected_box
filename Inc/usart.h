#ifndef USART_H_
#define USART_H_

#define BUFFER_SIZE		128
#define COMMAND_SIZE	16

struct USART_Handler{
	uint8_t buffer[BUFFER_SIZE];	// Data reception buffer
	uint8_t size;					// Buffer's size
	uint8_t timeOut;				// Timer time out flag
	uint8_t command[COMMAND_SIZE];	// Module command string
};

void USART1_Init(void);
void UART4_Init(void);

void USART_TX(USART_TypeDef *USART, uint8_t *data);

void waitForTimeOut(struct USART_Handler *usart);

void createMessage(struct USART_Handler *usart, uint8_t *headMessage, uint8_t *message, uint8_t *tailMessage);


#endif /* USART_H_ */
