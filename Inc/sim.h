#ifndef SIM_H_
#define SIM_H_

#define LINE_1	1
#define LINE_2	2
#define LINE_3	3
#define LINE_4	4

void SIM_sendCommand(uint8_t *ATcommand);

void SIM_sendSMS(uint8_t *SMS);
void SIM_readSMS();

void extractCommand(uint8_t commandLine);

void SIM_commandInterpreter(void);


#endif /* SIM_H_ */
