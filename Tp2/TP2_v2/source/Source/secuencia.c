/*
 * secuencia.c
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

/*< Archivos varios >*/
#include "Include/secuencia.h"
#include "Include/MACROS.h"
#include "Include/IntMma.h"
#include "Include/nextion.h"
#include "Include/sdcard.h"
#include "fatfs/fatfs_include/ff.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_debug_console.h"
#include "math.h"

/*< Archvios de freertos >*/
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

#define RANGO_2G	120000.0
#define MAX_VAL_WAVEFORM	200.0
#define CONV_VAL	MAX_VAL_WAVEFORM / RANGO_2G

/*< VARIABLES >*/
typedef enum {
	EST_SECUENCIA_REPOSO = 0, EST_SECUENCIA_RESULTADO,
} estMefSec_enum;

typedef enum {
	TIMER_10s = 0, TIMER_BLINK, CANT_TIMER,
} TimerID_t;

static uint32_t ValNorma_Max = 0;
static estMefSec_enum estMefSec;

static FIL file_ejes;
static File_t file;
static DatosMMA8451_t DatosEjes;
static float NormaMaxima;

/*< Timers Handlers >*/
TimerHandle_t Timer10s, TimerBlink;

/*< Banderas >*/
static bool F_timer10s = false;

/*< FUNCIONES >*/

/*
 * @brief	Mef de secuencia
 * */
static void mefSecuencia(void);

/*
 * @brief	Inicializacion de la mef secuencia
 * */
static void mefSecuencia_init(void);

/*
 * @brief	Inicia el timer por argumento
 *
 * @param	TimerID_t	Id del timer
 * */
static void timerRtos_start(TimerID_t timerID);

/*
 * @brief	Detiene el timer por argumento
 *
 * @param	TimerID_t	Id del timer
 * */
static void timerRtos_stop(TimerID_t timerID);

/*< CALLBACKs >*/
static void timerRtos_Timer10s(TimerHandle_t xTimer);
static void timerRtos_TimerBlink(TimerHandle_t xTimer);

extern void taskSecuencia(void *pvparameters) {
	PRINTF("> Tarea: Secuencia\r\n");

	mefSecuencia_init();

	for (;;) {
		mefSecuencia();
		delay_ms(10);
	}

	vTaskDelete(NULL);

	return;
}

static void mefSecuencia_init(void) {
	estMefSec = EST_SECUENCIA_REPOSO;
	ValNorma_Max = 0;

	file.file_ = file_ejes;
	file.nameFile = "DatosEjes.txt";

	return;
}

static void mefSecuencia(void) {
#define LONGITUD_MAX_STRING	20
	static uint8_t longitud;
	static char buffer[LONGITUD_MAX_STRING];
	static bool Flag = false;

	/*
	 * NOTA: El tipo de variables File_t ocupa mucha ram por lo tanto
	 * debemos tenerlo en cuenta a la hora de crear la tarea secuencia.
	 *
	 * */

	switch (estMefSec) {
	/* ============================================================
	 * DESCRIPCION: Secuencia de reposo. Se queda en este estado
	 * cuando finaliza la muestra de resultados o apenas arranca
	 * el programa. Se encarga de verificar si cumple con las
	 * condiciones para estar en caida libre.
	 * ============================================================
	 * */
	case EST_SECUENCIA_REPOSO:
		LED_AZUL(ON);
		LED_ROJO(OFF);

		/*< CAMBIO DE ESTADO >*/
		if (intMma_getIFFreeFall()) {
			LED_AZUL(OFF);
			timerRtos_start(TIMER_BLINK);
			timerRtos_start(TIMER_10s);
			estMefSec = EST_SECUENCIA_RESULTADO;		// Cambia de estado
		}

		break;

		/* ============================================================
		 * DESCRIPCION: Toman los valores que provienen de la cola de
		 * datos y los guarda en una variable que luego será tomada
		 * por la mefDisplay para mostrar el resultado.
		 * ============================================================
		 * */
	case EST_SECUENCIA_RESULTADO:
		/* ================================================
		 * DESCRIPCION: Recive el valor máximo de la norma
		 * al cuadrado.
		 * ================================================
		 * */
		if (!Flag) {
			Flag = true;
			/*< IMPRIME LA NORMA MAXIMA >*/
			NormaMaxima = sqrt((float) queueRtos_receiveNormaMaxCuad()) / 100.0;

			PRINTF("> Dato leido\n");
			PRINTF("Norma Maxima:%.2f g\r\n", NormaMaxima);

			sprintf(buffer, "%.2f g", NormaMaxima);
			nextion_text(TEXT_ID(0), buffer);

			/*< ENVIA LA NORMA A LA PANTALLA >*/


//			sprintf(file.buffer, "Eje X\t\tEje Y\t\tEje Z\r\n");
//			sd_write(file);

			for (uint8_t dato = 0; dato < MAX_QUEUE_LONG; dato++) {
				queueRtos_receiveDatosEjes(&DatosEjes, &longitud);

				if (longitud == 0)
					break;

				/*< Escritura en la pantalla >*/
				nextion_waveform(WAVEFORM_ID0(2), WAVEFORM_CHANNEL(0),
						(uint8_t)( DatosEjes.NormaCuad * CONV_VAL));
				delay_ms(5);

				/*< Escritura en la memoria sd >*/
//				sprintf(file.buffer, "%.2f\t\t%.2f\t\t%.2f\r\n",
//						DatosEjes[dato].ReadX, DatosEjes[dato].ReadY,
//						DatosEjes[dato].ReadZ);
//				sd_write(file);
			}
		}

		/* ================================================
		 * DESCRIPCION: Finaliza la muestra de resultados.
		 * ================================================
		 * */
		if (F_timer10s || board_getSw(SW1)) {
			Flag = false;
			F_timer10s = false;
			timerRtos_stop(TIMER_10s);
			timerRtos_stop(TIMER_BLINK);
			intMma_clrIFFreeFall();
			estMefSec = EST_SECUENCIA_REPOSO;
		}

		break;
	default:
		estMefSec = EST_SECUENCIA_REPOSO;
		break;
	}

	return;
}

extern void timerRtos_init(void) {
	Timer10s = xTimerCreate("timer 10s", pdMS_TO_TICKS(10000), pdFALSE, NULL,
			timerRtos_Timer10s);
	if (Timer10s == NULL) {
		PRINTF("Error al crear un timer\r\n");
		while (1)
			;
	}

	TimerBlink = xTimerCreate("timer blink", pdMS_TO_TICKS(500), pdTRUE, NULL,
				timerRtos_TimerBlink);
	if (TimerBlink == NULL) {
		PRINTF("Error al crear un timer\r\n");
		while (1)
			;
	}

	return;
}

static void timerRtos_start(TimerID_t timerID) {
	switch (timerID) {
	case TIMER_10s:
		if (xTimerStart(Timer10s, pdMS_TO_TICKS(10)) != pdPASS)
			PRINTF("Error al iniciar el timer\r\n");
		break;
	case TIMER_BLINK:
		if (xTimerStart(TimerBlink, pdMS_TO_TICKS(10)) != pdPASS)
			PRINTF("Error al iniciar el timer\r\n");
		break;
	default:
		PRINTF("Error al iniciar un timer\r\n");
		break;
	}

	return;
}

static void timerRtos_stop(TimerID_t timerID) {
	switch (timerID) {
	case TIMER_10s:
		xTimerStop(Timer10s, pdMS_TO_TICKS(10));
		break;
	case TIMER_BLINK:
		xTimerStop(TimerBlink, pdMS_TO_TICKS(10));
		break;
	default:
		PRINTF("Error al detener un timer\r\n");
		break;
	}
}

static void timerRtos_Timer10s(TimerHandle_t xTimer) {
	F_timer10s = true;

	return;
}

static void timerRtos_TimerBlink(TimerHandle_t xTimer) {
	LED_ROJO(TOGGLE);

	return;
}
