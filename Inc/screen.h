#ifndef SCREEN_H_
#define SCREEN_H_

void SCREEN_sendCommand(uint8_t *command);

void SCREEN_commandInterpreter(void);

void extractCode(void);


#endif /* SCREEN_H_ */
