#include "IncludesFiles/MG90S.h"
#include "IncludesFiles/pwm.h"
#include "IncludesFiles/MACROS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"

static uint8_t quick_round(float num);

extern void MG90S_init(void) {
	pwm_init();

	return;
}

static uint8_t quick_round(float num) {

	float parte_entera;
	float parte_fraccionaria;

	parte_fraccionaria = modff(num, &parte_entera);

	if (parte_fraccionaria >= 0.5) {
		return parte_entera + 1;
	} else
		return parte_entera;
}

extern void MG90S_setAngle(int16_t angle) {
	float conv_lineal = 6.97 + 0.0685 * angle + 6.43 * 0.00001 * angle * angle
			- 9.02 * 0.0000001 * angle * angle * angle
			- 1.51 * 0.00000001 * angle * angle * angle * angle;
	float dutyCycle = quick_round(conv_lineal);

	pwm_setDuty((uint8_t) dutyCycle);

	return;
}
extern void MG90S_test(void) {
	MG90S_setAngle(-90);
	vTaskDelay(DELAY_1s);
	MG90S_setAngle(0);
	vTaskDelay(DELAY_1s);
	MG90S_setAngle(90);
	vTaskDelay(DELAY_1s);

	return;
}

