#ifndef MENSAJES_H
#define MENSAJES_H

#include <common_types.h>

#define ESP ((int8) 0)
#define ENG ((int8) 1)
#define LAT ((int8) 2)

#define IGUALDA      ((uint8 *)0x0c210000)
#define JACK         ((uint8 *)0x0c220000)
#define ROMA         ((uint8 *)0x0c230000)
#define WORLD        ((uint8 *)0x0c240000)

typedef struct{
	const char* IDIOMA;
    const char* PULSE_PANTALLA;
    const char* PARA_COMENZAR;
    const char* HORARIO;
    const char* DOMINGO;
    const char* LUNES;
    const char* MARTES;
    const char* MIERCOLES;
    const char* JUEVES;
    const char* VIERNES;
    const char* SABADO;
    const char* SELECCIONE_PLAZA;
    const char* PULSE_PLAZA;
    const char* TARIFAS;
    const char* PRECIO_MIN;
    const char* ESTANCIA_MIN;
    const char* ESTANCIA_MAX;
    const char* TARIFA_MAXIMA;
    const char* DEVOLUCION;
    const char* PLAZA;
    const char* CREDITO_EUROS;
    const char* INSERTE_MONEDA;
    const char* PULSE_ACEPTAR;
    const char* FIN;
    const char* PLAZA_LICPLATE;
    const char* MATRICULA;
    const char* MATRICULA_VALIDA;
    const char* PLAZA_PAGADA;
    const char* FIN_APARCAMIENTO;
    const char* GRAN_DIA;
    const char* PLAZA_OCUPADA;
    const char* FIN_FAILURE;
    const char* SALDO_SUPERADO;
    const char* SALDO_MAXIMO;
    const char* SALDO_INSUFICIENTE;
    const char* SALDO_MIN;
    const char* SALIR;
    const char* LIBERADA;
    const char* MATRICULA_VEHIC;
} tMensajes;

void init_mens(tMensajes *men);
int8 set_lang(tMensajes *men,uint8 lang);

#endif
