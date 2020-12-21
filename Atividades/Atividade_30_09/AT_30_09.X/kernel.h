/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"

// Defini��o das chamadas de sistema do kernel
uint8_t get_task_id();
void task_delay(u_int ms);
void task_create(u_int prior, task_ptr func);
void os_start();
void os_config();
void task_idle();
u_int who();



#endif	/* KERNEL_H */

