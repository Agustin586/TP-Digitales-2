#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/HCSR04.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/nextion.h"
#include "IncludesFiles/mefServo.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/timersRtos.h"
#include "IncludesFiles/SD2_board.h"
#include "IncludesFiles/procTrama.h"

typedef enum {
	EST_SENSOR_RESET = 0, EST_SENSOR_ENABLE, EST_SENSOR_DISABLE,
} estMefSensor_enum;

static estMefSensor_enum estMefSensor;
static float ult_dist = 0;
static bool dato_listo = false;

static void TriggerPulse(void);

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
		timersRtos_start(TIMER3);

		estMefSensor = EST_SENSOR_ENABLE;
		break;
	case EST_SENSOR_ENABLE:
		/*Acciones de enable*/
		if (dato_listo) {
			PRINTF("Distancia medida:%.2f\r\n", mefSensor_getDistance());
			if (mefSensor_getDistance() <= MAXIMA_DISTANCIA)
				nextion_setDataObj(mefServo_getAngle() + 105,
						mefSensor_getDistance());
		}

		if (!procTrama_estadoRadar()) {
			estMefSensor = EST_SENSOR_DISABLE;
			timersRtos_start(TIMER1);
			timersRtos_stop(TIMER3);
		}
		break;
	case EST_SENSOR_DISABLE:
		if (procTrama_estadoRadar()) {
			timersRtos_stop(TIMER1);
			timersRtos_start(TIMER3);
			board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
			estMefSensor = EST_SENSOR_ENABLE;
		}

		break;
	default:
		break;
	}

	return;
}

extern void Timer3_Callback(void *pvParameters) {
	if (!HCSR04_distanceReady())
		TriggerPulse();
	else
		ult_dist = HCSR04_getDistance(), dato_listo = true;

	return;
}

extern float mefSensor_getDistance(void) {
	return ult_dist;
}

static void TriggerPulse(void) {
	HCSR04_setTrigger();
	taskRtosPERIFERICOS_delay(10);
	HCSR04_clrTrigger();
}
