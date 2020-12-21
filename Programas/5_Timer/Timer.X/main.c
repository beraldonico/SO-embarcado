/*
 * freq = clock / ciclo de maquina
 * tempo - (freq/clock) * 256(8bits)*prescaler
 * 
 * periodo - 1/freq*8 bits * prescaler
 * 
 * tempo_ms = 10 us *256*256 = 65.536
 * tempo_ms = 10 us *246*256 = 62.976
*/

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
   // Informa��es sobre registradores (pilha)
   // Ponteiro para a tarefa
   tarefa *task_func;
   
   // Representa��o do estado da tarefa
   state_t task_state;
} tcb_t;

// Fila de aptos
tcb_t FILA_APTOS[APTOS];
int controle_aptos = 0;
int tarefa_da_vez = 0;

// Cabe�alho das chamadas de sistema
void criar_tarefa(unsigned int prioridade, tarefa *func);
int escalonador();
void despachante();

// Definir tarefas
void task_1();
void task_2();
void task_3();

void __interrupt() trataTimer0();
int contador = 0;

void main(void) {
   // Configurar portas RD
   TRISDbits.RD0 = 0;
   TRISDbits.RD1 = 0;
   TRISDbits.RD2 = 0;
   
   //Config do timer0
   T0CONbits.T0CS = 0; //clock interno
   T0CONbits.PSA = 1; //pre scaler interno
   T0CONbits.T0PS = 0b010; //pre scaler interno
   INTCONbits.TMR0IE = 1;
   INTCONbits.TMR0IF = 0;
   INTCONbits.PEIE =1;
   INTCONbits.GIE =1;
   T0CONbits.TMR0ON = 1;
   TMR0L = 0;
   
   
   // Criar as tarefas
   criar_tarefa(5, task_1);
   criar_tarefa(5, task_2);
   criar_tarefa(5, task_3);
   
   // L�gica do "SO"
   while (1) {
      
      despachante();
      __delay_ms(10);
   }
   
   return;
}

// Cabe�alho das chamadas de sistema
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
   // Determina a tarefa que ir� executar
   int vez = escalonador();
   // Salava e restaura contexto
   // Coloca a tarefa em execu��o
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

void __interrupt() trataTimer0(){
	contador++;
	if(INTCONbits.TMR0IF && contador >= 200){
		despachante();
		INTCONbits.TMR0IF = 0;
		TMR0L = 0 ;
		contador = 0;
	}
}