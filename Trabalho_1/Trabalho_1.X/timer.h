#ifndef TIMER_H
#define	TIMER_H

#include "types.h"

void setup_timer_0(void);
void __interrupt() IRQ_Timer0();
void controle_delay(void);

#endif	/* TIMER_H */

