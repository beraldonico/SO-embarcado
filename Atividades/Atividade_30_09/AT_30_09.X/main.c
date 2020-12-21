/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <xc.h>
#include "types.h"
#include "kernel.h"
#include "tasks.h"
#include "config.h"
#include "pragma.h"


// Fila de tarefas aptas a serem executadas
fila_aptos_t f_aptos;

void main(void)
{
   os_config();
   
   // Criar as tarefas de usu�rio
   task_create(2, &task_1);
   task_create(2, &task_2);
   task_create(3, &task_3);

   // Ativa o escalonador
   os_start();

   
   while (1) {
      ;
   }
   
   return;
}
