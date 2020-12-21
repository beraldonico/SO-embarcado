#ifndef SEMAFORO_H
#define	SEMAFORO_H

#include "config.h"
#include "types.h"

//Semaphore waiting queue
typedef struct fila_sem {
    u_int tasks[MAX_TASKS - 1];
    u_int queue;
    u_int next_to_free;
} sem_queue_t;

//Semaphore data type
typedef struct semaforo {
    int contadorSem;
    int total_tasks;
    sem_queue_t fila; 
} sem_t;

void sem_init(sem_t *sem, int valor);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);
int sem_get_value(sem_t sem);

#endif	/* SEMAFORO_H */