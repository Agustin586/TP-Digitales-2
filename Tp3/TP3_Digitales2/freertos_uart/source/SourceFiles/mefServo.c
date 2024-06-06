#include "IncludesFiles/mefServo.h"
#include "IncludesFiles/MG90S.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"

typedef enum {
	EST_SERVO_RESET = 0, EST_SERVO_RUNNING, EST_SERVO_STOP,
} estMefServo_enum;

static estMefServo_enum estMefServo;

extern void mefServo_init(void) {
	estMefServo = EST_SERVO_RESET;
	MG90S_init();

	return;
}

uint8_t getCharValue = 0U;

extern void mefServo(void) {
	switch (estMefServo) {
	case EST_SERVO_RESET:
		estMefServo = EST_SERVO_RUNNING;

		break;
	case EST_SERVO_RUNNING:
		getCharValue = GETCHAR() - 0x30U;

		PRINTF("%d\r\n", getCharValue);

		if (getCharValue == 1)
			MG90S_setAngle(-90);
		if (getCharValue == 5)
			MG90S_setAngle(0);
		if (getCharValue == 9)
			MG90S_setAngle(95);
		if (getCharValue == 7)
			MG90S_setAngle(-180);
		if (getCharValue == 8)
			MG90S_setAngle(3300);

		taskRtosPERIFERICOS_delayServo();

		break;
	case EST_SERVO_STOP:

		break;
	default:
		break;
	}

	return;
}
