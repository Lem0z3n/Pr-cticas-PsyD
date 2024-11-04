
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <pbs.h>
#include <timers.h>

extern void isr_PB_dummy( void );

void pbs_init( void )
{
    timers_init();
}

uint8 pb_scan( void )
{
    if(!(PDATG & PB_LEFT))
        return PB_LEFT;
    else if( !(PDATG & PB_RIGHT) )
        return PB_RIGHT;
    else
        return PB_FAILURE;
}

uint8 pb_pressed( void )
{
	return pb_scan() != PB_FAILURE;
}

uint8 pb_getchar( void )
{
	uint8 scancode;

	while( (pb_scan() == PB_FAILURE ));
	sw_delay_ms( PB_KEYDOWN_DELAY );

	scancode = pb_scan();

	while((pb_scan() != PB_FAILURE ));
	sw_delay_ms( PB_KEYUP_DELAY );

	return scancode;
}

uint8 pb_getchartime( uint16 *ms )
{
    uint8 scancode;
    
    while((pb_scan() == PB_FAILURE ) );
    timer3_start();
    sw_delay_ms( PB_KEYDOWN_DELAY );
    sw_delay_ms( PB_KEYDOWN_DELAY );
    
    scancode = pb_scan();
    
    while( (pb_scan() != PB_FAILURE ) );
    *ms = timer3_stop()/10;
    sw_delay_ms( PB_KEYUP_DELAY );

    return scancode;
}

uint8 pb_timeout_getchar( uint16 ms )
{
	uint8 scancode = PB_TIMEOUT;
	ms *= 10;
	timer3_start_timeout(ms);
    while((pb_scan() == PB_FAILURE ) && !timer3_timeout());
    if(ms == 0) return scancode;

    sw_delay_ms( PB_KEYDOWN_DELAY );

	scancode = pb_scan();

	while((pb_scan() != PB_FAILURE ) && !timer3_timeout());
	if(timer3_timeout()) return PB_TIMEOUT;
	sw_delay_ms( PB_KEYUP_DELAY );

	return scancode;
}

void pbs_open( void (*isr)(void) )
{
    pISR_PB   = (uint32) isr;
    EXTINTPND = 0x0;
	I_ISPC      = I_ISPC | (BIT_EINT4567) ;
	INTMSK     &= ~((BIT_EINT4567)|(BIT_GLOBAL));
}

void pbs_close( void )
{
    INTMSK  |= BIT_EINT4567;
    pISR_PB  = (uint32) isr_PB_dummy;
}

