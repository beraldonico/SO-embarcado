/*
 * freq = clock / ciclo de maquina
 * tempo - (freq/clock) * 256(8bits)*prescaler
 * 
 * periodo - 1/freq*8 bits * prescaler
 * 
 * tempo_ms = 10 us *256*256 = 65.536
 * tempo_ms = 10 us *246*256 = 62.976
*/
#include "pragma.h"

#define _XTAL_FREQ 4000000

void __interrupt() trataTimer0();
int contador = 1000;

void main(void) {
   // Configurar portas RD
   TRISDbits.RD0 = 0;
   TRISDbits.RD1 = 0;
   
   INTCONbits.GIE = 1;
   INTCONbits.PEIE = 1;
   INTCONbits.TMR0IE = 1;
   INTCONbits.TMR0IF = 0;
   
   T0CONbits.TMR0ON = 0;
   T0CONbits.T08BIT = 1;
   T0CONbits.T0CS = 0;
   T0CONbits.PSA = 0;
   T0CONbits.T0PS = 0b001;
   TMR0L = 6;
   TMR0H = 0;
   T0CONbits.TMR0ON = 1;
   
   // L�gica do "SO"
   while (1) {
      
   }
   
   return;
}

void __interrupt() trataTimer0(){
	if ( INTCONbits.TMR0IF == 1){
		PORTDbits.RD1 = ~PORTDbits.RD1;
		contador--;
		if(contador == 0){
			PORTDbits.RD0 = ~PORTDbits.RD0;
			contador = 1000;
		}
		INTCONbits.TMR0IF = 0;
		TMR0L = 6;
	    PORTDbits.RD1 = ~PORTDbits.RD1;
	}
}