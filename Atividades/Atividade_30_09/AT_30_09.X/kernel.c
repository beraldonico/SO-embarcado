/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include "kernel.h"
#include "dispatcher.h"
#include "timer.h"
#include "tasks.h"
 
extern fila_aptos_t f_aptos;

uint8_t get_task_id()
{
   return f_aptos.ready_queue[f_aptos.task_running].task_id;
}

void task_delay(u_int ms)
{
   // Desabilitar interrup��es globais
   DISABLE_INTERRUPTS();
   
   // Chamar o despachante para salvar o contexto da terefa que est�
   // executando
   SAVE_CONTEXT(WAITING_DELAY);
   
   // Define o tempo de espera   
   f_aptos.ready_queue[f_aptos.task_running].delay = ms;
   
   // Restaura o contexto da tarefa que entrar� em execu��o
   RESTORE_CONTEXT();
}

void task_create(u_int prior, task_ptr func)
{
   tcb_t new_task;
   
   new_task.task_id = f_aptos.ready_queue_size + 1;
   new_task.task_prior = prior;
   new_task.task_state = READY;
   new_task.task_func = func;
   new_task.task_stack.size = 0;
   
   // Incui a tarefa na fila de aptos
   f_aptos.ready_queue[f_aptos.ready_queue_size+1] = new_task;
   f_aptos.ready_queue_size++;
}

void os_start()
{   
   // Coloca o SO em execu��o   

   // Habilita as interrup��es globais   
   ENABLE_INTERRUPTS();

   // Inicia o TIMER0
   T0CONbits.TMR0ON = 1;   
}

void os_config()
{
   __asm("GLOBAL _task_idle, _task_1, _task_2, _task_3");

   
   // Configura as estruturas de dados do kernel   
   f_aptos.task_running       = 0;
   f_aptos.ready_queue_size   = 0;
   
   // Configura o timer
   setup_timer_0();
   
   // Cria a tarefa IDLE
   task_create(0, &task_idle);
   
   // Chama a rotina para configurar as tarefas
   config_tasks();
}

void task_idle()
{
   while (1) {
      //Nop();
      PORTDbits.RD3 = ~PORTDbits.RD3;
   }
}

u_int who()
{
   return f_aptos.task_running;
}