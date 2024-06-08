#include "IncludesFiles/mefNextion.h"
#include "IncludesFiles/nextion.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/mefServo.h"
#include "IncludesFiles/HCSR04.h"
#include "IncludesFiles/mefSensor.h"
#include <stdint.h>

#define PIC_ID	0

static estMefNextion_enum estMefNextion;
static uint8_t obj_guardados = 0;

static uint8_t picMovRadar(int angle);

extern void mefNextion_init(void) {
	estMefNextion = EST_NEXTION_pRADAR;

	return;
}

extern void mefNextion(void) {
	static int16_t angle_old = 0;

	switch (estMefNextion) {
	case EST_NEXTION_RESET:
		PRINTF("No se detecto la pagina de la pantalla\r\n");
//		taskRtosNextion_error();

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pMAIN:

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pRADAR:
		if (angle_old != mefServo_getAngle()) {
			if(angle_old == 45)	obj_guardados = 0;

			nextion_setDataObj(105 + mefServo_getAngle(), mefSensor_getDistance(), obj_guardados);
			obj_guardados++;
			nextion_putPicture(PIC_ID, picMovRadar(mefServo_getAngle()));
			angle_old = mefServo_getAngle();
		}

		for (uint8_t i = 0; i < obj_guardados; i++) {
			nextion_putObj(i,63);
		}

//		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pSERVO:

		estMefNextion = nextion_getPage();
		break;
	default:
		break;
	}

	return;
}

static uint8_t picMovRadar(int angle) {
#define NUM_TOTAL_PASOS	9
	int angles[] = { -75, -60, -45, -30, -15, 0, 15, 30, 45 };
	int values[] = { 22, 23, 24, 25, 26, 27, 28, 29, 30 };

	for (int i = 0; i < NUM_TOTAL_PASOS; i++) {
		if (angles[i] == angle) {
			return values[i];
		}
	}
	return -1;
}
