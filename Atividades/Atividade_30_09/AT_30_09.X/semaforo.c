/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include "semaforo.h"
#include "types.h"
#include "kernel.h"
#include "dispatcher.h"

// Implementa��o das rotinas de manipula��o do sem�foro
void sem_init(sem_t *sem, u_int valor)
{
   DISABLE_INTERRUPTS();
   
   // Inicializa o sem�foro
   sem->contadorSem = valor;
   sem->fila.queue_size = 0;
   sem->fila.next_to_free = 0;
   
   ENABLE_INTERRUPTS();   
}

void sem_wait(sem_t *sem)
{
   DISABLE_INTERRUPTS();
   
   sem->contadorSem--;
   if (sem->contadorSem < 0) {
      // Bloqueia a tarefa que est� tentando acessar o recurso,
      // pois n�o existem mais inst�ncias dispon�veis
      sem->fila.tasks[sem->fila.queue_size] = who();
      sem->fila.queue_size++;      
      // Salva o contexto da tarefa que ficou bloqueada no sem�foro
      SAVE_CONTEXT(WAITING_SEM);
      // Coloca outra tarefa em execu��o
      RESTORE_CONTEXT();      
   }
   
   ENABLE_INTERRUPTS();      
}

void sem_post(sem_t *sem)
{
   DISABLE_INTERRUPTS();
   
   sem->contadorSem++;
   if (sem->contadorSem <= 0) {
      f_aptos.ready_queue[sem->fila.next_to_free].task_state = READY;   
      sem->fila.next_to_free = (sem->fila.next_to_free + 1) % MAX_TASKS; 
      sem->fila.queue_size--; 
   }
   
   ENABLE_INTERRUPTS();   
}

int sem_get_value(sem_t sem)
{
   return sem.contadorSem;
}

