#ifndef SCREEN_H_
#define SCREEN_H_

#define CODE_CONFIRM_CODE		"Confirmez vous ce code ?\n"
#define CODE_CONFIRM_CHOICE		"\nEnvoyez code suivi de:\noui pour valider,\nnon pour refuser."

#define COMMAND_VERIFY_CODE	0x17

#define GTP_accueil			"page accueil"
//#define GTP_saisie			"page saisie"
#define GTP_erreur_saisie	"page erreur_saisie"
#define GTP_depose			"page depose"
//#define GTP_recupe			"page recupe"
#define GTP_termine			"page termine"
#define GTP_erreur			"page erreur"
#define GTP_adtp			"page adtp"
#define GTP_END_COMMAND		"\xFF\xFF\xFF\0"

void SCREEN_sendCommand(uint8_t *command);

void SCREEN_executeCommand(void);

void SCREEN_extractCode(void);


#endif /* SCREEN_H_ */
