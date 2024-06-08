#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/HCSR04.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "fsl_debug_console.h"

#define SENSOR_DELAY 10

typedef enum {
	EST_SENSOR_RESET = 0, EST_SENSOR_ENABLE, EST_SENSOR_DISABLE,
} estMefSensor_enum;

static estMefSensor_enum estMefSensor;

static void TriggerPulse(void);

extern void mefSensor_init(void) {
	estMefSensor = EST_SENSOR_RESET;

	return;
}

extern void mefSensor(void) {
	switch (estMefSensor) {
	case EST_SENSOR_RESET:
		/*Acciones de reset*/
		HCSR04_init();

		estMefSensor = EST_SENSOR_ENABLE;
		break;
	case EST_SENSOR_ENABLE:
		/*Acciones de enable*/

		if (!HCSR04_distanceReady()) {
			TriggerPulse();
		} else {
			PRINTF("Distancia medida:%.2f\r\n", mefSensor_getDistance());
			taskRtosPERIFERICOS_delay(50);
		}

//		estMefSensor = EST_SENSOR_DISABLE;
		break;
	case EST_SENSOR_DISABLE:
		/*Acciones de disable*/
//		vtaskDelay(SENSOR_DELAY);
//		estMefSensor = EST_SENSOR_ENABLE;
		break;
	default:
		break;
	}

	return;
}

extern float mefSensor_getDistance(void) {
	return HCSR04_getDistance();
}

static void TriggerPulse(void) {
	HCSR04_setTrigger();
	taskRtosPERIFERICOS_delay(2);
	HCSR04_clrTrigger();
}
