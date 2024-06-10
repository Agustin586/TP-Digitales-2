#ifndef INCLUDESFILES_TIMERSRTOS_H_
#define INCLUDESFILES_TIMERSRTOS_H_

#include "FreeRTOS.h"
#include "timers.h"

typedef enum {
	TIMER1 = 0, TIMER2, TIMER3, NUM_TIMERS,
} TimersId_t;

extern void timersRtos_create(void);
extern void timersRtos_start(TimersId_t TimerId);
extern void timersRtos_stop(TimersId_t TimerId);

#endif /* INCLUDESFILES_TIMERSRTOS_H_ */
