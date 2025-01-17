#include <stdio.h> // Para poder hacer = NULL
#include "mensajes.h"


void init_mens(tMensajes *men)
{
	set_lang(men,ESP);
}

int8 set_lang(tMensajes *men,uint8 lang){
	if (lang == ESP) {
			men->IDIOMA ="ESP";
	        men->PULSE_PANTALLA = "Pulse la pantalla";
	        men->PARA_COMENZAR = " para comenzar ";
	        men->HORARIO = " HORARIO DE FUNCIONAMIENTO ";
	        men->DOMINGO = "dom: gratis";
	        men->LUNES = "lun: 09:00-21:00";
	        men->MARTES = "mar: 09:00-21:00";
	        men->MIERCOLES = "mie: 09:00-21:00";
	        men->JUEVES = "jue: 09:00-21:00";
	        men->VIERNES = "vie: 09:00-21:00";
	        men->SABADO = "sab: 09:00-15:00";
	        men->SELECCIONE_PLAZA = "Seleccione plaza";
	        men->PULSE_PLAZA = "Pulse una plaza por favor";
	        men->TARIFAS = "TARIFAS";
	        men->PRECIO_MIN = "Precio por minuto: 0,01 euros";
	        men->ESTANCIA_MIN = " Estancia minima: 20 minutos";
	        men->ESTANCIA_MAX = " Estancia maxima: 240 minutos";
	        men->PLAZA = "PLAZA: ";
	        men->CREDITO_EUROS ="Credito:    ,   euros";
	        men->INSERTE_MONEDA = "Inserte monedas";
	        men->PULSE_ACEPTAR = "Pulse la pantalla para aceptar";
	        men->TARIFA_MAXIMA = "Estancia maxima superada";
	        men->DEVOLUCION    = "Devolviendo monedas";
	        men->FIN = "  Fin: ";
	        men->PLAZA_LICPLATE = "PLAZA";
	        men->MATRICULA = "Matricula:";
	        men->MATRICULA_VALIDA = "Introduzca una matricula valida.";
	        men->PLAZA_PAGADA = "Plaza i pagada";
	        men->FIN_APARCAMIENTO = "    Fin: ";
	        men->GRAN_DIA = "Que tenga un gran dia ";
	        men->PLAZA_OCUPADA = "Plaza i ocupada";
	        men->FIN_FAILURE = "    Fin: ";
	        men->SALDO_SUPERADO = "el maximo saldo es 2,40 euros";
	        men->SALDO_MAXIMO ="el maximo saldo es 2,40 euros";
	        men->SALDO_INSUFICIENTE = "Saldo insuficiente";;
	        men->SALDO_MIN = "el saldo minimo es 0,20 euros";
	        men->SALIR = "    IRSE";
	        men->LIBERADA = "LIBERADA";
	        men->MATRICULA_VEHIC = "Introduzca la matricula de su coche.";
	    }
	else if (lang == ENG){
			men->IDIOMA ="ENG";
	        men->PULSE_PANTALLA = "		Touch screen";
	        men->PARA_COMENZAR = " 		  to start ";
	        men->HORARIO = " OPERATING HOURS ";
	        men->DOMINGO = "Sun: free";
	        men->LUNES = "Mon: 09:00-21:00";
	        men->MARTES = "Tue: 09:00-21:00";
	        men->MIERCOLES = "Wed: 09:00-21:00";
	        men->JUEVES = "Thu: 09:00-21:00";
	        men->VIERNES = "Fri: 09:00-21:00";
	        men->SABADO = "Sat: 09:00-15:00";
	        men->SELECCIONE_PLAZA = "	Select spot";
	        men->PULSE_PLAZA = "Please select a spot";
	        men->TARIFAS = "RATES";
	        men->PRECIO_MIN = "Price per minute: 0.01 euros";
	        men->ESTANCIA_MIN = " Minimum stay: 20 minutes";
	        men->ESTANCIA_MAX = " Maximum stay: 240 minutes";
	        men->PLAZA = "SPOT:  ";
	        men->CREDITO_EUROS ="Credit:     ,   euros";
	        men->INSERTE_MONEDA =  "Insert coins";
	        men->PULSE_ACEPTAR = "Touch the screen to confirm";
	        men->TARIFA_MAXIMA = "Maximum stay reached";
	        men->DEVOLUCION    = "Returning coins";
	        men->FIN = "  End: ";
	        men->PLAZA_LICPLATE = "SPOT:  ";
	        men->MATRICULA = "License plate:";
	        men->MATRICULA_VALIDA = "Enter a valid license plate.";
	        men->PLAZA_PAGADA = "Spot  i paid";
	        men->FIN_APARCAMIENTO = "    End: ";
	        men->GRAN_DIA = "Have a great day ";
	        men->PLAZA_OCUPADA = "Spot  i occupied";
	        men->FIN_FAILURE = "    End: ";
	        men->SALDO_SUPERADO = "Max balance exceeded";
	        men->SALDO_MAXIMO = "the maximum balance is 2.40 euros";
	        men->SALDO_INSUFICIENTE = "Insufficient balance";
	        men->SALDO_MIN = "the minimum balance is 0.20 euros";
	        men->SALIR = "   LEAVE";
	        men->LIBERADA = "FREED";
	        men->MATRICULA_VEHIC = "Insert your cars license plate.";
	    }
	    else if (lang == LAT) {
	    	men->IDIOMA ="LAT";
	        men->PULSE_PANTALLA = "	 Tabulam carpere";
	        men->PARA_COMENZAR  = "    ad incipere ";
	        men->HORARIO = " SCHEDULA OPERATIONIS ";
	        men->DOMINGO = "Sol: non solvere";
	        men->LUNES = "Lun: IX-XXI";
	        men->MARTES = "Mar: IX-XXI";
	        men->MIERCOLES = "Mer: IX-XXI";
	        men->JUEVES = "Jov: IX-XXI";
	        men->VIERNES = "Ven: IX-XXI";
	        men->SABADO = "Sat: IX-XV";
	        men->SELECCIONE_PLAZA = "  Locum eligere";
	        men->PULSE_PLAZA = "Unun locum carpere, amabo";
	        men->TARIFAS = "PRETIUM";
	        men->PRECIO_MIN = "Pretium per minutum: 0,0I denarios";
	        men->ESTANCIA_MIN = " Tempus minimus: XX denarios";
	        men->ESTANCIA_MAX = " Tempus maximus: CCXL denarios";
	        men->PLAZA = "LOCUS: ";
	        men->CREDITO_EUROS = "Quantitas:  ,   denarios";
	        men->INSERTE_MONEDA = "Denario insertus";
	        men->PULSE_ACEPTAR  = "Tabulam carpere at accipere";
	        men->TARIFA_MAXIMA  = "Tempus maximus pervenit";
	        men->DEVOLUCION     = "redde monetas";
	        men->FIN = "Finis: ";
	        men->PLAZA_LICPLATE = "LOCUS: ";
	        men->MATRICULA = "Numeri vectura:";
	        men->MATRICULA_VALIDA = "Insertus bona numeri vectura.";
	        men->PLAZA_PAGADA = "Locus i solvit";
	        men->FIN_APARCAMIENTO =  "    Finis: ";
	        men->GRAN_DIA = "Habere bonum diem ";
	        men->PLAZA_OCUPADA ="Locus i occupatus";
	        men->FIN_FAILURE = "  Finis: ";
	        men->SALDO_SUPERADO = "Quantitas maximus exceditur";
	        men->SALDO_MAXIMO = "Maximus quantitas II,XL denarios est";
	        men->SALDO_INSUFICIENTE = "Quantitas deficiens";
	        men->SALDO_MIN = "Quantitas minimus 0,XX denarios est";
	        men->SALIR = "RELINQUO";
	        men->LIBERADA = "LIBERATUS";
	        men->MATRICULA_VEHIC = "Insertus numerum licentiae tuae carri";
	    }
	else
		return -1;
}



