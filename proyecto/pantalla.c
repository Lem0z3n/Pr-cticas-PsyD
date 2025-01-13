/*
 * pantalla.c
 *
 *  Created on: 09/01/2025
 *      Author: guill
 */
#include <lcd.h>
#include <rtc.h>
#include "proyecto.h"

void plotTime()
{

	rtc_time_t rtc_time;

	rtc_gettime(&rtc_time);

	lcd_puts(84,0,BLACK, rtc_stringDate(rtc_time));
}

void plotWelcomeScreen( void )
{

    lcd_clear();
    lcd_puts_x2( 24, 48, BLACK, "Pulse la pantalla" );
    lcd_puts_x2( 24, 76, BLACK, "  para comenzar  " );
    lcd_draw_box(0, 126, LCD_WIDTH-1 , LCD_HEIGHT-20, BLACK, 1);
    lcd_puts(66, 122, BLACK, " HORARIO DE FUNCIONAMIENTO ");
    lcd_puts(24, 140, BLACK, "dom: gratis");
    lcd_puts(24, 156, BLACK, "lun: 09:00-21:00");
    lcd_puts(24, 172, BLACK, "mar: 09:00-21:00");
    lcd_puts(24, 188, BLACK, "mie: 09:00-21:00");
    lcd_puts(166, 140, BLACK, "jue: 09:00-21:00");
    lcd_puts(166, 156, BLACK, "vie: 09:00-21:00");
    lcd_puts(166, 172, BLACK, "sab: 09:00-15:00");

}


void plotSelectionScreen( parking_t* places)
{
	lcd_clear();
	lcd_puts_x2( 32, 32, BLACK, "Seleccione plaza" );
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

		x += LCD_WIDTH/8;
		if(i == 3) {
			x = LCD_WIDTH/16+8;
			y = 145;
		}

	}
}

void plotSelEnc()
{
	lcd_puts( 32, LCD_HEIGHT - 35, BLACK, ("Pulse una plaza por favor."));
}

void plotPaymentScreen(parking_t parking, uint8 credit)
{
	lcd_clear();
	lcd_draw_box(0,16,LCD_WIDTH-1,70,BLACK,1);
	lcd_puts(LCD_WIDTH/2-32, 16, BLACK, " TARIFAS ");
	lcd_puts(32,26,BLACK,"Precio por minuto: 0,01 euros");
	lcd_puts(32,42,BLACK," Estancia minima:  20 minutos");
	lcd_puts(32,58,BLACK," Estancia maxima: 240 minutos");
	lcd_puts_x2( 64, 80, BLACK, "PLAZA ");
	lcd_putint_x2( 64+48, 80, BLACK, parking.id);
	lcd_puts((LCD_WIDTH/2) -80, 80 + 50, BLACK, "Credito:   ,   euros");
	plotPayCredit(credit);
	plotPayDeadline(parking.deadline);
	lcd_puts( (LCD_WIDTH/2) -56, LCD_HEIGHT - 55, BLACK, ("Inserte monedas"));
	lcd_puts( 32, LCD_HEIGHT - 35, BLACK, ("Pulse la pantalla para aceptar"));

}

void plotPayCredit(uint8 credit)
{
	lcd_putint((LCD_WIDTH/2), 80 + 50, BLACK, credit/100);
	lcd_putint((LCD_WIDTH/2)+16, 80 + 50, BLACK, credit%100);
}

void plotPayDeadline(rtc_time_t deadline)
{
	lcd_puts( (LCD_WIDTH/2) -100, 80 + 70, BLACK, "Fin: ");
	lcd_puts( (LCD_WIDTH/2) -60, 80 + 70, BLACK, rtc_stringDate(deadline));
}

void plotLicPlateScreen(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 64, 40, BLACK, "PLAZA ");
	lcd_putint_x2( 64+48, 40, BLACK, parking.id);
	lcd_puts((LCD_WIDTH/2) -100, 95, BLACK, "Matricula: ?");
	plotPayDeadline(parking.deadline);
}

void plotLicPlateUpdate(int16 licPlate)
{
	lcd_puthex((LCD_WIDTH/2) -12, 95, BLACK, licPlate);
}

void plotLicPlateEnc()
{
	lcd_puts( 16, LCD_HEIGHT - 35, BLACK, ("Introduzca una matricula valida por favor."));
}

void plotSucScreen(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 32, LCD_HEIGHT /2-50, BLACK, ("Plaza i pagada") );
	lcd_putint_x2( 80, LCD_HEIGHT /2-50, BLACK, parking.id );
	lcd_puts( 32, LCD_HEIGHT /2 -12, BLACK, ("    Fin: "));
	lcd_puts( 104, LCD_HEIGHT /2 -12, BLACK, rtc_stringDate(parking.deadline));
	lcd_puts( 32, LCD_HEIGHT /2 + 50, BLACK, ("    Que tenga un gran dia "));
}

void plotSelecFailure(parking_t parking)
{
	lcd_clear();
	lcd_puts_x2( 32, LCD_HEIGHT /2-50, BLACK, ("Plaza i ocupada") );
	lcd_putint_x2( 80, LCD_HEIGHT /2-50, BLACK, parking.id );
	lcd_puts( 32, LCD_HEIGHT /2 + 38, BLACK, ("    Fin: "));
	lcd_puts( 104, LCD_HEIGHT /2 + 38, BLACK, rtc_stringDate(parking.deadline));
}

void plotPayExcessFailure()
{
	lcd_clear();
	lcd_puts_x2(16, (LCD_HEIGHT/2)-30, BLACK, "Saldo max superado");
	lcd_puts(40, (LCD_HEIGHT/2), BLACK, "el maximo saldo es 2,40 euros");
}

void plotPayInsufFailure()
{
	lcd_clear();
	lcd_puts_x2(16, (LCD_HEIGHT/2)-30, BLACK, "Saldo insuficiente");
	lcd_puts(40, (LCD_HEIGHT/2), BLACK, "el saldo minimo es 0,20 euros");
}
