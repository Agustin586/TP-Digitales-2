#include "IncludesFiles/timersRtos.h"
#include "IncludesFiles/SD2_board.h"
#include "IncludesFiles/mefSensor.h"

TimerHandle_t Timer1, Timer2, Timer3;

void Timer1_Callback(void *pvParameters);
void Timer2_Callback(void *pvParameters);

extern void timersRtos_create(void) {
	Timer1 = xTimerCreate("Radar Apagado", pdMS_TO_TICKS(200), pdTRUE, NULL,
			Timer1_Callback);
	Timer2 = xTimerCreate("Msj Toggle LRojo", pdMS_TO_TICKS(500), pdTRUE, NULL,
			Timer2_Callback);
	Timer3 = xTimerCreate("Lectura sensor", pdMS_TO_TICKS(20), pdTRUE, NULL,
			Timer3_Callback);
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
	case TIMER3:
		xTimerStart(Timer3, pdMS_TO_TICKS(100));
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
	case TIMER3:
		xTimerStop(Timer3, pdMS_TO_TICKS(100));
		break;
	default:
		break;
	}

	return;
}

void Timer1_Callback(void *pvParameters) {
	board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_TOGGLE);

	return;
}

void Timer2_Callback(void *pvParameters) {
	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);

	return;
}
