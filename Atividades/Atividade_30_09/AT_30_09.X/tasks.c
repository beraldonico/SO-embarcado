/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <xc.h>

#include "tasks.h"
#include "kernel.h"

void config_tasks()
{
   TRISDbits.RD0 = 0;
   TRISDbits.RD1 = 0;
   TRISDbits.RD2 = 0;
   TRISDbits.RD3 = 0;
}

void task_1()
{
   while (1) {
      PORTDbits.RD0 = ~PORTDbits.RD0;
      task_delay(500);
   }
}

void task_2()
{
   while (1) {
      PORTDbits.RD1 = ~PORTDbits.RD1;
      task_delay(300);
   }
}

void task_3()
{
   while (1) {
      PORTDbits.RD2 = ~PORTDbits.RD2;
      task_delay(100);
   }
}
