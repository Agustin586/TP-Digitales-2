#include "tareasRtos.h"
#include "MACROS.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "display.h"
#include "mefSEC.h"

#define FREQ_FRAME_RATE_DISPLAY(x) pdMS_TO_TICKS(1/x)	// En Hz

volatile bool Int_Freefall;
volatile uint16_t ValMax_Norma=0;

extern bool tareasRtos_getEst_IntFreefall(void){
	return Int_Freefall;
}

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

	uint16_t ReadNorma=0;

	for (;;) {
		mma8451_IntDRYD();

		ReadNorma = mma8451_norma_cuadrado();

		if (ReadNorma > ValMax_Norma && ReadNorma <= THS_REF_RANGO_2G_CUADRADO){
			ValMax_Norma = ReadNorma;
			PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n",mma8451_getAcX(),mma8451_getAcY(),mma8451_getAcZ());
		}

		if (ValMax_Norma > THS_MAX_FF_CUADRADO)
			if (ReadNorma < THS_MAX_FF_CUADRADO)
				mma8451_disableDRDYInt();

		xSemaphoreTake(DrydSemaphore, portMAX_DELAY);
	}

	vTaskDelete(NULL);
}

extern static void tareasRtos_Freefall_Interrupt(void){
	for (;;) {
		PRINTF("Caida Libre\r\n");

		Int_Freefall = 1;

		mma8451_enableDRDYInt();

		xSemaphoreTake(FreefallSemaphore, portMAX_DELAY);
	}
	//	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptOrDMADisabled);	// Deshabilita la interrupcion 2

	return;
}


