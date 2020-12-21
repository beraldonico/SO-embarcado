#include "scheduler.h"
#include "config.h"
#include "dispatcher.h"
#include "kernel.h"

extern fila_aptos_t f_aptos;

u_int scheduler(){
#if PRIOR_SCHED == OFF
   return rr_scheduler();
#else
   return prior_scheduler();
#endif
}
 
u_int rr_scheduler(){
   u_int next_task = f_aptos.task_running;
	do {
		next_task += 1;
		if(next_task >= f_aptos.ready_queue_size){
			next_task = 0;
			break;
		}
	} while (f_aptos.ready_queue[next_task].task_state != READY);
	return next_task;
}

u_int prior_scheduler(){
    u_int tmp_task = 0, next_task = 0;
	u_int prior = 0;
	do {
		if(f_aptos.ready_queue[tmp_task].task_prior > prior && f_aptos.ready_queue[tmp_task].task_state != READY){
			next_task = tmp_task;
			prior = f_aptos.ready_queue[tmp_task].task_prior;
		}
		tmp_task++;
	} while (tmp_task < f_aptos.ready_queue_size);
    return next_task;
}