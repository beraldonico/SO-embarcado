// CONFIG2H
#pragma config WDT = OFF		// Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF		// PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON		// MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#include <xc.h>

#define _XTAL_FREQ 4000000		//Simulate a 4Ghz clock

void red_led();

//Interruptions declaration
void __interrupt() interrup0();

void main(void)
{
	// Set RD0 as output
	TRISDbits.RD0 = 0;
	
	// interruption config
	INTCONbits.GIE = 1; //set global interruption
	INTCONbits.PEIE = 1; //set periferal interruption
	INTCONbits.TMR0IE = 1;//enable timer 0 interruption
	INTCONbits.TMR0IF = 0;//set timer 0 interruption flag
	
	//timer config
	T0CONbits.TMR0ON = 1;//enable timer
	T0CONbits.T08BIT = 1;//8bit timer
	T0CONbits.T0CS = 0;//enable internal timer/clock
	T0CONbits.PSA = 1;//activate pre scale
	T0CONbits.T0PS = 0b111;//set pre scale
	TMR0L = 0;
	
	while (1){
	}
	return;
}

void red_led(){
	PORTDbits.RD0 = ~PORTDbits.RD0;
}

void __interrupt() interrup0(){	
	if (INTCONbits.TMR0IF){
		red_led();
		INTCONbits.TMR0IF = 0;
		TMR0L = 0;
	}		
}

