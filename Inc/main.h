#ifndef MAIN_H_
#define MAIN_H_

#define SIM_Init_OK	"Initialisation OK\r\n"	// Initialization was successful

//	GPIOx_MODER	[7.4.1]
#define INPUT_MODE 	0U
#define OUTPUT_MODE	1U
#define ALTERN_MODE	2U
#define ANALOG_MODE	3U

//	GPIOx_PUPDR	[7.4.4]
#define PUPDR_NO	0U
#define PUPDR_UP	1U
#define PUPDR_DOWN	2U

//	GPIOx_AFR	[7.4.9]
#define GPIO_AFR7 	7U
#define GPIO_AFR8 	8U

#define TIMEOUT_DEFAULT		1000
#define TIMEOUT_EXTENDED	5000
#define TIMEOUT_INIT		30000


#endif /* MAIN_H_ */
