#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <xc.h>

#define IGNITION_KEY		PORTEbits.RE1
#define DISCHARGING			PORTEbits.RE2
#define CHARGING			PORTEbits.RE3
#define LED_ERROR			PORTEbits.RE4
#define LED_CHARGING		PORTEbits.RE5
#define LED_DISCHARGING		PORTEbits.RE6
#define LED_IGNITION		PORTEbits.RE7
#define MOTOR_CHARGING		PORTCbits.RC1
#define MOTOR_DISCHARGING	PORTCbits.RC2
#define CHARGING_KEY		PORTCbits.RC3
#define DISCHARGING_KEY		PORTCbits.RC4

extern SemaphoreHandle_t sem_igni;
extern SemaphoreHandle_t sem_motor;
extern QueueHandle_t pipe_igni;
extern QueueHandle_t pipe_error;
extern QueueHandle_t pipe_motor_error;

//Task to start system
void task_1(){
	int status_igni;
	while(1){
		if (IGNITION_KEY == 0){
			LED_IGNITION = 1;
			status_igni = 1;
			xQueueSend(pipe_igni, (void*)&status_igni ,1);
		}
	}
}

//Task to stop system
void task_2(){
	int status_igni;
	while(1){
		if (IGNITION_KEY == 1) {
			LED_IGNITION = 0;
			MOTOR_DISCHARGING = 0;
			MOTOR_CHARGING = 0;
			status_igni = 0;
			xQueueSend(pipe_igni, (void*)&status_igni ,1);
		}
	}
}

//Error task
void task_3(){
	int status_igni;
	int status_error;
	int status_motor_error;
	while(1){
		xQueueReceive(pipe_igni, &status_igni, 1);
		xQueueReceive(pipe_motor_error, &status_motor_error, 1);
		if(status_igni == 1){
			if((DISCHARGING_KEY == 1 && CHARGING == 0) || (CHARGING_KEY == 1 && DISCHARGING == 0) || (status_motor_error)){
				LED_ERROR = 1;
				status_error = 1;
				xQueueSend(pipe_error, (void*)&status_error ,1);
			}
			else{
				LED_ERROR = 0;
				status_error = 0;
				xQueueSend(pipe_error, (void*)&status_error ,1);
			}
		}
		else{
			if(DISCHARGING_KEY == 1 || CHARGING_KEY == 1){
				LED_ERROR = 1;
				status_error = 1;
				xQueueSend(pipe_error, (void*)&status_error ,1);
			}
			else{
				LED_ERROR = 0;
				status_error = 0;
				xQueueSend(pipe_error, (void*)&status_error ,1);
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
	int status_igni;
	int status_error;
	while(1){
		if (DISCHARGING_KEY == 1 || CHARGING_KEY == 1){
			xQueueReceive(pipe_igni, &status_igni, 1);
			if (status_igni){
				if (DISCHARGING_KEY == 1){
					xSemaphoreTake(sem_motor, portMAX_DELAY);
					MOTOR_DISCHARGING = 1;
				}
				if (CHARGING_KEY == 1){
					xSemaphoreTake(sem_motor, portMAX_DELAY);
					MOTOR_CHARGING = 1;
				}
			}
			else{
				status_error = 1;
				xQueueSend(pipe_motor_error, (void*)&status_error ,1);
			}
		}
	}
}

//Stop motor
void task_6(){
	int status_motor_error; 
	while(1){
		if (DISCHARGING_KEY == 0 && CHARGING_KEY == 0){
			MOTOR_DISCHARGING = 0;
			MOTOR_CHARGING = 0;
			status_motor_error = 0;
			xQueueSend(pipe_motor_error, (void*)&status_motor_error ,1);
			xSemaphoreGive(sem_motor);
		}
	}
}