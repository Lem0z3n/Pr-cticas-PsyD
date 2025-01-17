/*
 * pantalla.h
 *
 *  Created on: 09/01/2025
 *      Author: guill
 */

#ifndef PANTALLA_H_
#define PANTALLA_H_

#include "proyecto.h"

void init_screen_mens(void);
void plotTime(void);
void plotLangSelScreen(void);
void changeScreenLang(int8 lang);
void plotWelcomeScreen( void );
void plotSelectionScreen(parking_t *places);
void plotSelEnc(void);
void plotSelecFailure(parking_t parking);
void plotPaymentScreen(parking_t parking, uint8 credit);
void plotExcessReturn(int8 excess);
void plotPayCredit(uint8 credit);
void plotPayDeadline(rtc_time_t deadline);
void plotPayDeadline(rtc_time_t deadline);
void plotPayCredit(uint8 credit);
void plotLicPlateScreen(parking_t parking);
void plotLicPlateUpdate(int16 licPlate);
void plotLicPlateEnc();
void plotSucScreen(parking_t parking);
void plotExitFailure(parking_t parking);
void plotPayInsufFailure(void);
void plotErrorScreen(parking_t parking);
void plotLicPlateDemand(void);
void makeTicket( parking_t parking );
void plotExitSuccess(parking_t parking);
//todo error screen generica.


#endif /* PANTALLA_H_ */
