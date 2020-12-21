#ifndef TYPES_H
#define	TYPES_H

#include <stdint.h>
#include <xc.h>
#include "config.h"

#define STACK_SIZE 20
#define IDLE        1

#define DISABLE_INTERRUPTS() INTCONbits.GIE     = 0
#define ENABLE_INTERRUPTS() INTCONbits.GIE      = 1

typedef unsigned char u_char;
typedef unsigned int u_int;

//Task state
typedef enum {READY = 0, RUNNING, WAITING_SEM, WAITING_DELAY, FINISHED} task_state_t;

//Task pointer
typedef void (*task_ptr)(void);

//TOS registers
typedef struct data_stack {
    u_char TOSU_reg;
    u_char TOSL_reg;
    u_char TOSH_reg;
} data_stack_t;

//Stack memory
typedef struct task_stack {
    data_stack_t stack[STACK_SIZE];    
    u_int size;
} task_stack_t;

//Task Control Block
typedef struct tcb {
    u_int task_id;
    task_state_t task_state;
    u_int task_prior;
    task_ptr task_func;
    task_stack_t task_stack;
    u_char bsr_reg;
    u_char w_reg;
    u_char status_reg;
    u_int delay;
} tcb_t;

//Task Queue 
typedef struct fila_aptos {
    tcb_t ready_queue[MAX_TASKS+IDLE];
    u_int ready_queue_size;
    u_int task_running;
} fila_aptos_t;

#endif	/* TYPES_H */
