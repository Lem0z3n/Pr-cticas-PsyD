#include <s3c44b0x.h>
#include <common_types.h>
#include <system.h>
#include <iis.h>
#include <segs.h>
#include <leds.h>
#include <timers.h>
#include <uart.h>
#include <uda1341ts.h>

#define delay_punto	(100)
#define delay_raya (delay_punto*3)
#define TICKS_PER_SEC   (16000)
#define WAVETABLE_SIZE     (36)       // 16000sps / 440wps (LA) = 36 samples/wave

uint16 duracion;


void iis_buzz_ms( uint16 ms );
void isr_tick( void ) __attribute__ ((interrupt ("IRQ")));

void main(void)
{
	sys_init();      /* Inicializa el sistema */
	timers_init();
	uda1341ts_init();
	iis_init( IIS_POLLING );

	uint16 milisecs = 50000;

	iis_buzz_ms(milisecs);


}


void isr_tick( void )
{
	static int16 sample = MAX_INT16;
	static uint8 alt = 10;

	iis_putSample(sample,sample);

	if(alt-- == 0){
		sample *= -1;
		alt = 10;
	}

	duracion--;
    I_ISPC = BIT_TIMER0;
}


void iis_buzz_ms( uint16 ms )
{
	duracion = ms * 160;
	timer0_open_tick( isr_tick, TICKS_PER_SEC );  /* Instala isr_tick como RTI del timer0  */
	while(duracion);
	timer0_close();
};

