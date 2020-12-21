// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#define _XTAL_FREQ 4000000		//Simulate a 4Ghz clock

#include <xc.h>

void blue_led();
void red_led();

//Interruptions declaration
void __interrupt() interrup0();

void main(void)
{
	// Set RD0 and RD1 as input or output
	TRISDbits.RD0 = 0;
	TRISDbits.RD1 = 0;
	
	// interruption config
	INTCONbits.GIE = 1; //set global interrup
	INTCONbits.INT0IE = 1;//enable interrup 0
	INTCONbits.INT0IF = 0;//set interrup flag to 0
	
	while (1){
		if (PORTBbits.RB1){
			red_led();
			__delay_ms(500);
		}
	}

	return;
}

void blue_led(){
	PORTDbits.RD0 = ~PORTDbits.RD0;
}

void red_led(){
	PORTDbits.RD1 = ~PORTDbits.RD1;
}

void __interrupt() interrup0(){
	if (INTCONbits.INT0IF){
		blue_led();
		__delay_ms(500);
		INTCONbits.INT0IF = 0;
	}
}

