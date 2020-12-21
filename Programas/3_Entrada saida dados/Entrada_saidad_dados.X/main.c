/*
 * File:   main.c
 * Author: nicol
 *
 * Created on 20 de Setembro de 2020, 10:15
 */


#include <xc.h>
#define  _XTAL_FREQ 4000000

void pisca_led();

void main(void) {
    //configurar a direação dos dados
    // 1 - input
    // 0 output
    TRISB = 0b11111110;
	//TRISB = 0xfe;
    TRISDbits.RD0 = 1;
	
    while (1){
		if(PORTDbits.RD0){
		pisca_led();
		}
		__delay_ms(100);
    };
    return;
}

void pisca_led(){
	 //escreve na porta RB0
    //RB0 = ~LATB0;
	PORTBbits.RB0 = ~LATBbits.LB0;
}