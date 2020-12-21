#include <xc.h>
#include "types.h"
#include "kernel.h"
#include "tasks.h"
#include "config.h"
#include "mcu_config.h"
#include "mm.h"

//Task queue declaration
fila_aptos_t f_aptos;

void main(void){
	//Start dinamic RAM
	SRAMInitHeap();
	//OS config
	os_config();

	//Create and add task to queue
	task_create(2, &task_1);
	task_create(2, &task_2);
	task_create(3, &task_3);
	task_create(3, &task_4);
	task_create(3, &task_5);
	task_create(3, &task_6);

	//start system clock
	os_start();

	//Idle task
	//Used to not go back to main func
	task_idle();

	return;
}