#include "mefSEC.h"
#include "MACROS.h"
#include "mma8451.h"
#include "tareasRtos.h"

static uint32_t ValNorma_Max=0;
static estMefSec_enum estMefSec;

#define G_THS		0.7
#define THS_MAX_FF 	G_THS*100
#define THS_MAX_FF_CUADRADO	THS_MAX_FF*THS_MAX_FF
#define THS_REF_RANGO_2G_CUADRADO	200*200

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
			estMefSecuencia = EST_SECUENCIA_CAIDALIBRE;		// Cambia de estado
			Delay_ms = DELAY_500ms;	// Configura el delay para el proximo estado
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

		/* CAMBIO DE ESTADO */
		if (ValNorma_Max > THS_MAX_FF_CUADRADO) {
			Delay_ms = DELAY_10seg;
			Delay2_ms = DELAY_500ms;

			LED_ROJO(OFF);
			SW_Pulsado(SW1);

			PRINTF("\nNorma Maxima:%.2f\n",
					mefSEC_getNormaMaxima() / 100.0);

			estMefSecuencia = EST_SECUENCIA_RESULTADO;
		}

		break;

	case EST_SECUENCIA_RESULTADO:


		break;
	default:
		break;
	}

	return;
}

extern void mefSEC_periodicTask1ms(void){

	return;
}
