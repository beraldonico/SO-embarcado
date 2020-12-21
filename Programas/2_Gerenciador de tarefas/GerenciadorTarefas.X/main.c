#include <xc.h>

#define APTOS 4

#define _XTAL_FREQ 4000000

// Ponteiro para a tarefa
typedef void tarefa();

// Estado da tarefa
typedef enum {READY = 0, RUNNING, WAITING} state_t;

// Task control block
typedef struct TCB {
   unsigned int task_ID;
   unsigned int task_prior;
   // Informações sobre registradores (pilha)
   // Ponteiro para a tarefa
   tarefa *task_func;
   
   // Representação do estado da tarefa
   state_t task_state;
} tcb_t;

// Fila de aptos
tcb_t FILA_APTOS[APTOS];
int controle_aptos = 0;
int tarefa_da_vez = 0;

// Cabeçalho das chamadas de sistema
void criar_tarefa(unsigned int prioridade, tarefa *func);
int escalonador();
void despachante();

// Definir tarefas
void task_1();
void task_2();
void task_3();


void main(void) {
   // Configurar portas RD
   TRISDbits.RD0 = 0;
   TRISDbits.RD1 = 0;
   TRISDbits.RD2 = 0;
   
   // Criar as tarefas
   criar_tarefa(5, task_1);
   criar_tarefa(5, task_2);
   criar_tarefa(5, task_3);
   
   // Lógica do "SO"
   while (1) {
      
      despachante();
      __delay_ms(10);
   }
   
   return;
}

// Cabeçalho das chamadas de sistema
void criar_tarefa(unsigned int prioridade, tarefa *func)
{
   FILA_APTOS[controle_aptos].task_ID = controle_aptos+1;
   FILA_APTOS[controle_aptos].task_func = func;
   FILA_APTOS[controle_aptos].task_state = READY;
   FILA_APTOS[controle_aptos].task_prior = prioridade;
   controle_aptos++;
}

int escalonador()
{
   tarefa_da_vez = (tarefa_da_vez+1) % controle_aptos;
   
   return tarefa_da_vez;
}

void despachante()
{
   // Determina a tarefa que irá executar
   int vez = escalonador();
   // Salava e restaura contexto
   // Coloca a tarefa em execução
   (*FILA_APTOS[vez].task_func)(); 
   
}

// Definir tarefas
void task_1()
{
   PORTDbits.RD0 = ~PORTDbits.RD0;
}

void task_2()
{
   PORTDbits.RD1 = ~PORTDbits.RD1;
}

void task_3()
{
   PORTDbits.RD2 = ~PORTDbits.RD2;
}

