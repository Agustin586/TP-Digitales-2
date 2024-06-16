#include "IncludesFiles/mefServo.h"
#include "IncludesFiles/MG90S.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/nextion.h"
#include "IncludesFiles/MACROS.h"

#include "IncludesFiles/procTrama.h"

typedef enum {
	EST_SERVO_RESET = 0, EST_SERVO_RUNNING, EST_SERVO_STOP,
} estMefServo_enum;

static estMefServo_enum estMefServo;
static int8_t angle_servo = 0;

extern void mefServo_init(void) {
	estMefServo = EST_SERVO_RESET;
	MG90S_init();

	return;
}

extern void mefServo(void) {
	switch (estMefServo) {
	case EST_SERVO_RESET:
		MG90S_test();

		estMefServo = EST_SERVO_RUNNING;
		break;
	case EST_SERVO_RUNNING:
		for (int8_t angle = -75; angle <= 45; angle = angle + 15) {
			angle_servo = angle;
			MG90S_setAngle(-angle - 15);
			vTaskDelay(pdMS_TO_TICKS(600));
		}

		nextion_clrPaso();	/*<Resetea el indice>*/
		nextion_clrDatos();	/*<Pone en cero todos los datos del arreglo>*/

		for (int8_t angle = 45; angle >= -75; angle -= 15) {
			angle_servo = angle;
			MG90S_setAngle(-angle - 15);
			vTaskDelay(pdMS_TO_TICKS(600));
		}

		nextion_clrPaso();
		nextion_clrDatos();

		if (!procTrama_estadoRadar()) {
			estMefServo = EST_SERVO_STOP;
		}

		break;
	case EST_SERVO_STOP:

		if (procTrama_estadoRadar()) {
			estMefServo = EST_SERVO_RUNNING;
		}

		break;
	default:
		break;
	}

	return;
}

extern int8_t mefServo_getAngle(void) {
	return angle_servo;
}

