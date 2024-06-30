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
#include "stdint.h"
#include "stdbool.h"
#include "fsl_debug_console.h"
#include "math.h"

/*< Archvios de freertos >*/
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

/*< VARIABLES >*/
typedef enum {
	EST_SECUENCIA_REPOSO = 0, EST_SECUENCIA_RESULTADO,
} estMefSec_enum;

typedef enum {
	TIMER_10s = 0, TIMER_BLINK, CANT_TIMER,
} TimerID_t;

static uint32_t ValNorma_Max = 0;
static estMefSec_enum estMefSec;

/*< Timers Handlers >*/
TimerHandle_t Timer10s, TimerBlink;

/*< Banderas >*/
bool F_timer10s = false;

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
 * @brief	Inicializa todos los timer de rtos
 * */
static void timerRtos_init(void);

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
void timerRtos_Timer10s(void *pvParameters);
void timerRtos_TimerBlink(void *pvParameters);

extern void taskSecuencia(void *pvparameters) {
	mefSecuencia_init();

	for (;;) {
		mefSecuencia();
	}

	vTaskDelete(NULL);
	return;
}

static void mefSecuencia_init(void) {
	estMefSec = EST_SECUENCIA_REPOSO;
	ValNorma_Max = 0;

	return;
}

static void mefSecuencia(void) {
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

		/* CAMBIO DE ESTADO */
		if (intMma_getIFFreeFall()) {
			LED_AZUL(OFF);
			timerRtos_start(TIMER_10s);
			timerRtos_start(TIMER_BLINK);
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
		PRINTF("DATO LEIDO\r\n");
		PRINTF("Norma Maxima:%.2f\r\n", sqrt((float)queueRtos_receiveNormaMaxCuad()) / 100.0);

		/*
		 * Deberia enviar todos los datos a la pantalla y a la
		 * memoria sd.
		 * */

		/* ================================================
		 * DESCRIPCION: Finaliza la muestra de resultados.
		 * ================================================
		 * */
		if (F_timer10s || key_getPressEvRTOS(SW1)) {
			ValNorma_Max |= 0;
			mefInt2_clrIF_Freefall();

			estMefSec = EST_SECUENCIA_REPOSO;
		}

		break;
	default:
		estMefSec = EST_SECUENCIA_REPOSO;
		break;
	}
}

static void timerRtos_init(void) {
	if (xTimerCreate("timer 10s", pdMS_TO_TICKS(10000), pdFALSE, NULL,
			timerRtos_Timer10s) == NULL)
		PRINTF("Error al crear un timer\r\n");
	if (xTimerCreate("timer blink", pdMS_TO_TICKS(500), pdTRUE, NULL,
			timerRtos_TimerBlink) == NULL)
		PRINTF("Error al crear un timer\r\n");

	return;
}

static void timerRtos_start(TimerID_t timerID) {
	switch (timerID) {
	case TIMER_10s:
		xTimerStart(Timer10s, delay_ms(100));
		break;
	case TIMER_BLINK:
		xTimerStart(TimerBlink, delay_ms(100));
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
		xTimerStop(Timer10s, delay_ms(100));
		break;
	case TIMER_BLINK:
		xTimerStop(TimerBlink, delay_ms(100));
		break;
	default:
		PRINTF("Error al detener un timer\r\n");
		break;
	}
}

void timerRtos_Timer10s(void *pvParameters) {
	F_timer10s = true;

	return;
}

void timerRtos_TimerBlink(void *pvParameters) {
	LED_ROJO(TOGGLE);

	return;
}
