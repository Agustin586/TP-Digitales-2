#include "IncludesFiles/timersRtos.h"
#include "IncludesFiles/SD2_board.h"
#include "IncludesFiles/mefSensor.h"
#include "IncludesFiles/MACROS.h"

TimerHandle_t Timer1, Timer2;

void Timer1_Callback(void *pvParameters);
void Timer2_Callback(void *pvParameters);

extern void timersRtos_create(void) {
	Timer1 = xTimerCreate("Timer_RadarApag", DELAY_250ms, pdTRUE, NULL,
			Timer1_Callback);
	Timer2 = xTimerCreate("Timer_ToggleRL", DELAY_500ms, pdTRUE, NULL,
			Timer2_Callback);
	return;
}

extern TimerHandle_t timersRtos_getTimerId(TimersId_t TimerId) {
	switch (TimerId) {
	case TIMER1:
		return Timer1;
		break;
	case TIMER2:
		return Timer2;
		break;
	default:
		break;
	}
	return NULL;
}

extern void timersRtos_start(TimersId_t TimerId) {
	switch (TimerId) {
	case TIMER1:
		xTimerStart(Timer1, pdMS_TO_TICKS(100));
		break;
	case TIMER2:
		xTimerStart(Timer2, pdMS_TO_TICKS(100));
		break;
	default:
		break;
	}

	return;
}

extern void timersRtos_stop(TimersId_t TimerId) {
	switch (TimerId) {
	case TIMER1:
		xTimerStop(Timer1, pdMS_TO_TICKS(100));
		break;
	case TIMER2:
		xTimerStop(Timer2, pdMS_TO_TICKS(100));
		break;
	default:
		break;
	}

	return;
}

void Timer1_Callback(void *pvParameters) {
	LED_VERDE(TOGGLE);

	return;
}

void Timer2_Callback(void *pvParameters) {
	LED_ROJO(TOGGLE);

	return;
}
