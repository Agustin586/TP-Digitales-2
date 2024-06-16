#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/mefServo.h"
#include "IncludesFiles/HCSR04.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"

extern void taskRtosPERIFERICOS_Servo(void *pvParameters) {
	PRINTF("Tarea: Manejo de Servo Motor\r\n");

	mefServo_init();

	for (;;) {
		mefServo();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_Sensor(void *pvParameters) {
	PRINTF("Tarea: Manejo de Sensor\r\n");

	mefSensor_init();

	for (;;) {
		mefSensor();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosPERIFERICOS_Hcsr04(void *pvParameters) {
	PRINTF("Tarea: Muestro del sensor");

	TickType_t xLastWakeTime;

	xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		if (!HCSR04_distanceReady() && !mefSensor_getDatoListo())
			TriggerPulse();
		else
			mefSensor_setDistancia(HCSR04_getDistance()), mefSensor_setDatoListo();

		vTaskDelayUntil(&xLastWakeTime, DELAY_20ms);
	}
	vTaskDelete(NULL);
	return;
}
