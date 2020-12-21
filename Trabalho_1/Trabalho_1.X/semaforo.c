#include "semaforo.h"
#include "types.h"
#include "kernel.h"
#include "dispatcher.h"

extern fila_aptos_t f_aptos;

//Semaforo iniciation
void sem_init(sem_t *sem, int valor){
	DISABLE_INTERRUPTS();
	sem->contadorSem = valor;	//Number of task that acess that feature
	sem->total_tasks = valor;	//Number of task that acess that feature
	sem->fila.queue = 0;
	sem->fila.next_to_free = 0;
	ENABLE_INTERRUPTS();   
}

//Used to secure resources
void sem_wait(sem_t *sem){
	DISABLE_INTERRUPTS();
	//When semaphore value is zero it's no possible to use execute
	if (sem->contadorSem <=	 0) {
		//Save task position to restore
		sem->fila.tasks[sem->fila.queue] = f_aptos.task_running;
		sem->fila.queue = (sem->fila.queue + 1) % (MAX_TASKS - 1);
		//Save context that only semaphore post can restore
		SAVE_CONTEXT(WAITING_SEM);
		RESTORE_CONTEXT();
	}
	sem->contadorSem--;
	ENABLE_INTERRUPTS();      
}

//Used to free resources
void sem_post(sem_t *sem){
	DISABLE_INTERRUPTS();
	if (sem->contadorSem < 0) {
		f_aptos.ready_queue[sem->fila.tasks[sem->fila.next_to_free]].task_state = READY;   
		sem->fila.next_to_free = (sem->fila.next_to_free + 1) % (MAX_TASKS - 1);
	}
	if(sem->contadorSem < sem->total_tasks)
		sem->contadorSem++;
	ENABLE_INTERRUPTS();   
}