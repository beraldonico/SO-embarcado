/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <xc.h>

#include "timer.h"
#include "dispatcher.h"


extern fila_aptos_t f_aptos;
int quantum = QUANTUM;


void setup_timer_0()
{
   // Configura��o do timer 0
   INTCONbits.TMR0IE = 1;
   INTCONbits.PEIE   = 1;
   T0CONbits.T0CS    = 0;
   T0CONbits.PSA     = 0;
   T0CONbits.T0PS    = 0b001;
   TMR0L             = 6;
}

void controle_delay(void){
    int i;
    for (i = 0; i < f_aptos.ready_queue_size; i++) {
        if (f_aptos.ready_queue[i].task_state == WAITING_DELAY) {
            f_aptos.ready_queue[i].delay--;
            if (f_aptos.ready_queue[i].delay == 0) {
				f_aptos.ready_queue[i].task_state = READY;
            }
        }
    }
}

void __interrupt() IRQ_Timer0(){
	DISABLE_INTERRUPTS();
	//T0CONbits.TMR0ON = 0;
	if(INTCONbits.TMR0IF){
		#if RR_SCHED == ON
			quantum--;
			if(quantum == 0){
				SAVE_CONTEXT(READY);
				RESTORE_CONTEXT();
				quantum = QUANTUM;
			}
		#else
			SAVE_CONTEXT(READY);
			RESTORE_CONTEXT();
		#endif
		controle_delay();
		INTCONbits.TMR0IF = 0;    
		TMR0L = 6;
	}
	ENABLE_INTERRUPTS();
}
