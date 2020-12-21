/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"

// Defini��o das chamadas de sistema do kernel
u_int get_task_id(void);
void task_delay(u_int ms);
void task_create(u_int prior, task_ptr func);
void os_start(void);
void os_config(void);
void task_idle(void);
void task_yield(void);

// Rotinas auxiliares do kernel
u_int who(void);



#endif	/* KERNEL_H */

