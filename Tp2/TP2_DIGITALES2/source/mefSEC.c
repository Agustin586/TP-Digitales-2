#include "mefSEC.h"
#include "MACROS.h"
#include "mma8451.h"
#include "tareasRtos.h"
#include "math.h"
#include "queue.h"
#include "display.h"

static uint16_t Delay_ms,Delay2_ms;
static uint32_t ValNorma_Max=0;
static estMefSec_enum estMefSec;

extern xQueueHandle queue_NormaMaxima,queueNormaRaiz;

#define DELAY_SOFTTIMER_500ms	50
#define DELAY_SOFTTIMER_250ms	25
#define DELAY_SOFTTIMER_10s		10*100

extern estMefSec_enum mefSEC_getEstado(void){
	return estMefSec;
}

extern uint16_t mefSEC_getNormaMaxima(void){
	return (uint16_t) sqrt(ValNorma_Max);
}

extern void mefSEC_init(void){
	estMefSec = EST_SECUENCIA_REPOSO;

	ValNorma_Max = 0;

	return;
}

extern void mefSEC(void){
	switch (estMefSec) {
	/* ============================================================
	 * DESCRIPCION: Se ingresa al estado siempre que no haya caida
	 * libre o que esta haya finalizado y mostrado los resultados.
	 * En este estado se detectará la cada libre.
	 * ============================================================
	 * */
	case EST_SECUENCIA_REPOSO:
		LED_AZUL(ON);
		LED_ROJO(OFF);

		/* CAMBIO DE ESTADO */
		if (tareasRtos_getEst_IntFreefall()) {
			estMefSec = EST_SECUENCIA_CAIDALIBRE;		// Cambia de estado
			Delay_ms = DELAY_SOFTTIMER_250ms;
		}

		break;

	/* =============================================================
	 * DESCRIPCION: Se inicia tal estado cuando el acelerometro
	 * detecta que ya hay caida libre y sale de dicho estado cuando
	 * se sobrepase cierto umbral una vez tomada cierta cantidad de
	 * muestras.
	 * =============================================================
	 * */
	case EST_SECUENCIA_CAIDALIBRE:
		LED_AZUL(OFF);

		if ((xQueueReceive(queue_NormaMaxima, &ValNorma_Max, DELAY_100ms) != errQUEUE_EMPTY) || !Delay_ms) {

			/* CAMBIO DE ESTADO */
			if (ValNorma_Max > THS_MAX_FF_CUADRADO) {
				Delay_ms = DELAY_SOFTTIMER_10s;
				Delay2_ms = DELAY_SOFTTIMER_500ms;

				LED_ROJO(OFF);

				PRINTF("\nNorma Maxima:%.2f\r\n",
						mefSEC_getNormaMaxima() / 100.0);

				estMefSec = EST_SECUENCIA_RESULTADO;
			}
			else if (!Delay_ms) {
				ValNorma_Max = 0;
				tareasRtos_reset_IntFreefall();
				estMefSec = EST_SECUENCIA_ERROR;
			}
		}

		break;

	case EST_SECUENCIA_RESULTADO:
		if (!Delay2_ms){
			LED_ROJO(TOGGLE);
			Delay2_ms = DELAY_SOFTTIMER_500ms;
		}

		if (!Delay_ms || key_getPressEvRTOS(SW1)){
			ValNorma_Max = 0;

			tareasRtos_reset_IntFreefall();

			estMefSec = EST_SECUENCIA_REPOSO;
		}

		break;

	case EST_SECUENCIA_ERROR:
		vTaskDelay(DELAY_2s);
		estMefSec = EST_SECUENCIA_REPOSO;

		break;
	default:
		estMefSec = EST_SECUENCIA_REPOSO;
		break;
	}

	return;
}

extern void mefSEC_periodicTask1ms(void){
	if (estMefSec != EST_SECUENCIA_REPOSO){
		if (Delay_ms)	Delay_ms--;
		if (Delay2_ms)	Delay2_ms--;
	}

	return;
}
