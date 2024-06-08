#include "IncludesFiles/MG90S.h"
#include "IncludesFiles/pwm.h"
#include "IncludesFiles/MACROS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"

static uint8_t quick_round(float num);

extern void MG90S_init(void) {
	pwm_init();
	MG90S_test();

	return;
}

static uint8_t quick_round(float num){

	float parte_entera;
	float parte_fraccionaria;

	parte_fraccionaria = modff(num, &parte_entera);

	if(parte_fraccionaria >= 0.5){
		return parte_entera + 1;
	}
	else return parte_entera;
}

extern void MG90S_setAngle(int16_t angle) {
//	float dutyCycle = 5.0 + (3.35 * (angle / 120.0));
	float conv_lineal = 6.97 + 0.0685*angle + 6.43*0.00001*angle*angle - 9.02*0.0000001*angle*angle*angle - 1.51*0.00000001*angle*angle*angle*angle;
	float dutyCycle = quick_round(conv_lineal);

	pwm_setDuty((uint8_t) dutyCycle);
//	pwm_setDuty(angle);

	return;
}
extern void MG90S_test(void) {
	MG90S_setAngle(-90);	// 1ms -90 grados
	vTaskDelay(DELAY_2s);
	pwm_setDuty(0);	// 1.5ms 0 grados
	vTaskDelay(DELAY_2s);
	pwm_setDuty(90); //2ms son 90 grados
	vTaskDelay(DELAY_2s);

	return;
}

