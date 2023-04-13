#ifndef SIM_H_
#define SIM_H_

#define LINE_1	1
#define LINE_2	2
#define LINE_3	3
#define LINE_4	4

#define COMMAND_CODE_VALIDITY	'c'
#define COMMAND_GO_TO_PAGE		'p'

void SIM_sendCommand(uint8_t *ATcommand);

void SIM_configure(void);

void SIM_setIdentity(uint8_t *IDENTITY);

void SIM_extractCommand(uint8_t commandLine);

void SIM_sendSMS(uint8_t *SMS);
void SIM_readSMS(void);

void SIM_executeCommand(void);


#endif /* SIM_H_ */
