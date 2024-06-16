#ifndef INCLUDESFILES_TIMERSRTOS_H_
#define INCLUDESFILES_TIMERSRTOS_H_

#include "FreeRTOS.h"
#include "timers.h"

typedef enum {
	TIMER1 = 0, TIMER2, NUM_TIMERS,
} TimersId_t;

/*
 * @brief Crea todos los timers solicitados.
 * */
extern void timersRtos_create(void);

/*
 * @brief Inicia el timer solicitado.
 *
 * @param TimersId_t TimerId.
 * */
extern void timersRtos_start(TimersId_t TimerId);

/*
 * @brief Detiene el timer solicitado.
 *
 * @param TimersId_t TimerId.
 * */
extern void timersRtos_stop(TimersId_t TimerId);

/*
 * @brief Devuelve el handle del timer solicitado.
 *
 * @param TimersId_t TimerId.
 * @return TimerHandle_t.
 * */
extern TimerHandle_t timersRtos_getTimerId(TimersId_t TimerId);

#endif /* INCLUDESFILES_TIMERSRTOS_H_ */
