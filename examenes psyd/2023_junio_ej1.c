#include <s3c44b0x.h>
#include <s3cev40.h>
#include <common_types.h>
#include <system.h>
#include <uart.h>
#include <timers.h>

#define MAXLEN (100)

char mailbox [MAXLEN];

void isr_ms( void ) __attribute__ ((interrupt ("IRQ")));


boolean mailboxflag = FALSE;


void main(void)
{
	sys_init();      /* Inicializa el sistema */
	timers_init();
	uart0_init();

	timer0_open_ms(isr_ms,20,TIMER_INTERVAL );

	while(1){
		if(mailboxflag){
			uart0_puts(mailbox);
			mailboxflag = FALSE;
		}
	}

}


void isr_ms()
{
	static char *mail = mailbox;
	char ch;
	 if( UFSTAT0 & 0xF )
		{
			ch = URXH0;
			*mail++ = ch;
			if( ch == '\n' )
			{
				*mail = '\0';
				mail = mailbox;
				mailboxflag = TRUE;
			}
		}
		I_ISPC = BIT_TIMER0;
}
