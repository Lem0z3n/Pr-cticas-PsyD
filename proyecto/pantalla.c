/*
 * pantalla.c
 *
 *  Created on: 09/01/2025
 *      Author: guill
 */
#include <lcd.h>
#include <rtc.h>
#include "proyecto.h"

tMensajes mensaje;

void init_screen_mens()
{
	init_mens(&mensaje);
}

void plotTime()
{

	rtc_time_t rtc_time;

	rtc_gettime(&rtc_time);

	lcd_puts(84,0,BLACK, rtc_stringDate(rtc_time));
	lcd_puts_x2(LCD_WIDTH-48,0,BLACK,mensaje.IDIOMA);
}

void plotLangSelScreen(void)
{
	lcd_clear();
	lcd_puts_x2( 24, 48, BLACK, "IDIOMA / LANGUAGE" );
	lcd_puts_x2(LCD_WIDTH/4-24, 76, BLACK, "ESP");
	lcd_puts_x2(LCD_WIDTH*3/4-24, 76, BLACK, "ENG");
	lcd_puts_x2(LCD_WIDTH/2-24, 146, BLACK, "LAT");
}

void changeScreenLang(int8 lang)
{
	set_lang(&mensaje, lang);
}

void plotWelcomeScreen( void )
{

    lcd_clear();
    lcd_puts_x2( 24, 48, BLACK, mensaje.PULSE_PANTALLA );
    lcd_puts_x2( 24, 76, BLACK, mensaje.PARA_COMENZAR );
    lcd_draw_box(0, 126, LCD_WIDTH-1 , LCD_HEIGHT-20, BLACK, 1);
    lcd_puts(66, 122, BLACK, mensaje.HORARIO);
    lcd_puts(24, 140, BLACK, mensaje.DOMINGO);
    lcd_puts(24, 156, BLACK, mensaje.LUNES);
    lcd_puts(24, 172, BLACK, mensaje.MARTES);
    lcd_puts(24, 188, BLACK, mensaje.MIERCOLES);
    lcd_puts(166, 140, BLACK, mensaje.JUEVES);
    lcd_puts(166, 156, BLACK, mensaje.VIERNES);
    lcd_puts(166, 172, BLACK, mensaje.SABADO);

}


void plotSelectionScreen( parking_t* places)
{
	lcd_clear();
	lcd_puts_x2( 32, 32, BLACK, mensaje.SELECCIONE_PLAZA );
	lcd_draw_box(0, 64, LCD_WIDTH/4, 128, BLACK, 1);
	lcd_draw_box(LCD_WIDTH/4, 64, LCD_WIDTH*2/4, 128, BLACK, 1);
	lcd_draw_box(LCD_WIDTH*2/4, 64, LCD_WIDTH*3/4, 128, BLACK, 1);
	lcd_draw_box(LCD_WIDTH*3/4, 64, LCD_WIDTH-1, 128, BLACK, 1);
	lcd_draw_box(0, 128 , LCD_WIDTH/4, 192, BLACK, 1);
	lcd_draw_box(LCD_WIDTH/4, 128, LCD_WIDTH*2/4, 192, BLACK, 1);
	lcd_draw_box(LCD_WIDTH*2/4, 128, LCD_WIDTH*3/4, 192, BLACK, 1);
	lcd_draw_box(LCD_WIDTH*3/4, 128, LCD_WIDTH-1, 192, BLACK, 1);

	int i, x = LCD_WIDTH/16+8, y = 80;
	char id;
	for(i = 0; i < MAXPLACES; i++)
	{
		if(places[i].free)
		{
			lcd_putint_x2(x,y,BLACK, i+1);
		}
		else
		{
			id ='X';
			lcd_putchar_x2(x,y,BLACK,id);
		}

		x += LCD_WIDTH/4;
		if(i == 3) {
			x = LCD_WIDTH/16+8;
			y = 145;
		}

	}
}

void plotSelEnc()
{
	lcd_puts( 32, LCD_HEIGHT - 35, BLACK, (mensaje.PULSE_PLAZA));
}

void plotPayCredit(uint16 credit)
{
	lcd_putint((LCD_WIDTH/2)+8, 80 + 50, BLACK, credit/100);
	lcd_putint((LCD_WIDTH/2)+24, 80 + 50, BLACK, credit%100);
}

void plotPayDeadline(rtc_time_t deadline)
{
	lcd_puts( (LCD_WIDTH/2) -100, 80 + 70, BLACK, mensaje.FIN);
	lcd_puts( (LCD_WIDTH/2) -36, 80 + 70, BLACK, rtc_stringDate(deadline));
}

void plotPaymentScreen(parking_t parking, uint8 credit)
{
	lcd_clear();
	lcd_draw_box(0,18,LCD_WIDTH-1,80,BLACK,1);
	lcd_puts(LCD_WIDTH/2-32, 14, BLACK, mensaje.TARIFAS);
	lcd_puts(32,30,BLACK,mensaje.PRECIO_MIN);
	lcd_puts(32,44,BLACK,mensaje.ESTANCIA_MAX);
	lcd_puts(32,62,BLACK,mensaje.ESTANCIA_MIN);
	lcd_puts_x2( 64, 90, BLACK, mensaje.PLAZA);
	lcd_putint_x2( 64+112, 90, BLACK, parking.id);
	lcd_puts((LCD_WIDTH/2) -80, 80 + 50, BLACK, mensaje.CREDITO_EUROS);
	plotPayCredit(credit);
	plotPayDeadline(parking.deadline);
	lcd_puts( (LCD_WIDTH/2) -56, LCD_HEIGHT - 65, BLACK, mensaje.INSERTE_MONEDA);
	lcd_puts( 32, LCD_HEIGHT - 49, BLACK, mensaje.PULSE_ACEPTAR);

}

void plotExcessReturn(int8 excess)
{
	lcd_puts(32, LCD_HEIGHT-33, BLACK, mensaje.TARIFA_MAXIMA);
	lcd_puts(32, LCD_HEIGHT-17, BLACK, mensaje.DEVOLUCION);
	lcd_putint(192, LCD_HEIGHT-17, BLACK, excess);
	lcd_puts(216, LCD_HEIGHT-17, BLACK, "euros");
}

void plotLicPlateScreen(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 64, 40, BLACK, mensaje.PLAZA);
	lcd_putint_x2( 64+112, 40, BLACK, parking.id);
	lcd_puts((LCD_WIDTH/2) -100, 95, BLACK, mensaje.MATRICULA);
	plotPayDeadline(parking.deadline);
}

void plotLicPlateUpdate(int16 licPlate)
{
	lcd_puthex((LCD_WIDTH/2) +28, 95, BLACK, licPlate);
}

void plotLicPlateEnc()
{
	lcd_puts( 16, LCD_HEIGHT - 35, BLACK, mensaje.MATRICULA_VALIDA);
}

void plotSucScreen(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 32, LCD_HEIGHT /2-50, BLACK, mensaje.PLAZA_PAGADA );
	lcd_putint_x2( 128, LCD_HEIGHT /2-50, BLACK, parking.id );
	lcd_puts( 32, LCD_HEIGHT /2 -24, BLACK, mensaje.MATRICULA);
	lcd_puthex(160, 95, BLACK, parking.licPlate);
	lcd_puts( 32, LCD_HEIGHT /2 +12, BLACK, mensaje.FIN_APARCAMIENTO);
	lcd_puts( 104, LCD_HEIGHT /2 +12, BLACK, rtc_stringDate(parking.deadline));
	lcd_puts( 32, LCD_HEIGHT /2 + 50, BLACK, mensaje.GRAN_DIA);
}

void plotSelecFailure(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 32, LCD_HEIGHT /2-70, BLACK, mensaje.PLAZA_OCUPADA );
	lcd_putint_x2( 128, LCD_HEIGHT /2-70, BLACK, parking.id );
	lcd_puts( 32, LCD_HEIGHT /2 + 38, BLACK, mensaje.FIN_FAILURE);
	lcd_puts( 104, LCD_HEIGHT /2 + 38, BLACK, rtc_stringDate(parking.deadline));
	lcd_draw_box(LCD_WIDTH/2-80, LCD_HEIGHT-64, LCD_WIDTH/2+80, LCD_HEIGHT-16, BLACK, 1);
	lcd_puts(LCD_WIDTH/2-16, LCD_HEIGHT-48, BLACK, mensaje.SALIR);
}

void plotLicPlateDemand()
{
	lcd_puts(32, 79, BLACK, mensaje.MATRICULA_VALIDA);
}

void plotExitFailure(parking_t parking)
{
	lcd_clear();
	lcd_puts(16, (LCD_HEIGHT/2)-30, BLACK, mensaje.MATRICULA_VEHIC);
}

void plotPayInsufFailure()
{
	lcd_clear();
	lcd_puts_x2(16, (LCD_HEIGHT/2)-30, BLACK, mensaje.SALDO_INSUFICIENTE);
	lcd_puts(40, (LCD_HEIGHT/2), BLACK, mensaje.SALDO_MIN);
}

void plotErrorScreen(parking_t parking)
{
	switch (parking.error)
	{
	case EXIT_ERROR:
		plotExitFailure(parking);
		break;
	case INSUF_ERROR:
		plotPayInsufFailure();
		break;
	case SELEC_ERROR:
		plotSelecFailure(parking);
		break;
	}
}

void makeTicket( parking_t parking )
{
	uart0_puts("\n ------------------------\n");
	uart0_puts(mensaje.PLAZA);
	uart0_putint(parking.id);
	uart0_putchar('\n');
	uart0_puts(mensaje.MATRICULA);
	uart0_puthex(parking.licPlate);
	uart0_putchar('\n');
	uart0_puts(mensaje.FIN);
	uart0_puts(rtc_stringDate(parking.deadline));
	uart0_puts("\n ------------------------\n");
}

void plotExitSuccess(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 32, 40, BLACK, mensaje.PLAZA);
	lcd_putint_x2( 32+112, 40, BLACK, parking.id);
	lcd_puts_x2( 32+128, 40, BLACK, mensaje.LIBERADA);
	lcd_puts_x2(8,100,BLACK,mensaje.GRAN_DIA);
}

