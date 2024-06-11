#include "IncludesFiles/mefNextion.h"
#include "IncludesFiles/nextion.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/mefServo.h"
#include "IncludesFiles/HCSR04.h"
#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/pwm.h"
#include <stdint.h>

#define PIC_ID	0

static estMefNextion_enum estMefNextion;
static bool ref_pantalla = false;

static uint8_t picMovRadar(int angle);

extern void mefNextion_init(void) {
	estMefNextion = EST_NEXTION_pMAIN;
	ref_pantalla = true;
	nextion_reset();

	return;
}

extern void mefNextion(void) {
#define COLOR_RED	63488
#define PASO		0
#define CANT_MAX_MUESTRAS	14
	static int16_t angle_old = 0;
	static uint8_t muestra = 0;
	uint8_t pwm_dutty;

	switch (estMefNextion) {
	case EST_NEXTION_RESET:
		PRINTF("No se detecto la pagina de la pantalla\r\n");

		estMefNextion = nextion_getPage(EST_NEXTION_RESET);
		break;
	case EST_NEXTION_pMAIN:

		taskRtosNextion_delay(50);

		estMefNextion = nextion_getPage(EST_NEXTION_pMAIN);
		break;
	case EST_NEXTION_pRADAR:

		if (angle_old != mefServo_getAngle()) {
			nextion_putPicture(PIC_ID, picMovRadar(mefServo_getAngle()));
			angle_old = mefServo_getAngle();
			taskRtosNextion_delay(5);
			nextion_setAngle(angle_old);
		}

		taskRtosNextion_delay(5);

		nextion_putObj(PASO, muestra,
				nextion_getColorAngle(muestra, mefServo_getAngle()));

		if (muestra < CANT_MAX_MUESTRAS)
			muestra++;
		else {
			muestra = 0;
			taskRtosNextion_delay(5);
			nextion_setDistanciaProm();
		}

		estMefNextion = nextion_getPage(EST_NEXTION_pRADAR);
		break;
	case EST_NEXTION_pSERVO:
#define WAVEFORM_ID	2
#define WAVEFORM_CHANNEL	0
#define MAX_VAL_DISPLAY		255
#define FREQ_OBJ	1
#define PWM_OBJ		0

		taskRtosNextion_delay(10);

		nextion_sendPwmValue(WAVEFORM_ID, WAVEFORM_CHANNEL,
				pwm_Value() * (1 + MAX_VAL_DISPLAY));

		taskRtosNextion_delay(10);

		pwm_dutty = pwm_getPwmDutty();
		nextion_sendFreqPwm(PWM_OBJ, pwm_dutty);

		taskRtosNextion_delay(10);

		nextion_sendFreqPwm(FREQ_OBJ,50);

		taskRtosNextion_delay(5);

		nextion_setMsPwm(pwm_dutty*0.2);

		taskRtosNextion_delay(5);

		estMefNextion = nextion_getPage(EST_NEXTION_pSERVO);
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
