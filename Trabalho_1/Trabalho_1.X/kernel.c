#include "kernel.h"
#include "dispatcher.h"
#include "timer.h"
#include "tasks.h"
#include "types.h"
#include "scheduler.h"

u_int index;
extern fila_aptos_t f_aptos;

//Task delay in ms, NOT EXACT ms count
void task_delay(u_int ms){
	DISABLE_INTERRUPTS();
	SAVE_CONTEXT(WAITING_DELAY)

	f_aptos.ready_queue[f_aptos.task_running].delay = ms;

	RESTORE_CONTEXT();
	ENABLE_INTERRUPTS();
}

//Alloc task on tasks queue with state READY
void task_create(u_int prior, task_ptr func){
   tcb_t new_task;
   
   new_task.task_id = f_aptos.ready_queue_size + 1;
   new_task.task_prior = prior;
   new_task.task_state = READY;
   new_task.task_func = func;
   new_task.task_stack.size = 0;
   
   f_aptos.ready_queue[f_aptos.ready_queue_size] = new_task;
   f_aptos.ready_queue_size++;
}

//Start OS
void os_start(void){
	ENABLE_INTERRUPTS();	//Enable interruptios
	TMR0L = TIMER_START;				//Set timer start time
	INTCONbits.TMR0IF = 0;	//Reset timer flag
	T0CONbits.TMR0ON = 1;	//Start timer
}

//OS configuration
void os_config(){
	//Write your tasks name as "_<task name>"
	__asm("GLOBAL _task_idle, _task_1, _task_2, _task_3, _task_4, _task_5, _task_6");

	T0CONbits.TMR0ON = 0;			//Stop timer count
	f_aptos.task_running = 0;
	f_aptos.ready_queue_size = 0;
	setup_timer_0();
	task_create(0, &task_idle);
	config_tasks();
}

//Idle task
void task_idle(){
	while (1) {
		Nop();
	}
}

//Stop task running and start other
void task_yield(){
	DISABLE_INTERRUPTS();
	SAVE_CONTEXT(READY);
	RESTORE_CONTEXT();
	ENABLE_INTERRUPTS();
}