#ifndef USART_H_
#define USART_H_

#define BUFFER_SIZE		128
#define COMMAND_SIZE	16

struct USART_HANDLER{
	uint8_t buffer[BUFFER_SIZE];	// USART reception buffer
	uint8_t size;					// buffer's size
	uint8_t timeOut;				// Timer time out
	uint8_t command[COMMAND_SIZE];	// Module command string
};

void USART2_Init(void);
void UART4_Init(void);

void USART_TX(USART_TypeDef *USART, uint8_t *data);
void waitForTimeOut(struct USART_HANDLER *usart);


#endif /* USART_H_ */
