/*
 * File:   main.c
 * Author: nicol
 *
 * Created on 20 de Setembro de 2020, 10:59
 */

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

#define _XTAL_FREQ 4000000

#include <xc.h>

void led_azul();
void led_vermelho();

//tratador de interrupção externa
void __interrupt(high_priority) trataint0();
void __interrupt(low_priority) trataint1();

void main(void)
{
	// Configura RD0 e RD1 como input
	// 0 out
	// 1 in
	TRISDbits.RD0 = 0;
	TRISDbits.RD1 = 0;
	
	// config da interrupção
	INTCON2bits.RBPU = 0;//resistor interrup in pull-up
	//INTCONbits.GIE = 1; global interrup
	INTCONbits.GIEH = 1;//high priority interrup
	INTCONbits.GIEL = 1;//low priority interrup
	INTCONbits.INT0IE = 1;//interrup 0 enable
	INTCONbits.INT0IF = 0;// flag interrup 0 
	INTCON2bits.INTEDG0 = 0; // falling edge interrup
	INTCON2bits.INTEDG1 = 0;
	INTCON3bits.INT1IE = 1;
	INTCON3bits.INT1IF = 0;
	INTCON3bits.INT1P = 0;
	
	while (1){
	}
	
	return;
}

void led_azul(){
	PORTDbits.RD0 = ~PORTDbits.RD0;
}

void led_vermelho(){
	PORTDbits.RD1 = ~PORTDbits.RD1;
}

void __interrupt(high_priority) trataint0(){
	if (INTCONbits.INT0IF){
		led_azul();
		INTCONbits.INT0IF = 0;
	}
}

void __interrupt(low_priority) trataint1(){
	if (INTCON3bits.INT1IF){
		led_vermelho();
		INTCON3bits.INT1IF = 0;
	}
}

