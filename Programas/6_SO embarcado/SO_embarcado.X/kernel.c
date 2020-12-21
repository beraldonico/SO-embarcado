/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include "kernel.h"
#include "dispatcher.h"
#include "timer.h"
#include "tasks.h"
#include "types.h"
#include "scheduler.h"

u_int index;
extern fila_aptos_t f_aptos;

// Chamadas de sistema

/**
 * Retorna a identifica��o da tarefa
 */
u_int get_task_id()
{
   return f_aptos.ready_queue[f_aptos.task_running].task_id;
}

/**
 * Coloca a tarefa que atualmente est� executando em estado de espera
 */
void task_delay(u_int ms)
{
	DISABLE_INTERRUPTS();
	SAVE_CONTEXT(WAITING_DELAY)
	
   // Define o tempo de espera   
   f_aptos.ready_queue[f_aptos.task_running].delay = ms;
   
   // Restaura o contexto da tarefa que entrar� em execu��o
	RESTORE_CONTEXT();
	ENABLE_INTERRUPTS();
}

/**
 * Cria uma tarefa no SO. Os par�metros necess�rios s�o a prioridade e o ponteiro para a fun��o da tarefa
 */
void task_create(u_int prior, task_ptr func)
{
   tcb_t new_task;
   
   new_task.task_id = f_aptos.ready_queue_size + 1;
   new_task.task_prior = prior;
   new_task.task_state = READY;
   new_task.task_func = func;
   new_task.task_stack.size = 0;
   
   // Incui a tarefa na fila de aptos
   f_aptos.ready_queue[f_aptos.ready_queue_size] = new_task;
   f_aptos.ready_queue_size++;
}

/**
 * Inicia a execu��o do SO. Inicia o perif�rico do TIMER
 */
void os_start(void){
    ENABLE_INTERRUPTS();
	// Reset timer count
    TMR0L = 6;
    INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 1;
}

/**
 * Configura o SO antes de sua execu��o
 */
void os_config()
{
   __asm("GLOBAL _task_idle, _task_1, _task_2, _task_3");

   
	T0CONbits.TMR0ON = 0;
    // Configura as estruturas de dados do kernel
    f_aptos.task_running = 0;
    f_aptos.ready_queue_size   = 0;
    // Set Timer
    setup_timer_0();
    // Create IDLE task
    task_create(0, &task_idle);
    // Set tasks created by user
    config_tasks();
}

/**
 * Tarefa IDLE - � executada quando n�o h� nenhuma outra tarefa no estado de pronto na fila de aptos
 */
void task_idle()
{
   while (1) {
      //Nop();
      PORTDbits.RD3 = ~PORTDbits.RD3;
   }
}

/**
 * Faz com que a tarefa libere o processador voluntariamente
 */
void task_yield()
{
   DISABLE_INTERRUPTS();
   SAVE_CONTEXT(READY);
   RESTORE_CONTEXT();
   ENABLE_INTERRUPTS();
}

// Rotinas auxiliares do kernel
u_int who()
{
   return f_aptos.task_running;
}
