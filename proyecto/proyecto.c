/*-------------------------------------------------------------------
**
**  Aplicacion demo bajo un kernel de planificacion no expropiativo

** Proyecto desarrollado por Guillermo Lemonnier y Lucas Arranz,
**
** basado en la demo inicial desarrollada por el profesor José Manuel Mendías Cuadros
**
**
**-----------------------------------------------------------------*/
#include "proyecto.h"
#include "pantalla.h"
/////////////////////////////////////////////////////////////////////////////
// Declaracion de recursos
/////////////////////////////////////////////////////////////////////////////

parking_t parking[MAXPLACES];

struct mbox1 {                /* mailbox donde la coinsAcceptorTask indica a mainTask la moneda introducida */
    boolean flag;
    boolean licPlateInput;
    uint8   cents;
    uint8  keyPress;
} kpPressedMsg;

struct mbox2 {                /* mailbox donde la tsScanTask indica a mainTask la posicion de la pantalla presionada */
    boolean flag, langFlag;
    uint16  x, y;
} tsPressedMsg;

struct mbox3 {                 /* mailbox donde la mainTask indica a coinsMoverTask si debe aceptar o devolver las monedas introducidas */
    boolean flag;
    boolean accept;
} coinsMoverMsg;


struct mbox5{
	boolean flag;	  /* mailbox donde la mainTask indica a ticketPrintTask que imprima un ticket */
	parking_t place;
}ticketMsg;



/*******************************************************************/

void main( void )

{
    
    sys_init();      
    leds_init();
    segs_init();
    uart0_init();
    rtc_init();
    timers_init();
    keypad_init();
    lcd_init();
    ts_init();

    lcd_on();
    ts_on();

    setup();

    scheduler_init();                       /* Inicializa el kernel */

    create_task( tsScanTask,          5 );    /* Crea las tareas de la aplicacion... */
    create_task( kpScanTask,   5 );    /* ... el kernel asigna la prioridad segun orden de creacion */
    create_task( mainTask,           10 );    /* ... las tareas mas frecuentes tienen mayor prioridad (criterio Rate-Monotonic-Scheduling) */
    create_task( coinsMoverTask,     10 );
    create_task( clockTask,          10 );
    create_task( ticketPrinterTask,  10 );

    timer0_open_tick( scheduler, TICKS_PER_SEC );  /* Instala scheduler como RTI del timer0  */

    while( 1 )
    {
        sleep();                /* Entra en estado IDLE, sale por interrupcion */
        dispacher();            /* Las tareas preparadas se ejecutan en esta hebra (background) en orden de prioridad */
    }

}

/*******************************************************************/

/* 
** Tarea principal, se activa cada 100 ms muestreando los mensajes enviados de otras tareas y actuando en consecuencia  
*/
void mainTask( void )  
{
    static boolean init = TRUE;
    static uint16 ticks, licPlateComp;
    static uint8 placeSelected;
    static rtc_time_t og_deadline;
    static state_t state, prevState;

    if( init )
    {
        init   = FALSE;
        init_screen_mens();
        plotWelcomeScreen();
        int i;
        for(i = 0; i < MAXPLACES; i++)
        {
        	parking[i].id = i+1;
        	parking[i].free = TRUE;
        	parking[i].licPlate = 0;
        	parking[i].credit = 0;
        	parking[i].error = 0;
        }
        placeSelected = 0; //	valor para inicializar la variable se sobreescribirá antes de ser relevante
        state = waiting;
    }
    else switch( state )
    {
    	case language:
    		if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                     /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */
			}
    		if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE;                 /* Marca el mensaje como leido */
				if( changeLang() >= 0)
				{									/* Borra pantalla */
					plotState(prevState, placeSelected);
					state = prevState;                  /* Salta al estado previo ... */
				}
				ticks = 500;                               /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
    		if ( tsPressedMsg.langFlag )
    		{
    			tsPressedMsg.flag = FALSE;
    			tsPressedMsg.langFlag = FALSE;
    			state = language;
    		}
			if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
			}
			break;
        case waiting:                        /* Estado en donde rechaza monedas y espera la pulsacion de la pantalla */
            if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
            {
                tsPressedMsg.flag = FALSE;                 /* Marca el mensaje como leido */
                plotState(selection,placeSelected);
                state = selection;                  /* Salta al estado selection ... */
                ticks = 1000;                               /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
            }
            if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = waiting;
				plotState(language, placeSelected);
				state = language;
			}
            if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
            {
                kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
                coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
                coinsMoverMsg.flag   = TRUE;
                ticks = 500;
            }    
            break;
        case selection:
        	if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                      /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */
			}
        	if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
			}
        	if( tsPressedMsg.flag && !tsPressedMsg.langFlag )
        	{
        		tsPressedMsg.flag = FALSE;                 /* Marca el mensaje como leido */
				if(parking[(placeSelected = selectorTask())].free) //si se selecciona una plaza disponible vamos al pago
				{
					rtc_gettime(&og_deadline);
					parking[placeSelected].deadline = og_deadline;	//establecer la fecha inicial de la plaza
					plotState(payment,placeSelected);
					state = payment;
					ticks = 500;

				} else if(placeSelected != 10){ // si no vamos a la pantalla de error

					parking[placeSelected].error = SELEC_ERROR;
					plotState(error,placeSelected);
					state = error;
					ticks = 300;
				} else {
					plotSelEnc();
				}
        	}
        	if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotLangSelScreen();
				state = language;
				ticks = 500;
			}
        	break;
        case payment:
        	if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
        		plotState(waiting,placeSelected);                         /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */
				parking[placeSelected].credit=0;
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
			}
        	if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag   = FALSE;           /* Marca el mensaje como leido */
				parking[placeSelected].credit += kpPressedMsg.cents;          /* Incrementa el saldo */
				ticks = 500;                              /* Restaura el tiempo maximo sin actividad que permanece en este estado */
				if(parking[placeSelected].credit > 240){
					uint16 excess;
					excess = parking[placeSelected].credit - 240;
					parking[placeSelected].credit = 240;
					plotExcessReturn(excess);
					coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
					coinsMoverMsg.flag   = TRUE;
				} else {
					coinsMoverMsg.accept = TRUE;              /* Envia un mensaje para que las monedas se acepten */
					coinsMoverMsg.flag   = TRUE;
				}
				updateDeadline(parking[placeSelected].credit,&parking[placeSelected].deadline); // Actualiza la fecha de salida.
				plotPayCredit(parking[placeSelected].credit);
				plotPayDeadline(parking[placeSelected].deadline);

			}
        	if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE;                /* Marca el mensaje como leido */
				if(parking[placeSelected].credit < 20){
					parking[placeSelected].credit = 0;
					parking[placeSelected].error = INSUF_ERROR;
					plotState(error,placeSelected);
					state = error;
					ticks = 300;
				} else {
					plotState(licPlateInput,placeSelected);
					state = licPlateInput;
					kpPressedMsg.licPlateInput = TRUE;
					parking[placeSelected].credit = 0;
					ticks = 500;
				}
			}
        	if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotState(language,placeSelected);
				state = language;
				ticks = 500;
			}
        	break;
        case licPlateInput:
        	kpPressedMsg.licPlateInput = TRUE;							//activa el modo matricula
			if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                          /* Visualiza pantalla inicial */
				parking[placeSelected].credit=0;
				kpPressedMsg.licPlateInput = FALSE;			//limpio el flag del keypress porque voy a cambiar de estado.
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
				state = waiting;                      /* Salta al estado demo_waiting */
			}
			if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;             						 /* Marca el mensaje como leido */
				if(getLicPlate(&parking[placeSelected].licPlate))
				{
					plotLicPlateUpdate(parking[placeSelected].licPlate);
				}
				ticks = 500;
			}
			if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE;                 /* Marca el mensaje como leido */
				parking[placeSelected].free = FALSE;
				ticketMsg.place = parking[placeSelected]; //imprime el ticket
				ticketMsg.flag = TRUE;
				kpPressedMsg.licPlateInput = FALSE;			//limpio el flag del keypress porque voy a cambiar de estado.
				state = success;      		 /* Salta al estado success */
				ticks = 500;

				if(!parking[placeSelected].licPlate){  //si hay un error en la matricula repite la operacion.
					state = licPlateInput;
					plotLicPlateEnc();
				} else
					plotState(success,placeSelected);  /* Visualiza pantalla de exito */;
			}
			if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotState(language,placeSelected);
				state = language;
				ticks = 500;
			}
			break;
        case success:
			if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                        /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */
			}
			if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
			}
			if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE;                 /* Marca el mensaje como leido */
				plotState(waiting,placeSelected);                                 /* Borra pantalla */
				state = waiting;                  /* Salta al estado selection ... */
				ticks = 500;                               /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
			if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotState(language,placeSelected);
				state = language;
				ticks = 500;
			}
			break;
        case error:
			if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                          /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */

			}
			if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
				coinsMoverMsg.accept = FALSE;              /* Envia un mensaje para que la moneda se devuelva */
				coinsMoverMsg.flag   = TRUE;
			}
			if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				tsPressedMsg.flag = FALSE;    /* Marca el mensaje como leido */
				if(parking[placeSelected].error == SELEC_ERROR && exitSol()){
					licPlateComp = 0;
					plotState(exit,placeSelected);
					state = exit;
				} else {
					plotState(waiting,placeSelected);                              /* vuelve al welcome screen*/
					state = waiting;
				}
				ticks = 500;                               /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
			if ( tsPressedMsg.langFlag )
			{
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotLangSelScreen();
				state = language;
			}
			break;
        case exit:
        	kpPressedMsg.licPlateInput = TRUE;							//activa el modo matricula
        	if( !(--ticks) )                           /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo maximo */
			{
				plotState(waiting,placeSelected);                          /* Visualiza pantalla inicial */
				state = waiting;                      /* Salta al estado demo_waiting */
				kpPressedMsg.licPlateInput = FALSE;			//limpio el flag del keypress porque voy a cambiar de estado.
			}
			if( kpPressedMsg.flag )                /* Chequea si se ha introducido una moneda (mensaje recibido de la tarea coinAcceptorTask) */
			{
				kpPressedMsg.flag = FALSE;              /* Marca el mensaje como leido */
				if(getLicPlate(&licPlateComp))
				{
					plotLicPlateUpdate(licPlateComp);
				}
				ticks=500;
			}
			if( tsPressedMsg.flag && !tsPressedMsg.langFlag )                    /* Chequea si se ha pulsado la pantalla (mensaje recibido de la tarea tsPressedTask) */
			{
				kpPressedMsg.licPlateInput = FALSE;			//limpio el flag del keypress porque voy a cambiar de estado.
				tsPressedMsg.flag = FALSE;
				if ( licPlateComp == parking[placeSelected].licPlate)
				{
					parking[placeSelected].credit = 0;
					parking[placeSelected].free = TRUE;
					parking[placeSelected].licPlate = 0;
					plotExitSuccess(parking[placeSelected]);            /* vuelve al welcome screen*/
					state = success;
				} else {
					parking[placeSelected].error = EXIT_ERROR;
					plotState(error,placeSelected);                              /* vuelve al welcome screen*/
					state = error;
				}
				ticks = 500;                               /* ... en el que debera permanecer un maximo de 500 ticks sin no hay actividad */
			}
			if ( tsPressedMsg.langFlag )
			{
				kpPressedMsg.licPlateInput = FALSE;			//limpio el flag del keypress porque voy a cambiar de estado.
				tsPressedMsg.flag = FALSE;
				tsPressedMsg.langFlag = FALSE;
				prevState = state;
				plotLangSelScreen();
				state = language;
			}
    }
}
/* 
** Emula el comportamiento de la impresora de tickets:
**   Cada segundo muestrea si ha recibido un mensaje de la tarea principal enviar a traves de la UART el texto del ticket del aparcamiento elegido
*/
void ticketPrinterTask( void )
{
   if(!ticketMsg.flag){
	   return;
   }
   ticketMsg.flag = FALSE;
   makeTicket(ticketMsg.place);
}

/* 
** Cada segundo visualiza la fecha/hora en la pantalla y libera aquellas plazas cuya hora de finalizacion haya pasado
*/
void clockTask( void )  
{
	plotTime();
}

/* 
** Cada 50 ms muestrea la touchscreen y envia un mensaje a la tarea principal con la posicion del lugar pulsado
*/
void tsScanTask( void )  
{
    static boolean init = TRUE;
    static enum { wait_keydown, scan, wait_keyup } state;
    
    if( init )
    {
        init  = FALSE;
        state = wait_keydown;
    }
    else switch( state )
    {
        case wait_keydown:                        /* Estado esperando la presion de la pantalla */
            if( ts_pressed() )                        /* Chequea la pantalla esta presionada */
                state = scan;                         /* Salta al estado scan */
            break;
        case scan:                                /* Estado en que escanea la pantalla */
            ts_getpos( &tsPressedMsg.x, &tsPressedMsg.y );    /* Lee la pantalla */
            tsPressedMsg.flag = TRUE;                 /* Envia un mensaje con el valor de la posicion presionada */
            if(tsPressedMsg.x > LCD_WIDTH-50 && tsPressedMsg.y < 50)
            	tsPressedMsg.langFlag = TRUE;
            state = wait_keyup;                       /* Salta al estado wait_keyup */
            break;
        case wait_keyup:                          /* Estado esperando la depresion de la pantalla  */
            if( !ts_pressed() )                       /* Chequea si la pantalla ya no esta presionada */
                state = wait_keydown;                 /* Salta al estado wait_keydown */
            break;
    }
}

/* 
** Emula el comportamiento de un reconocedor de monedas:
**   Cada 50 ms muestrea el keypad y envia un mensaje a la tarea principal con el valor de la moneda asociada a la tecla
*/
void kpScanTask( void )
{
    static boolean init = TRUE;
    static enum { wait_keydown, coinInput, licenseInput, wait_keyup } state;


    
    if( init )
    {
        init  = FALSE;
        state = wait_keydown;
    }
    else switch( state )
    {
        case wait_keydown:                        /* Estado esperando la presion teclas */
            if( keypad_pressed() && !kpPressedMsg.licPlateInput)                    /* Chequea si hay una tecla presionada */
                state = coinInput;                         /* Salta al estado scan */
            else if(keypad_pressed())
            	state = licenseInput;
            break;
        case coinInput:                                /* Estado en que escanea el teclado */
            switch( keypad_scan() )                   /* Lee el teclado */
            {
                case KEYPAD_KEY0:                         /* La tecla 0 esta asociada a la moneda de 0,10 euros */
                    kpPressedMsg.cents = 10;           /* Envia un mensaje con el valor de la moneda */
                    kpPressedMsg.flag  = TRUE;
                    break;
                case KEYPAD_KEY1:                         /* La tecla 1 esta asociada a la moneda de 0,20 euros */
				    kpPressedMsg.cents = 20;           /* Envia un mensaje con el valor de la moneda */
				    kpPressedMsg.flag  = TRUE;
				    break;
                case KEYPAD_KEY2:                         /* La tecla 2 esta asociada a la moneda de 0,50 euros */
					kpPressedMsg.cents = 50;           /* Envia un mensaje con el valor de la moneda */
					kpPressedMsg.flag  = TRUE;
					break;
                case KEYPAD_KEY3:                         /* La tecla 3 esta asociada a la moneda de 1,00 euros */
					kpPressedMsg.cents = 100;           /* Envia un mensaje con el valor de la moneda */
					kpPressedMsg.flag  = TRUE;
					break;
            }
            state = wait_keyup;                       /* Salta al estado wait_keyup */
            break;
		case licenseInput:
			kpPressedMsg.keyPress = keypad_scan();
		    state = wait_keyup;
		    kpPressedMsg.flag  = TRUE;
			break;
        case wait_keyup:                          /* Estado esperando la depresion de teclas  */
            if( !keypad_pressed() )                   /* Chequea si la tecla ya no esta presionada */
                state = wait_keydown;                     /* Salta al estado wait_keydown */
            break;
    }    
}



/* 
** Emula el comportamiento del dispositivo que envia las monedas a la alcancia o al cajetin de devolucion:
**   Cada 100 ms muestrea si ha recibido un mensaje de la tarea principal para mover monedas
**   Si van a la alcancia activa durante 1 s los leds y muestra una A en los segs
**   Si van al cajetin de devolucion activa durante 1 segundo los leds y muestra una D en los segs
*/
void coinsMoverTask( void )
{
    static boolean init = TRUE;
    static enum { off, on } state;    
    static uint32 ticks;
    
    if( init )
    {
        init  = FALSE;
        state = off;
    }
    else switch( state )
    {
        case off:                                /* Estado con leds y 7-segs apagados */
            if( coinsMoverMsg.flag )                /* Chequea si ha recibido mensaje */
            {
                coinsMoverMsg.flag = FALSE;            /* Marca el mensaje como leido */
                if( coinsMoverMsg.accept )             /* Muestra el mensaje que corresponda en segs */
                    segs_putchar( 10 );
                else
                    segs_putchar( 13 );
                led_on( LEFT_LED );                    /* Enciende leds */
                led_on( RIGHT_LED );
                state = on;                            /* Salta al estado on ... */
                ticks = 10;                            /* ... en el que debera permanecer 10 ticks */
            }
            break;
        case on:                                /* Estado con leds y 7-segs encendidos */
            if( !(--ticks) )                        /* Decrementa ticks y chequea si ha permanecido en este estado el tiempo requerido */
            {  
                segs_off();                            /* Apaga segs */
                led_off( LEFT_LED );                   /* Apaga leds */
                led_off( RIGHT_LED );                                
                state = off;                           /* Salta al estado off */
            }
            break;
    }
}    


int8 selectorTask(void)
{

	//PUESTO 1
	if(tsPressedMsg.x < LCD_WIDTH/4 && 64 < tsPressedMsg.y && tsPressedMsg.y < 128)
	{
		return 0;
	}
	//PUESTO 2
	else if(tsPressedMsg.x > LCD_WIDTH/4 && tsPressedMsg.x < LCD_WIDTH*2/4 &&  64 < tsPressedMsg.y && tsPressedMsg.y < 128)
	{
		return 1;

	}
	//PUESTO 3
	else if(tsPressedMsg.x > LCD_WIDTH*2/4 && tsPressedMsg.x < LCD_WIDTH*3/4 && 64 < tsPressedMsg.y && tsPressedMsg.y < 128)
	{
		return 2;

	}
	//PUESTO 4
	else if(tsPressedMsg.x > LCD_WIDTH*3/4 && 64 < tsPressedMsg.y && tsPressedMsg.y < 128)
	{
		return 3;

	}
	//PUESTO 5
	else if(tsPressedMsg.x > LCD_WIDTH/4 && tsPressedMsg.x < LCD_WIDTH*2/4 && 128 < tsPressedMsg.y && tsPressedMsg.y < 192)
	{
		return 4;

	}
	//PUESTO 6
	else if(tsPressedMsg.x > LCD_WIDTH*2/4 && tsPressedMsg.x < LCD_WIDTH*3/4 && 128 < tsPressedMsg.y && tsPressedMsg.y < 192)
	{
		return 5;

	}
	//PUESTO 7
	else if(tsPressedMsg.x < LCD_WIDTH*3/4 && 128 < tsPressedMsg.y && tsPressedMsg.y < 192)
	{
		return 6;

	}
	//PUESTO 8
	else if( tsPressedMsg.x > LCD_WIDTH*3/4 && 128 < tsPressedMsg.y && tsPressedMsg.y < 192 )
	{
		return 7;
	}
	return -1;

}


/*******************************************************************/

/*
** Inicializa flags, mailboxes y variables globales
*/
void setup( void )
{
    kpPressedMsg.flag  = FALSE;
    kpPressedMsg.licPlateInput = FALSE;
    kpPressedMsg.cents = 0;

    tsPressedMsg.flag     = FALSE;
    tsPressedMsg.langFlag = FALSE;
    tsPressedMsg.x        = 0;
    tsPressedMsg.y        = 0;

    coinsMoverMsg.flag    = FALSE;
    coinsMoverMsg.accept  = FALSE;

    ticketMsg.flag       = FALSE;
}

void plotState(state_t state, uint8 placeSelected)
{
	switch(state)
	{
	case language:
		plotLangSelScreen();
		break;
	case waiting:
		plotWelcomeScreen();
		break;
	case selection:
		plotSelectionScreen(parking);
		break;
	case payment:
		plotPaymentScreen(parking[placeSelected],parking[placeSelected].credit);
		break;
	case licPlateInput:
		plotLicPlateScreen(parking[placeSelected]);
		break;
	case success:
		plotSucScreen(parking[placeSelected]);
		break;
	case error:
		plotErrorScreen(parking[placeSelected]);
		break;
	case exit:
		plotLicPlateDemand();
		break;
	}
}

uint8 getMonthdays(uint8 month){
	uint8 totdays;
	if(month == 2)
		totdays = 28;
	else if((month < 8 && month%2) || (month <= 8 && !month%2))
		totdays = 31;
	else
		totdays = 30;

	return totdays;
}


void updateDeadline(uint16 credit, rtc_time_t * deadline)
{
	uint8 hours = credit/60;
	uint8 mins = credit%60;

	if((deadline->min += mins) >= 60){
		deadline->hour++;
		deadline->min %= 60;
	}
	if((deadline->hour += hours) >= 24){
		deadline->hour %= 24;
		deadline->mday++;
		deadline->wday++;
	}
	if(deadline->mday > getMonthdays(deadline->mon)){
		deadline->mon++;
		deadline->mday = 1;
	}
	if(deadline->mon > 12)
		deadline->year++;


}

uint8 getLicPlate(uint16 * licPlate)
{
	static uint8 licPlatePoint =0;

	if(kpPressedMsg.keyPress == KEYPAD_FAILURE)
		return 0;
	if(licPlatePoint == 4 && *licPlate != 0)
		return 0;
	else if(*licPlate == 0)
		licPlatePoint = 0;

	*licPlate |= ( kpPressedMsg.keyPress << (licPlatePoint*4)); //hacemos un OR entre licPlate y la tecla pulsada desplazada 0,4,8,12
	licPlatePoint++;
	return 1;
}

int8 changeLang()
{
	int8 lang;
	lang = -1;
	if(tsPressedMsg.x > LCD_WIDTH/4-24 && tsPressedMsg.x < LCD_WIDTH/4 + 24 && tsPressedMsg.y > 76 && tsPressedMsg.y < 126)
		lang = ESP;
	else if(tsPressedMsg.x > LCD_WIDTH*3/4-24 && tsPressedMsg.x < LCD_WIDTH*3/4 +24 && tsPressedMsg.y > 76 && tsPressedMsg.y < 126)
		lang = ENG;
	else if(tsPressedMsg.x > LCD_WIDTH/2-24 && tsPressedMsg.x < LCD_WIDTH/2 + 24 && tsPressedMsg.y > 146 && tsPressedMsg.y < 214)
		lang = LAT;

	if(lang >= 0)
		changeScreenLang(lang);

	return lang;
}

int8 exitSol()
{

	if(tsPressedMsg.x > LCD_WIDTH/2-80 && tsPressedMsg.x < LCD_WIDTH/2+80 && tsPressedMsg.y > LCD_HEIGHT-64 && tsPressedMsg.y < LCD_HEIGHT)
		return 1;
	return 0;
}


