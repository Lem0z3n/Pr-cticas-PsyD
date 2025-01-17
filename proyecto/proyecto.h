/*
 * proyecto.h
 *
 *  Created on: 10/01/2025
 *      Author: guill
 */

#ifndef PROYECTO_H_
#define PROYECTO_H_

#include <common_types.h>
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <system.h>
#include <leds.h>
#include <segs.h>
#include <uart.h>
#include <rtc.h>
#include <timers.h>
#include <keypad.h>
#include <lcd.h>
#include <ts.h>

#include "mensajes.h"
#include "kernelcoop.h"
/////////////////////////////////////////////////////////////////////////////
// CONFIGURACION
/////////////////////////////////////////////////////////////////////////////

#define MAXPLACES              (8)
#define EXIT_ERROR ((int8) 1)
#define INSUF_ERROR ((int8) 2)
#define SELEC_ERROR ((int8) 3)

/////////////////////////////////////////////////////////////////////////////
// Declaracion de tipos
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
	uint8 id;
	boolean free;
	rtc_time_t deadline;
	uint16 licPlate;
	uint16 credit;
	int8 error;
} parking_t;

typedef enum { language, waiting, selection, payment, licPlateInput, success, error, exit} state_t;
/////////////////////////////////////////////////////////////////////////////
// Declaracion de funciones
/////////////////////////////////////////////////////////////////////////////

void setup( void );
uint8 getMonthdays(uint8 month);
void updateDeadline(uint16 credit, rtc_time_t * deadline);
uint8 getLicPlate(uint16 * licPlate);
void plotState(state_t state, uint8 placeSelected);
int8 changeLang(void);
int8 exitSol(void);
/////////////////////////////////////////////////////////////////////////////
// Declaracion de tareas
/////////////////////////////////////////////////////////////////////////////

void tsScanTask( void );
void kpScanTask( void );
void clockTask( void );
void coinsMoverTask( void );
void mainTask( void );
void ticketPrinterTask( void );
int8 selectorTask(void);


#endif /* PROYECTO_H_ */
