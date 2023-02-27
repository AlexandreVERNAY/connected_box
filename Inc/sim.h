#ifndef SIM_H_
#define SIM_H_

#define LINE_1	1
#define LINE_2	2

void SIM_sendCommand(uint8_t *ATcommand);

void extractCommand(uint8_t commandLine);


#endif /* SIM_H_ */
