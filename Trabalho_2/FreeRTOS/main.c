#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <xc.h>
#include "semphr.h"
#include "user_task.h"

SemaphoreHandle_t sem_igni;
SemaphoreHandle_t sem_motor;
QueueHandle_t pipe_igni;
QueueHandle_t pipe_error;
QueueHandle_t pipe_motor_error;

int main(void) {
	TRISEbits.TRISE1 = 1;
	TRISEbits.TRISE2 = 1;
	TRISEbits.TRISE3 = 1;
	TRISEbits.TRISE4 = 0;
	TRISEbits.TRISE5 = 0;
	TRISEbits.TRISE6 = 0;
	TRISEbits.TRISE7 = 0;
	TRISCbits.TRISC1 = 0;
	TRISCbits.TRISC2 = 0;
	TRISCbits.TRISC3 = 1;
	TRISCbits.TRISC4 = 1;

	sem_igni = xSemaphoreCreateBinary();
	sem_motor = xSemaphoreCreateBinary();
	pipe_igni = xQueueCreate(1, sizeof (unsigned int));
	pipe_error = xQueueCreate(1, sizeof (unsigned int));
	pipe_motor_error = xQueueCreate(1, sizeof (unsigned int));

	xTaskCreate(task_1, "task_1", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(task_2, "task_2", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(task_3, "task_3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(task_4, "task_4", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(task_5, "task_5", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(task_6, "task_6", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	vTaskStartScheduler();
	return 0;
}