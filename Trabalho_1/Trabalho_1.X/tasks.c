#include <xc.h>
#include "tasks.h"
#include "kernel.h"
#include "pipe.h"
#include "semaforo.h"

sem_t sem_igni;
sem_t sem_motor;
pipe_t pipe_igni;
pipe_t pipe_error;
pipe_t pipe_motor_error;

#define IGNITION_KEY		PORTDbits.RD1
#define DISCHARGING			PORTDbits.RD2
#define CHARGING			PORTDbits.RD3
#define LED_ERROR			PORTDbits.RD4
#define LED_CHARGING		PORTDbits.RD5
#define LED_DISCHARGING		PORTDbits.RD6
#define LED_IGNITION		PORTDbits.RD7
#define MOTOR_CHARGING		PORTBbits.RB4
#define MOTOR_DISCHARGING	PORTBbits.RB5
#define CHARGING_KEY		PORTBbits.RB6
#define DISCHARGING_KEY		PORTBbits.RB7

void config_tasks(){
    TRISDbits.RD1 = 1;
    TRISDbits.RD2 = 1;
    TRISDbits.RD3 = 1;
    TRISDbits.RD4 = 0;
    TRISDbits.RD5 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 1;
    TRISBbits.RB7 = 1;
    sem_init(&sem_igni, 0);
    sem_init(&sem_motor, 0);
	pipe_create(&pipe_igni, 1);
	pipe_create(&pipe_error, 1);
	pipe_create(&pipe_motor_error, 1);
}

//Task to start system
void task_1(){
    while(1){
        if (IGNITION_KEY == 0){
			LED_IGNITION = 1;
			pipe_write(&pipe_igni, 1);
			sem_wait(&sem_igni);
        }
    }
}

//Task to stop system
void task_2(){
    while(1){
        if (IGNITION_KEY == 1) {
			LED_IGNITION = 0;
			MOTOR_DISCHARGING = 0;
			MOTOR_CHARGING = 0;
			pipe_write(&pipe_igni, 0);
			sem_post(&sem_igni);
        }
    }
}

//Error task
void task_3(){
	while(1){
		if(pipe_read(&pipe_igni)){
			if((DISCHARGING_KEY == 1 && CHARGING == 0) || (CHARGING_KEY == 1 && DISCHARGING == 0) || pipe_read(&pipe_motor_error)){
				LED_ERROR = 1;
				pipe_write(&pipe_error, 1);
			}
			else{
				LED_ERROR = 0;
				pipe_write(&pipe_error, 0);
			}
		}
		else{
			if(DISCHARGING_KEY == 1 || CHARGING_KEY == 1){
				LED_ERROR = 1;
				pipe_write(&pipe_error, 1);
			}
			else{
				LED_ERROR = 0;
				pipe_write(&pipe_error, 0);
			}
		}
	}
}

//Display motor start/stop
void task_4(){
    while(1){
		if (CHARGING == 0)
			LED_CHARGING = 1;
		if (CHARGING == 1)
			LED_CHARGING = 0;
		if (DISCHARGING == 0)
			LED_DISCHARGING = 1;
		if (DISCHARGING == 1)
			LED_DISCHARGING = 0;
	}
}

//Start motor
void task_5(){
    while(1){
		if (DISCHARGING_KEY == 1 || CHARGING_KEY == 1){
			if (pipe_read(&pipe_igni)){
				if (DISCHARGING_KEY == 1){
					MOTOR_DISCHARGING = 1;
					sem_wait(&sem_motor);
				}
				if (CHARGING_KEY == 1){
					MOTOR_CHARGING = 1;
					sem_wait(&sem_motor);
				}
			}
			else{
				pipe_write(&pipe_motor_error, 1);
				sem_wait(&sem_motor);
			}
		}
    }
}

//Stop motor
void task_6(){
    while(1){
		if (DISCHARGING_KEY == 0 && CHARGING_KEY == 0){
			MOTOR_DISCHARGING = 0;
			MOTOR_CHARGING = 0;
			pipe_write(&pipe_motor_error, 0);
			sem_post(&sem_motor);
        }
    }
}