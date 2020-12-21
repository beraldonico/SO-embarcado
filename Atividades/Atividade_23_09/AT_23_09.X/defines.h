#ifndef DEFINES_H
#define	DEFINES_H

//Defines
// Clock freq (????????????)
#define _XTAL_FREQ 4000000
// Timer start count
// 62500 bits left
#define START_TIME_L 0b11011100 // 8 low bits
#define START_TIME_H 0b00001011 // 8 high bits
// Flags counter
#define FLAGS_COUNTER 1000 // 1 second in this settings
// Pre Scaler
#define PRESCALER 0b011 // Set pre scale to 16
// Set timer size (1 = 8 bits, 0 = 16 bits)
#define TIMER_BITS 0 //
// Timer size(don't change!!!)
#if TIMER_BITS == 0
	#define TIMER_SIZE 65536
#elif
	#define TIMER_SIZE 256
#endif
// PWM
// Number of LED to control
#define LED_NUM 10
// Confirm digital voltage output
#define DIGITAL_OUT 5
// Flag counter to set pulse period
#define PERIOD_COUNTER 10

#endif	/* DEFINES_H */

