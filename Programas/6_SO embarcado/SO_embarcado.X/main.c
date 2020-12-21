/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <xc.h>
#include "types.h"
#include "kernel.h"
#include "tasks.h"
#include "config.h"
#include "mcu_config.h"
#include "mm.h"


// Fila de tarefas aptas a serem executadas
fila_aptos_t f_aptos;


void main(void)
{
	// Cria mem�ria heap
   SRAMInitHeap();
   
   // Configura o SO antes da execu��o
   os_config();
   
   // Criar as tarefas de usu�rio
   task_create(2, &task_1);
   task_create(2, &task_2);
   task_create(3, &task_3);

   // Ativa o escalonador
   os_start();

	task_idle();
   
   return;
}
