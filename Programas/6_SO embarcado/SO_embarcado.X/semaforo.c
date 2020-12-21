#include "semaforo.h"
#include "types.h"
#include "kernel.h"
#include "dispatcher.h"

extern fila_aptos_t f_aptos;

// Implementação das rotinas de manipulação do semáforo
void sem_init(sem_t *sem, int valor)
{
   DISABLE_INTERRUPTS();
   
   // Inicializa o semáforo
   sem->contadorSem = valor;
   sem->fila.queue = 0;
   sem->fila.next_to_free = 0;
   
   ENABLE_INTERRUPTS();   
}

void sem_wait(sem_t *sem)
{
   DISABLE_INTERRUPTS();
   
   if (sem->contadorSem <=	 0) {
      // Bloqueia a tarefa que está tentando acessar o recurso,
      // pois não existem mais instâncias disponíveis
      sem->fila.tasks[sem->fila.queue] = who();
      sem->fila.queue = (sem->fila.queue + 1) % (MAX_TASKS - 1);
      // Salva o contexto da tarefa que ficou bloqueada no semáforo
      SAVE_CONTEXT(WAITING_SEM);
      // Coloca outra tarefa em execução
      RESTORE_CONTEXT();
   }
   sem->contadorSem--;
   
   ENABLE_INTERRUPTS();      
}

void sem_post(sem_t *sem)
{
   DISABLE_INTERRUPTS();
   
   if (sem->contadorSem < 0) {
      f_aptos.ready_queue[sem->fila.tasks[sem->fila.next_to_free]].task_state = READY;   
      sem->fila.next_to_free = (sem->fila.next_to_free + 1) % (MAX_TASKS - 1);
   }
   sem->contadorSem++;
   
   ENABLE_INTERRUPTS();   
}

int sem_get_value(sem_t sem)
{
   return sem.contadorSem;
}