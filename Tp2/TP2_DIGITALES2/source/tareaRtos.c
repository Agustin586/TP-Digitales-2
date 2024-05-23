#include "tareasRtos.h"
#include "MACROS.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "display.h"
#include "mefSEC.h"
#include "queue.h"
#include "mefDisplay.h"
#include "mefInt2.h"

#define FREQ_FRAME_RATE_DISPLAY(x) pdMS_TO_TICKS(1/x)	// En Hz
#define BUFFER_FIN_FREEFALL	5;

extern xSemaphoreHandle FreefallSemaphore, DrydSemaphore;

extern void tareasRtos_TaskMEF(void *pvparameters) {
//	PRINTF("Se creo la tarea MEF\r\n");

	mefSEC_init();

	for (;;) {
		mefSEC();

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);
}

extern void tareasRtos_TaskDisplay(void *pvparameters) {
//	PRINTF("Se creo la tarea Display\r\n");

	board_configSPI0();
	oled_reset();
	display_init();

	mefDisplay_init();

	vTaskDelay(DELAY_3s);

	for (;;) {
		mefDisplay();

		vTaskDelay(FREQ_FRAME_RATE_DISPLAY(1));
	}

	vTaskDelete(NULL);
}

extern void tareasRtos_TaskRxMMA8451(void *pvparameters) {
//	PRINTF("Se creo la tarea Interrupcion por drdy\r\n");

	mefInt2_init();

	for (;;) {

		if (xSemaphoreTake(DrydSemaphore, portMAX_DELAY) == pdTRUE) {

			/* TOMO TODOS LO DATOS NECESARIOS */
			for (;;) {
				mefInt2();
//				vTaskDelay(DELAY_50ms);

				if (mefInt2_getFDrdy()) {
					mefInt2_clrFDrdy();
					break;
				}
			}
//
//			PORT_ClearPinsInterruptFlags(INT1_PORT, 1 << INT1_PIN);
//			PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN,
//					kPORT_InterruptLogicZero);
//		}
			PORT_ClearPinsInterruptFlags(INT2_PORT, 1 << INT2_PIN);
			PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN,
					kPORT_InterruptLogicZero);
		}
	}

	/*
	 * NOTA: El valor de Max_Norm debe enviarse a otra tarea, por lo que
	 * debe hacerse uso de una cola de datos Queue.
	 * */

	vTaskDelete(NULL);

	return;
}

extern void tareasRtos_Freefall_Interrupt(void *pvparameters) {
	for (;;) {
		if (xSemaphoreTake(FreefallSemaphore, portMAX_DELAY) == pdTRUE) {
			PRINTF("Caida Libre!!!\r\n");

			mma8451_IntFF();
			mefInt2_reset();
			mma8451_enableDRDYInt();
			xSemaphoreGive(DrydSemaphore);
		}
	}

	vTaskDelete(NULL);

	return;
}

