#include <xc.h>

#include "tasks.h"
#include "kernel.h"
#include "semaforo.h"
#include "pipe.h"

sem_t sem_teste;
pipe_t pipe_teste;

void config_tasks()
{
	TRISDbits.RD0 = 0;
	TRISDbits.RD1 = 0;
	TRISDbits.RD2 = 0;
	TRISDbits.RD3 = 0;
	sem_init(&sem_teste, 0);
	pipe_create(&pipe_teste, 2);
}

void task_1()
{
	while (1) {
		PORTDbits.RD0 = ~PORTDbits.RD0;
		task_delay(500);
		sem_wait(&sem_teste);
	}
}

void task_2()
{
	while (1) {
		PORTDbits.RD1 = ~PORTDbits.RD1;
	}
}

void task_3()
{
	while (1) {
		PORTDbits.RD2 = ~PORTDbits.RD2;
		task_delay(1500);
		PORTDbits.RD2 = ~PORTDbits.RD2;
		sem_post(&sem_teste);
	}
}