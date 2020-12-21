#ifndef CONFIG_H
#define	CONFIG_H

// Max number of tasks
#define MAX_TASKS           6
// RR quantum
#define QUANTUM             5
// Timer 0 prescaler setting
#define PRESCALER           0b001
// Timer 0 start
#define TIMER_START         6
#define ON                  1   
#define OFF                 0
// Enable RR scheduler
#define RR_SCHED            ON
// Enable priority scheduler
// Both enable will use priority with RR
#define PRIOR_SCHED         OFF
//Required to use timer
#define _XTAL_FREQ          4000000

#endif	/* CONFIG_H */

/* Total time = (timer bit size length - start time)*prescaler*machine period
 * timer bit size, T08BIT(256 or 65536)
 * start time, TMR0L(setable)
 * prescaler, T0PS
 * 111 = 1:256 Prescale value
 * 110 = 1:128 Prescale value
 * 101 = 1:64 Prescale value
 * 100 = 1:32 Prescale value
 * 011 = 1:16 Prescale value
 * 010 = 1:8 Prescale value
 * 001 = 1:4 Prescale value
 * 000 = 1:2 Prescale value
 * machine prediod, 4/cristal clock
 * 1ms = (256 - 6)*4*(4/4000000)
 * 1ms for each timer flag
 * 1000 timers flags equals 1 second
 * CHECK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */