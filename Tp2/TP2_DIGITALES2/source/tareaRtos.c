#include "tareasRtos.h"
#include "MACROS.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "display.h"
#include "mefSEC.h"
#include "queue.h"

#define FREQ_FRAME_RATE_DISPLAY(x) pdMS_TO_TICKS(1/x)	// En Hz
#define QUEUE_LONGITUD	10

volatile bool Int_Freefall;

static xQueueHandle queue_NormaMaxima;
extern xSemaphoreHandle FreefallSemaphore,DrydSemaphore;

extern xQueueHandle tareasRtos_getQueue_Norma(void){
	return queue_NormaMaxima;
}

extern bool tareasRtos_getEst_IntFreefall(void){
	return Int_Freefall;
}

extern void tareasRtos_reset_IntFreefall(void){
	Int_Freefall = 0;

	return;
}

extern void tareasRtos_TaskMEF(void *pvparameters){
	PRINTF("Se creo la tarea MEF\r\n");
	mefSEC_init();

	queue_NormaMaxima = xQueueCreate(QUEUE_LONGITUD, sizeof(uint32_t));

	for (;;) {
		mefSEC();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);
}

extern void tareasRtos_TaskDisplay(void *pvparameters){
	PRINTF("Se creo la tarea Display\r\n");
	/* Modulo spi */
	board_configSPI0();

	/* Modulo pantalla oled */
	oled_reset();

	display_init();

	vTaskDelay(DELAY_3s);

	for (;;) {
		display_frame();
		display_reposo();

		vTaskDelay(FREQ_FRAME_RATE_DISPLAY(10));
	}

	vTaskDelete(NULL);
}

extern void tareasRtos_TaskRxMMA8451(void *pvparameters){
	PRINTF("Se creo la tarea mma8451\r\n");

	uint32_t ReadNorma=0;
	static uint32_t ValMax_Norma=0;

	for (;;) {
		if (xSemaphoreTake(DrydSemaphore, portMAX_DELAY) == pdTRUE) {

			PRINTF("TOMANDO DATOS\r\n");

			mma8451_IntDRYD();

			ReadNorma = mma8451_norma_cuadrado();

			if (ReadNorma > ValMax_Norma&& ReadNorma <= THS_REF_RANGO_2G_CUADRADO) {
				ValMax_Norma = ReadNorma;
				PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
						mma8451_getAcY(), mma8451_getAcZ());
			}

			if (ValMax_Norma > THS_MAX_FF_CUADRADO) {
				if (ReadNorma < THS_MAX_FF_CUADRADO) {
					mma8451_disableDRDYInt();
					xQueueSendToBackFromISR(queue_NormaMaxima, &ValMax_Norma,
							NULL);
				}
			}
		}
	}

	/*
	 * NOTA: El valor de ValMax_Norma debe enviarse a otra tarea, por lo que
	 * debe hacerse uso de una cola de datos Queue.
	 * */

	vTaskDelete(NULL);
}

extern void tareasRtos_Freefall_Interrupt(void *pvparameters){
	for (;;) {
		if (xSemaphoreTake(FreefallSemaphore, portMAX_DELAY) == pdTRUE) {
			PRINTF("Caida Libre\r\n");
			mma8451_IntFF();
			PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptLogicZero);
			Int_Freefall = 1;
			mma8451_enableDRDYInt();
		}
	}
	//	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptOrDMADisabled);	// Deshabilita la interrupcion 2

	return;
}


