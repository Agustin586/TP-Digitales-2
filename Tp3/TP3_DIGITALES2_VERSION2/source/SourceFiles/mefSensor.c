#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/HCSR04.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/nextion.h"
#include "IncludesFiles/mefServo.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/timersRtos.h"
#include "IncludesFiles/SD2_board.h"
#include "IncludesFiles/procTrama.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IncludesFiles/MACROS.h"

typedef enum {
	EST_SENSOR_RESET = 0, EST_SENSOR_ENABLE, EST_SENSOR_DISABLE,
} estMefSensor_enum;

static estMefSensor_enum estMefSensor;
static float ult_dist = 0;					/*<Ultima distancia medida>*/
static bool dato_listo = false;				/*<Indica cuando se debe cargar otro dato nuevo>*/

extern void mefSensor_init(void) {
	estMefSensor = EST_SENSOR_RESET;

	return;
}

extern void mefSensor(void) {
#define MAXIMA_DISTANCIA	60
	switch (estMefSensor) {
	case EST_SENSOR_RESET:
		/*Acciones de reset*/
		HCSR04_init();
		board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);

		estMefSensor = EST_SENSOR_ENABLE;
		break;
	case EST_SENSOR_ENABLE:
		/*Acciones de enable*/
		if (dato_listo) {
//			PRINTF("Distancia medida:%.2f\r\n", mefSensor_getDistance());
			/*Solo guarda objetos dentro de la distancia espefcificada*/
			if (mefSensor_getDistance() <= MAXIMA_DISTANCIA)
				nextion_setDataObj(mefServo_getAngle() + 105,
						mefSensor_getDistance()); /*<Guarda el objeto dentro del arreglo>*/
			dato_listo = false;
		}

		/*Condición de salida por trama de Uart0*/
		if (!procTrama_estadoRadar()) {
			estMefSensor = EST_SENSOR_DISABLE;
			timersRtos_start(TIMER1);
		}
		break;
	case EST_SENSOR_DISABLE:
		if (procTrama_estadoRadar()) {
			timersRtos_stop(TIMER1);
			board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
			estMefSensor = EST_SENSOR_ENABLE;
		}

		break;
	default:
		break;
	}

	return;
}

extern void mefSensor_setDatoListo(void) {
	dato_listo = true;

	return;
}

extern void mefSensor_clrDatoListo(void) {
	dato_listo = false;

	return;
}

extern bool mefSensor_getDatoListo(void) {
	return dato_listo;
}

extern void mefSensor_setDistancia(float distancia) {
	ult_dist = distancia;

	return;
}

extern float mefSensor_getDistance(void) {
	return ult_dist;
}

extern void TriggerPulse(void) {
	HCSR04_setTrigger();
	vTaskDelay(DELAY_5ms);
	HCSR04_clrTrigger();
}
