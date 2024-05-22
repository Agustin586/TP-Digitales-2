#include "tareasRtos.h"
#include "MACROS.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "display.h"
#include "mefSEC.h"
#include "queue.h"
#include "mefDisplay.h"

#define FREQ_FRAME_RATE_DISPLAY(x) pdMS_TO_TICKS(1/x)	// En Hz
#define BUFFER_FIN_FREEFALL	5;

volatile bool Int_Freefall;
static uint32_t ValMax_Norma;

extern xQueueHandle queue_NormaMaxima;
extern xSemaphoreHandle FreefallSemaphore, DrydSemaphore;

/*
 * @brief Pregunta si bajo ciertas condiciones termina la caida libre
 *
 * @param	uint32_t buffer[] Ultimos valores de buffer
 * 			uint8_t max_indice Cantidad maxima de buffer
 * @return bool
 * */
static bool Fin_Freefall(uint32_t buffer[], uint8_t max_indice);

extern bool tareasRtos_getEst_IntFreefall(void) {
	return Int_Freefall;
}

extern void tareasRtos_reset_IntFreefall(void) {
	Int_Freefall = 0;

	return;
}

extern void tareasRtos_TaskMEF(void *pvparameters) {
	PRINTF("Se creo la tarea MEF\r\n");

	mefSEC_init();
	for (;;) {
		mefSEC();
//		mma8451_IntDRYD();
//
//		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
//							mma8451_getAcY(), mma8451_getAcZ());
		vTaskDelay(DELAY_50ms);
	}

	vTaskDelete(NULL);
}

extern void tareasRtos_TaskDisplay(void *pvparameters) {
	PRINTF("Se creo la tarea Display\r\n");

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
	PRINTF("Se creo la tarea mma8451\r\n");

	uint32_t ReadNorma = 0;
	bool Freefall_interno = 0;
	static uint32_t buffer[10];
	static uint8_t indice = 0;

	for (uint8_t i = 0; i < 10; ++i) {
		buffer[i] = 0;
	}

	for (;;) {
		if (xSemaphoreTake(DrydSemaphore, portMAX_DELAY) == pdTRUE) {
//			PRINTF("TOMANDO DATOS\r\n");

			/* TOMA DATOS */
			mma8451_IntDRYD();
			ReadNorma = mma8451_norma_cuadrado();

			PRINTF("Valor Norma cuadrado:%d\r\n", ReadNorma);
			PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
					mma8451_getAcY(), mma8451_getAcZ());

			/* HAY UN VERDADERO FREEFALL */
			if (ReadNorma < THS_MAX_FF_CUADRADO && !Freefall_interno) {
				Freefall_interno = 1;
			} else if (ReadNorma > THS_MAX_FF_CUADRADO && !Freefall_interno) {
				mma8451_disableDRDYInt();
				ValMax_Norma = 0;
				ReadNorma = 0;
				Freefall_interno = 0;
				Int_Freefall = 0;
				indice = 0;
			}

			/* HUBO VERDADERAMENTE UNA CAIDA LIBRE */
			if (Freefall_interno) {
				Int_Freefall = 1;

				/* SIGUE EN CAIDA LIBRE */
				if (!Fin_Freefall(buffer, 10)) {
					buffer[indice] = ReadNorma;

					if (ReadNorma
							> ValMax_Norma&& ReadNorma <= THS_REF_RANGO_2G_CUADRADO) {
						ValMax_Norma = ReadNorma;
					}

					indice++;
					if (indice == 10)
						indice = 0;
				}

				else {
					mma8451_disableDRDYInt();
					xQueueSendToBackFromISR(queue_NormaMaxima, &ValMax_Norma,
							NULL);

					ReadNorma = 0;
					ValMax_Norma = 0;
					Freefall_interno = 0;
					indice = 0;
					Int_Freefall = 0;

					for (uint8_t i = 0; i < 10; ++i) {
						buffer[i] = 0;
					}
				}
			}

			PORT_ClearPinsInterruptFlags(INT1_PORT, 1 << INT1_PIN);
			PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN,
					kPORT_InterruptLogicZero);
		}
	}

	/*
	 * NOTA: El valor de ValMax_Norma debe enviarse a otra tarea, por lo que
	 * debe hacerse uso de una cola de datos Queue.
	 * */

	vTaskDelete(NULL);
}

extern void tareasRtos_Freefall_Interrupt(void *pvparameters) {
	for (;;) {
		if (xSemaphoreTake(FreefallSemaphore, portMAX_DELAY) == pdTRUE) {
			PRINTF("Caida Libre\r\n");

			mma8451_IntFF();

			PORT_ClearPinsInterruptFlags(INT2_PORT, 1 << INT2_PIN);
			PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN,
					kPORT_InterruptLogicZero);

			mma8451_enableDRDYInt();
		}
	}
	//	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptOrDMADisabled);	// Deshabilita la interrupcion 2
	vTaskDelete(NULL);

	return;
}

static bool Fin_Freefall(uint32_t buffer[], uint8_t max_indice) {
#define THS_FIN_FREEFALL_MIN	0.9*0.9
#define THS_FIN_FREEFALL_MAX	1.2*1.2
	for (uint8_t i = 0; i < max_indice; i++) {
		if (buffer[i] > THS_FIN_FREEFALL_MAX || buffer[i] < THS_FIN_FREEFALL_MIN)
			return false;
	}

	return true;
}

