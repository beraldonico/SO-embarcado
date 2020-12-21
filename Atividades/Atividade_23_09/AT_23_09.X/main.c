// Libraries
#include "pragma.h"
#include <xc.h>
#include "defines.h"
#include "variables.h"
#include "funtions.h"

void main() {
	// Interruption config
	INTCONbits.GIE = 1; //set global interruption
	INTCONbits.PEIE = 1; //set periferal interruption
	// Timer it's a periferal interruption
	INTCONbits.TMR0IE = 1;//enable timer 0 interruption
	INTCONbits.TMR0IF = 0;//set timer 0 interruption flag
	
	/* Flag time = (timer bit size length - start time) * prescaler * (4/cristal clock)
	 * Timer count = (Total time) / (Flag time)
	 * 
	 * timer bit size, T08BIT(256 or 65536)
	 * start time, TMR0L(setable)
	 * prescaler, T0PS(2,4,8,16,32,128,256)
	 * cristal clock 4000000
	 */
	// Timer 0 config
	T0CONbits.TMR0ON = 1;//Enable timer
	T0CONbits.T08BIT = TIMER_BITS;//Set timer BITS
	T0CONbits.T0CS = 0;//Enable internal timer/clock
	T0CONbits.T0SE = 1;//Timer increment on falling edge
	T0CONbits.PSA = 0;//Activate pre scale
	T0CONbits.T0PS = PRESCALER;//set pre scale
	// Timer 0 count
	TMR0L = START_TIME_L;
	TMR0H = START_TIME_H;
	
	// Capture/Compare/PWM (CCP)
	PR2 = 0b11111111; // Setting period
	CCPR1L = 0b00001111; // PWN low duty cycle
	CCP1CONbits.DC1B = 0b11; // PWN low duty cycle
	TRISCbits.RC2 = 0; // Setting PWM output
	
	// Timer 2 config (PWM)
	T2CONbits.T2CKPS = 0b10; // set prescale
	T2CONbits.TMR2ON = 0b1; // enable timer 2
	
	CCP1CONbits.CCP1M = 0b1100; // Enable PWM mode
	
	// SO logic
	while (1){
		
	}
	return;
}

void read_analog(void){
	analog = PORTAbits.AN0;
	PWM_calc();
}

void PWM_calc(void){
	active_per = analog / DIGITAL_OUT;
	flag_time = ( TIMER_SIZE - (START_TIME_H << 4 | START_TIME_L )) * PRESCALER * (4 / _XTAL_FREQ);
	pulse_time = active_per * (flag_time * PERIOD_COUNTER );
}

void __interrupt() interrup0(){
	if (INTCONbits.TMR0IF && quantum == FLAGS_COUNTER){
		read_analog();
		TMR0L = START_TIME_L;
		TMR0H = START_TIME_H;
		quantum = 0;
		INTCONbits.TMR0IF = 0;
	}
	else if (INTCONbits.TMR0IF && quantum % PERIOD_COUNTER == pulse_time){
		PORTDbits.RD0 = 1;
		TMR0L = START_TIME_L;
		TMR0H = START_TIME_H;
		quantum++;
		INTCONbits.TMR0IF = 0;
	}
	else if (INTCONbits.TMR0IF && quantum % PERIOD_COUNTER == 0){
		PORTDbits.RD0 = 0;
		TMR0L = START_TIME_L;
		TMR0H = START_TIME_H;
		quantum++;
		INTCONbits.TMR0IF = 0;
	}
	else if(INTCONbits.TMR0IF){
		quantum++;
		TMR0L = START_TIME_L;
		TMR0H = START_TIME_H;
		INTCONbits.TMR0IF = 0;
	}
}