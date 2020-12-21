// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
// Libraries
#include <xc.h>
// Task queue limit
#define TASK_COUNT 4
// Clock freq ???????????????????????????????????
#define _XTAL_FREQ 4000000

// Task Pointer
typedef void tarefa();

// Task state
typedef enum {
	READY = 0, 
	RUNNING, 
	WAITING
}state_t;

// Task control block
typedef struct TCB {
	// Task ID
	unsigned int task_ID;
	// Task priority
	unsigned int task_prior;
	// Task pointer
	tarefa *task_func;
	// Task state
	state_t task_state;
} tcb_t;

// Task queue
tcb_t task_list[TASK_COUNT];
int task_counter = 0;
int running_task = 0;

// Funtions
void create_task(unsigned int prioridade, tarefa *func);
void scheduler();

// Task declaration
void led_reset();
void red_led();
void blue_led();
void green_led();
void yellow_led();

//Interruptions declaration
void __interrupt() interrup0();

/*
* Configura??o de portas:
* 
* Registradores :
* 
* TRIS - define a dire??o dos dados.
* Se 0 ? saida, se 1 ? entrada. O padr?o ? 1
* 
* PORT - serve para fazer a leitura e a escrita de dados
* 
* LAT - serve para fazer a leitura e a escrtita de dados
* sempre mantem a informa??o sobre a ultima escrtita de dados
*/

//Variable declaration
/* Total time = (timer bit size length - start time)*prescaler*machine period
 * timer bit size, T08BIT(256 or 65536)
 * start time, TMR0L(setable)
 * prescaler, T0PS(2,4,8,16,32,128,256)
 * machine prediod, 4/cristal clock
 * 1ms = (256 - 6)*4*(4/4000000)
 * 1ms for each timer flag
 * 1000 timers flags equals 1 second
 * CHECK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
int quantum = 0;

void main(void) {
	// Setting I/O 
	TRISDbits.RD0 = 0;
	TRISDbits.RD1 = 0;
	TRISDbits.RD2 = 0;
	TRISDbits.RD3 = 0;

	// Interruption config
	INTCONbits.GIE = 1; //set global interruption
	INTCONbits.PEIE = 1; //set periferal interruption
	//Timer it's a periferal interruption
	INTCONbits.TMR0IE = 1;//enable timer 0 interruption
	INTCONbits.TMR0IF = 0;//set timer 0 interruption flag
	
	// Timer config
	T0CONbits.TMR0ON = 1;//enable timer
	T0CONbits.T08BIT = 1;//8 bit timer
	T0CONbits.T0CS = 0;//enable internal timer/clock
	T0CONbits.PSA = 1;//activate pre scale
	T0CONbits.T0PS = 0b001;//set pre scale
	TMR0L = 0x06;//Timer start with 6 counts

	// Create task
	//create_task(5, 1, red_led);
	create_task(5, blue_led);
	create_task(5, green_led);
	create_task(5, yellow_led);

	// SO logic
	while (1){
	}
	return;
}

// Funtions structures
void create_task(unsigned int prioridade, tarefa *func){
	task_list[task_counter].task_ID = task_counter+1;
	task_list[task_counter].task_prior = prioridade;
	task_list[task_counter].task_func = func;
	task_list[task_counter].task_state = READY;
	task_counter++;
}

void scheduler(){
	running_task = (running_task + 1) % task_counter;

	(*task_list[running_task++].task_func)();
}

//fowarding agent missing
//not necessary in this case
//fowarding agent run the task select by the scheduler

// Tasks definitions
void led_reset(){
	PORTDbits.RD0 = 0b0;
	PORTDbits.RD1 = 0b0;
	PORTDbits.RD2 = 0b0;
	PORTDbits.RD3 = 0b0;
}

void red_led(){
	led_reset();
	PORTDbits.RD0 = ~PORTDbits.RD0;
}

void blue_led(){
	led_reset();
	PORTDbits.RD1 = ~PORTDbits.RD1;
}

void green_led(){
	led_reset();
	PORTDbits.RD2 = ~PORTDbits.RD2;
}

void yellow_led(){
	led_reset();
	PORTDbits.RD3 = ~PORTDbits.RD3;	
}

void __interrupt() interrup0(){
	if (INTCONbits.TMR0IF && quantum == 3000){
		scheduler();
		TMR0L = 0x06;
		quantum = 0;
		INTCONbits.TMR0IF = 0;
	}
	else if(INTCONbits.TMR0IF){
		quantum++;
		TMR0L = 0x06;
		INTCONbits.TMR0IF = 0;
	}
}
