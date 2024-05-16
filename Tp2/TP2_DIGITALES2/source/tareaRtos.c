#include "tareasRtos.h"
#include "MACROS.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "display.h"
#include "mefSEC.h"

#define FREQ_FRAME_RATE_DISPLAY(x) pdMS_TO_TICKS(1/x)	// En Hz

extern void tareasRtos_TaskMEF(void *pvparameters){
	PRINTF("Se creo la tarea MEF\r\n");
	mefSEC_init();

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

	/* HABILITAMOS LA RECEPCION CONTINUA DE DATOS */
//	mma8451_enableDRDYInt();

	for (;;) {
		mma8451_IntDRYD();
		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n",mma8451_getAcX(),mma8451_getAcY(),mma8451_getAcZ());

		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);
}



