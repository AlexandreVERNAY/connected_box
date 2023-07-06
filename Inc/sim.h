#ifndef SIM_H_
#define SIM_H_

#define NB_TRY	3											// Number of attempt to send a message

#define LINE_1	1											// Select line number 1 in a string
#define LINE_2	2											// Select line number 2 in a string
#define LINE_3	3											// Select line number 3 in a string
#define LINE_4	4											// Select line number 4 in a string

#define COMMAND_CODE_VALIDITY	"code"						// Command to verify a user code
#define COMMAND_GO_TO_PAGE		"page"						// Command to change page

#define AT_ECHO_MODE_OFF	"ATE0\r\0"						// Disable echo mode for SIM module

#define AT_TEXT_MODE_ON		"AT+CMGF=1\r\0"					// Enable text mode for SIM module

#define AT_GET_IDENTITY		"AT+CGSN\r\0"					// Obtain the IMEI of the SIM module

#define AT_SMS_ERASE_ALL	"AT+CMGD=,4\r\0"				// Erase all SMS stored in SIM module
#define AT_SMS_READ_FIRST	"AT+CMGR=0,0\r\0"				// Read first SMS Message
#define AT_SMS_WRITE 		"AT+CMGS=\"+33784961263\"\r\0"	// Write an SMS to a phone number in text mode
#define AT_SMS_END_TEXT		"\x1A\0"						// End of text character for SMS

#define CODE_GOOD			"code oui"						// Response if the user code is good
#define CODE_WRONG			"code non"						// Response if the user code is wrong

#define SIM_ERROR			"ERROR"							// Response from SIM module if an error occurred

void SIM_sendCommand(uint8_t *ATcommand);

uint8_t errorDetection(void);

void SIM_configure(void);

uint8_t* extractLine(uint8_t commandLine);
void copyLine(uint8_t *dest, uint8_t *source);
void SIM_setIdentity(uint8_t *IDENTITY);

void SIM_extractCommand(uint8_t *dest, uint8_t commandLine);

void SIM_sendSMS(uint8_t *SMS);
void SIM_readSMS(void);

void SIM_executeCommand(void);
uint8_t codeValidity(void);
uint8_t goToPage(void);


#endif /* SIM_H_ */
