#ifndef TAREASRTOS_H_
#define TAREASRTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define STACK_SIZE	configMINIMAL_STACK_SIZE+10
#define TASK_MEF_PRIORITY		3
#define TASK_DISPLAY_PRIORITY	2
#define TASK_RX_MMA8451_PRIORITY 2

/* DELAYS TO TICKS */
#define DELAY_5ms	pdMS_TO_TICKS(5)
#define DELAY_10ms	pdMS_TO_TICKS(10)
#define DELAY_50ms	pdMS_TO_TICKS(50)
#define DELAY_100ms	pdMS_TO_TICKS(100)
#define DELAY_250ms	pdMS_TO_TICKS(250)
#define DELAY_500ms	pdMS_TO_TICKS(500)
#define DELAY_3s	pdMS_TO_TICKS(3000)
/*-----------------*/

/*
 * @brief Tarea que lleva a cabo la MEF del programa principal
 * */
extern void tareasRtos_TaskMEF(void *pvparameters);

/*
 * @brief Tarea que se comunica con el display oled
 * */
extern void tareasRtos_TaskDisplay(void *pvparameters);

/*
 * @brief Tarea que toma datos del acelerometro en el tiempo
 * */
extern void tareasRtos_TaskRxMMA8451(void *pvparameters);

/*
 * @brief Interrupcion por freefall
 * */
extern static void tareasRtos_Freefall_Interrupt(void);

/*
 * @brief Devuelve el estado de la bandera de interrupcion
 *
 * @return bool
 * */
extern bool tareasRtos_getEst_IntFreefall(void);

#endif /* TAREASRTOS_H_ */
