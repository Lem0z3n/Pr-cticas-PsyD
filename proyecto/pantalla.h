/*
 * pantalla.h
 *
 *  Created on: 09/01/2025
 *      Author: guill
 */

#ifndef PANTALLA_H_
#define PANTALLA_H_

#include "proyecto.h"

void plotTime(void);
void plotWelcomeScreen( void );
void plotSelectionScreen( parking_t* places);
void plotSelEnc(void);
void plotSelecFailure(parking_t parking);
void plotPaymentScreen(parking_t parking, uint8 credit);
void plotPayCredit(uint8 credit);
void plotPayDeadline(rtc_time_t deadline);
void plotPayDeadline(rtc_time_t deadline);
void plotPayCredit(uint8 credit);
void plotLicPlateScreen(parking_t parking);
void plotLicPlateUpdate(int16 licPlate);
void plotLicPlateEnc();
void plotSucScreen(parking_t parking);
void plotPayExcessFailure(void);
void plotPayInsufFailure(void);

#endif /* PANTALLA_H_ */
