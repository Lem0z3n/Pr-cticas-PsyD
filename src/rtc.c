
#include <s3c44b0x.h>
#include <s3cev40.h>
#include <rtc.h>

extern void isr_TICK_dummy( void );

void rtc_init( void )
{
	TICNT   = 0x0;
	RTCALM  = 0x0;
	RTCRST  = 0x0;

	RTCCON  = 0x9;

	BCDYEAR = 0x25;
	BCDMON  = 0x5;
	BCDDAY  = 0x15;
	BCDDATE = 0x4;
	BCDHOUR = 0x19;
	BCDMIN  = 0x33;
	BCDSEC  = 0x12;

	ALMYEAR = 0x0;
	ALMMON  = 0x0;
	ALMDAY  = 0x0;
	ALMHOUR = 0x0;
	ALMMIN  = 0x0;
	ALMSEC  = 0x0;

	RTCCON &= 0;
}

void rtc_puttime( rtc_time_t *rtc_time )
{
    RTCCON |= 0x1;
    
    BCDYEAR = ((rtc_time->year/10)<<4) +rtc_time->year%10;
	BCDMON  = ((rtc_time->mon/10)<<4) +rtc_time->mon%10;
	BCDDAY  = ((rtc_time->mday/10)<<4) +rtc_time->mday%10;
	BCDDATE = rtc_time->wday;
	BCDHOUR = ((rtc_time->hour/10)<<4) +rtc_time->hour%10;
	BCDMIN  = ((rtc_time->min/10)<<4) +rtc_time->min%10;
	BCDSEC  = ((rtc_time->sec/10)<<4) +rtc_time->sec%10;

	RTCCON &= ~(1<<0);
}

void rtc_gettime( rtc_time_t *rtc_time )
{
	RTCCON |= 0x1;

	rtc_time->year = (BCDYEAR & 0xF)+ (BCDYEAR >>4)*10;
	rtc_time->mon  = (BCDMON & 0xF)+ (BCDMON >>4)*10;
	rtc_time->mday = (BCDDAY & 0xF)+ (BCDDAY >>4)*10;
	rtc_time->wday = BCDDATE;
	rtc_time->hour = (BCDHOUR & 0xF)+ (BCDHOUR >>4)*10;
	rtc_time->min  = (BCDMIN & 0xF)+ (BCDMIN >>4)*10;
	rtc_time->sec  = (BCDSEC & 0xF)+ (BCDSEC >>4)*10;
	if( ! rtc_time->sec ){
		rtc_time->year = (BCDYEAR & 0xF)+ (BCDYEAR >>4)*10;
		rtc_time->mon  = (BCDMON & 0xF)+ (BCDMON >>4)*10;
		rtc_time->mday = (BCDDAY & 0xF)+ (BCDDAY >>4)*10;
		rtc_time->wday = BCDDATE;
		rtc_time->hour = (BCDHOUR & 0xF)+ (BCDHOUR >>4)*10;
		rtc_time->min  = (BCDMIN & 0xF)+ (BCDMIN >>4)*10;
		rtc_time->sec  = (BCDSEC & 0xF)+ (BCDSEC >>4)*10;
	};

	RTCCON &= ~(1<<0);
}


void rtc_open( void (*isr)(void), uint8 tick_count )
{
	pISR_TICK = (uint32)isr;
	I_ISPC    = (1<<20);
	INTMSK   &= ~((1<<26)+(1<<20));
	TICNT     = (1<<7) +(tick_count);//&0x3f
}

void rtc_close( void )
{
	TICNT     = 0x0;
	INTMSK   |= ((1<<20));
	pISR_TICK = (uint32) isr_TICK_dummy;
}

void intToStr(int num, char* str, int len)
{
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
	}
}

char* rtc_stringDate (rtc_time_t rtc_time)
{
	char* wday;
		switch(rtc_time.wday){
			case 1:  wday = "dom";
				break;
			case 2:  wday = "lun";
				break;
			case 3:  wday = "mar";
				break;
			case 4:  wday = "mie";
				break;
			case 5:  wday = "jue";
				break;
			case 6:  wday = "vie";
				break;
			case 7:  wday = "sab";
				break;
		}
		char * date = "";

		char dayStr[2] = {'0', '0'};
		char monStr[2] = {'0', '0'};
		char yearStr[2] = {'0', '0'};
		char hourStr[2] = {'0', '0'};
		char minStr[2] = {'0', '0'};
		char secStr[2] = {'0', '0'};

		intToStr(rtc_time.mday, dayStr, 2);
		intToStr(rtc_time.mon, monStr, 2);
		intToStr(rtc_time.year, yearStr, 2);
		intToStr(rtc_time.hour, hourStr, 2);
		intToStr(rtc_time.min, minStr, 2);
		intToStr(rtc_time.sec, secStr, 2);

		// Construir la cadena final
		int index = 0;
		while (*wday) {
			date[index++] = *wday++;
		}
		date[index++] = ',';
		date[index++] = dayStr[0];
		date[index++] = dayStr[1];
		date[index++] = '/';
		date[index++] = monStr[0];
		date[index++] = monStr[1];
		date[index++] = '/';
		date[index++] = yearStr[0];
		date[index++] = yearStr[1];
		date[index++] = ' ';
		date[index++] = hourStr[0];
		date[index++] = hourStr[1];
		date[index++] = ':';
		date[index++] = minStr[0];
		date[index++] = minStr[1];
		date[index++] = ':';
		date[index++] = secStr[0];
		date[index++] = secStr[1];

	return date;
}


