#ifndef DISPATCHER_H
#define	DISPATCHER_H

#include "types.h"
#include "scheduler.h"
#include <xc.h>

extern fila_aptos_t f_aptos;
u_int index;

//Save task context and desire state
#define SAVE_CONTEXT(state) { \
    index = 0; \
    f_aptos.ready_queue[f_aptos.task_running].task_state        = state; \
    f_aptos.ready_queue[f_aptos.task_running].w_reg             = WREG; \
    f_aptos.ready_queue[f_aptos.task_running].bsr_reg           = BSR; \
    f_aptos.ready_queue[f_aptos.task_running].status_reg        = STATUS; \
    while (STKPTR) { \
        f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index].TOSU_reg = TOSU; \
        f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index].TOSL_reg = TOSL; \
        f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index].TOSH_reg = TOSH; \
        index++; \
        __asm("POP"); \
    } \
    f_aptos.ready_queue[f_aptos.task_running].task_stack.size = index; \
}

//Restore task context and execute
#define RESTORE_CONTEXT(void) { \
    f_aptos.task_running = scheduler(); \
    STKPTR  = 0; \
    if (f_aptos.ready_queue[f_aptos.task_running].task_stack.size == 0) { \
        __asm("PUSH"); \
        TOS = f_aptos.ready_queue[f_aptos.task_running].task_func; \
		f_aptos.ready_queue[f_aptos.task_running].task_state = RUNNING; \
    } \
    else { \
        f_aptos.ready_queue[f_aptos.task_running].task_state = RUNNING; \
        WREG    = f_aptos.ready_queue[f_aptos.task_running].w_reg; \
        BSR     = f_aptos.ready_queue[f_aptos.task_running].bsr_reg; \
        STATUS  = f_aptos.ready_queue[f_aptos.task_running].status_reg; \
        index   = f_aptos.ready_queue[f_aptos.task_running].task_stack.size; \
        while(index) { \
            __asm("PUSH"); \
            TOSU = f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index - 1].TOSU_reg; \
            TOSL = f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index - 1].TOSL_reg; \
            TOSH = f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index - 1].TOSH_reg; \
            index--; \
        } \
    } \
}

#endif	/* DISPATCHER_H */

