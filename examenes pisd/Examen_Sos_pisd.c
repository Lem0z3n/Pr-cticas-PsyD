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
#define TICKS_PER_SEC   (100)
#define WAVETABLE_SIZE     (36)       // 16000sps / 440wps (LA) = 36 samples/wave

int16 square[WAVETABLE_SIZE] =
{
	900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,  900,
   -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900, -900
};

void poner_S(void);
void poner_O(void);


void main (void)
{
	sys_init();
    uart0_init();
    segs_init();
    leds_init();
    uda1341ts_init();
    iis_init( IIS_POLLING );


    while(1){
    	poner_S();
    	poner_O();
    	poner_S();

    	sw_delay_ms(delay_raya*3);
    }
}


void poner_O(void)
{
	uint8 i = 0;
	while(i < 3)
	{
    	leds_on();
    	segs_putsos(0);
    	uint8 j = 0;
    	timer3_start_timeout( delay_punto*10 );
    	while( !timer3_timeout())
		{
			iis_putSample( square[j], square[j] );
			if( ++j > WAVETABLE_SIZE )
				j = 0;
		}

    	leds_off();
    	sw_delay_ms(delay_punto);
    	i++;
	}

	sw_delay_ms(delay_raya);
}

void poner_S(void)
{
	uint8 i = 0;
	while(i < 3)
	{
    	leds_on();
    	segs_putsos(1);
    	uint8 j = 0;
    	timer3_start_timeout( delay_raya*10);
    	while( !timer3_timeout())
		{
			iis_putSample( square[j], square[j] );
			if( ++j > WAVETABLE_SIZE )
				j = 0;
		}

    	leds_off();
    	sw_delay_ms(delay_punto);
    	i++;
	}

	sw_delay_ms(delay_raya);
}






