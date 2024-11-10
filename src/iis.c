
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <iis.h>
#include <dma.h>

static void isr_bdma0( void ) __attribute__ ((interrupt ("IRQ")));

static uint8 iomode;

void iis_init( uint8 mode )
{
    iomode = mode;
    IISPSR  = ((7<<4)|(7<<0));

    if( iomode == IIS_POLLING )
    {
        IISMOD  = ((1<<0)|(1<<3)|(3<<6));
        IISFCON = ((1<<9)|(1<<8));
        IISCON  =  ((3<<0));
    }
    if( iomode == IIS_DMA )
    {
    	IISMOD  = ((1<<3)|(1<<0));
		IISFCON = (1<<9)|(1<<11)|(1<<8)|(1<<10);
		IISCON  = ((1<<1)|(1<<5)|(1<<4));
        bdma0_init();
        bdma0_open( isr_bdma0 );
    }
}

static void isr_bdma0( void )
{
    IISCON &= ~1;
    I_ISPC = BIT_BDMA0; 
}

inline void iis_putSample( int16 ch0, int16 ch1 )
{
    while( ((IISFCON & 0xf0) >> 4) > 6 );
    IISFIF = ch0;
    IISFIF = ch1;
}

inline void iis_getSample( int16 *ch0, int16 *ch1 )
{
	while( ((IISFCON & 0xf)) < 2 );
	*ch0 = IISFIF;
	*ch1 = IISFIF;
}

void iis_play( int16 *buffer, uint32 length, uint8 loop )
{
    uint32 i;
    int16 ch1, ch2;

    if( iomode == IIS_POLLING )
        for( i=0; i<length/2; )
        {
            ch1 = buffer[i++];
            ch2 = buffer[i++];
            iis_putSample( ch1, ch2 );
        }
    if( iomode == IIS_DMA )
    {
		while( IISCON & 1  )	;
		BDISRC0  = (1 << 28) | (uint32) buffer;
		BDIDES0  = (1 << 30) | (3 << 28) | (uint32) &IISFIF;
		BDCON0   = 0;
		BDICNT0  = ((loop==TRUE)? (1<<21)|(0<<22) : (0<<21|(3<<22))) | (0xfffff & length);
		BDICNT0 |= (1 << 20);

		IISMOD   = (IISMOD & ~(3 << 6)) | (2<<6);
		IISCON  |= (1<<0);
	}
}

void iis_rec( int16 *buffer, uint32 length )
{
    uint32 i;
    int16 ch1, ch2;

    if( iomode == IIS_POLLING )
    	 for( i=0; i<length/2; )
    	        {
    	            ch1 = buffer[i++];
    	            ch2 = buffer[i++];
    	            iis_getSample( ch1, ch2 );
    	        }
    if( iomode == IIS_DMA )
    {
        while( IISCON & 1  )	;
        BDISRC0  = (1 << 30) | (3 << 28) | (uint32) &IISFIF;
        BDIDES0  = (2 << 30) | (1 << 28) | (uint32) buffer;      
        BDCON0   = 0;
        BDICNT0  = (1 << 30) | (1 << 26) | (3 << 22) | (0xfffff & length); 
        BDICNT0 |= (1 << 20);

        IISMOD   = (IISMOD & ~(3 << 6)) | (1<<6);
		IISCON  |= (1<<0);
    }
}

void iis_pause( void )
{
	 IISCON &= ~(1);
}

void iis_continue( void )
{
	IISCON  |= (1);
}

uint8 iis_status( void )
{
	if((IISCON & 0x1)) return 1;
	return 0;
}

void iis_playWawFile( int16 *wav, uint8 loop )
{

    uint32 size;
    char *p;

    p = (char *) wav;
    while ( !(p[0] == 'd' && p[1] == 'a' && p[2] == 't' && p[3] == 'a') ) // busca el chunck data
        p++;
    p += 4;

    size = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24); // los datos de cabecera est�n en little-endian
    p += 4;

    iis_play( (int16 *)p, size, loop );

}