#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/mefServo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"

extern void taskRtosPERIFERICOS_Servo(void) {
	PRINTF("Tarea: Manejo de Servo Motor\r\n");

	mefServo_init();

	for (;;) {
		mefServo();

		vTaskDelay(DELAY_100ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_Sensor(void) {
	PRINTF("Tarea: Manejo de sensor\r\n");

	mefSensor_init();

	for (;;) {
		mefSensor();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_delay(void) {
	vTaskDelay(DELAY_500ms);

	return;
}
