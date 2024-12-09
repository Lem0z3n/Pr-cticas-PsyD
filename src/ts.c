
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <timers.h>
#include <adc.h>
#include <lcd.h>
#include <ts.h>

#define PX_ERROR    (5)

static uint16 Vxmin = 0;
static uint16 Vxmax = 0;
static uint16 Vymin = 0;
static uint16 Vymax = 0;

static uint8 state;

extern void isr_TS_dummy( void );

static void ts_scan( uint16 *Vx, uint16 *Vy );
static void ts_calibrate( void );
static void ts_sample2coord( uint16 Vx, uint16 Vy, uint16 *x, uint16 *y );

void ts_init( void )
{
    timers_init();  
    lcd_init();
    adc_init();
    PDATE = (1 << 4)|(1 <<5)|(0 << 6)|(1 << 7) ; //Y- a GND
    sw_delay_ms( 1 );
    ts_on();
    ts_calibrate();
    ts_off();
}

void ts_on( void )
{
    adc_on();
    state = TS_ON;
}

void ts_off( void )
{
    adc_off();
    state = TS_OFF;
}

uint8 ts_status( void )
{
    return state;
}

uint8 ts_pressed( void )
{
	return !(PDATG & 1<<2);
}

static void ts_calibrate( void )
{
    uint16 x, y;
    
    lcd_on();
    do {    
        lcd_clear();
        lcd_puts(10,80,BLACK,"SOFTWARE DE CALIBRACION DE TOUCHSCREEN\n");
        lcd_puts(30,105,BLACK,"hecho por Lucas 'a secas' Arranz\n");
        lcd_puts(115,125,BLACK,"INTEGRAMENTE\n");

        lcd_draw_box(0,0,5,5, BLACK, 1);
        lcd_puts(15,5,BLACK,"<-- presione aqui\n");


        while( ! ts_pressed() );
        sw_delay_ms( TS_DOWN_DELAY );
        ts_scan( &Vxmin, &Vymax );
        while( ts_pressed() );
        sw_delay_ms( TS_UP_DELAY );

        lcd_draw_box(314,234,319,239, BLACK, 1);
        lcd_puts(200,210,BLACK,"presione aqui -->\n");
           
        while( ! ts_pressed() );
        sw_delay_ms( TS_DOWN_DELAY );
        ts_scan( &Vxmax, &Vymin );
        while( ts_pressed() );
        sw_delay_ms( TS_UP_DELAY );

        lcd_clear();
        lcd_draw_box((LCD_WIDTH/2)-3, (LCD_HEIGHT/2)-3,(LCD_WIDTH/2)+3, (LCD_HEIGHT/2)+3, BLACK,1);
        lcd_puts(100,140,BLACK,"presione aqui ^\n");

        while( ! ts_pressed() );
        sw_delay_ms( TS_DOWN_DELAY );
        ts_getpos( &x, &y );      
        while( ts_pressed() );
		sw_delay_ms( TS_UP_DELAY );

    
    } while( (x > LCD_WIDTH/2+PX_ERROR) || (x < LCD_WIDTH/2-PX_ERROR) || (y > LCD_HEIGHT/2+PX_ERROR) || (y < LCD_HEIGHT/2-PX_ERROR) );
    lcd_clear();
}

void ts_getpos( uint16 *x, uint16 *y )
{
    sw_delay_ms(1);
    sw_delay_ms(TS_DOWN_DELAY);

    uint16 Vx=0, Vy=0;
    ts_scan(&Vx,&Vy);

    sw_delay_ms(TS_UP_DELAY);

    ts_sample2coord(Vx,Vy,x,y);

}

void ts_getpostime( uint16 *x, uint16 *y, uint16 *ms )
{

	while(!ts_pressed());
	timer3_start();
	ts_getpos(x,y);
	while(ts_pressed());

	*ms = timer3_stop() / 10;
	sw_delay_ms( KEYPAD_KEYUP_DELAY );

}

uint8 ts_timeout_getpos( uint16 *x, uint16 *y, uint16 ms )
{
    ms *= 10;
    timer3_start_timeout(ms);

    uint16 og_x = *x;
    uint16 og_y = *y;
    do{

    	ts_getpos(x,y);

    }while(og_x == *x && og_y == *y && !timer3_timeout());

    return (og_x == *x && og_y == *y) ? TS_TIMEOUT : TS_OK;
}

static void ts_scan( uint16 *Vx, uint16 *Vy )
{
    PDATE = (0 << 4)| (1 << 5) |(1 << 6) | (0 <<7); //X- a GND, X+ a Vdd
    *Vx = adc_getSample( ADC_AIN1 );
    
    PDATE =  (1<<4) | (0<<5) | (0 <<6) | (1 << 7); //Y- a GND, Y+ a Vdd
    *Vy = adc_getSample( ADC_AIN0 );
    
    PDATE = (1 << 4)|(1 <<5)|(0 << 6)|(1 << 7); //Y- a GND
    sw_delay_ms( 1 );
}

static void ts_sample2coord( uint16 Vx, uint16 Vy, uint16 *x, uint16 *y )
{
    if( Vx < Vxmin )
        *x = 0;
    else if( Vx > Vxmax )
        *x = LCD_WIDTH-1;
    else 
        *x = LCD_WIDTH*(Vx-Vxmin) / (Vxmax-Vxmin);    

    if(Vy < Vymin)
    	*y = LCD_HEIGHT-1;
    else if( Vx > Vxmax)
    	*y = 0;
    else
    	*y =LCD_HEIGHT- LCD_HEIGHT*(Vy-Vymin) / (Vymax-Vymin);

}

void ts_open( void (*isr)(void) )
{
    pISR_TS = (uint32) isr;
    I_ISPC |= BIT_TS;
    INTMSK &= ~(BIT_GLOBAL | BIT_TS);
}

void ts_close( void )
{
	INTMSK |= (BIT_TS);
	pISR_TS  = (uint32) isr_TS_dummy;
}
