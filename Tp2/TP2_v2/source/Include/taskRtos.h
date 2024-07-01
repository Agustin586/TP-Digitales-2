/*
 * taskRtos.h
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_TASKRTOS_H_
#define INCLUDE_TASKRTOS_H_

typedef enum {
	TASK_ISR_UART1 = 0,
	TASK_INT_FF,
	TASK_INT_DRDY,
	TASK_SEC,
	NUM_TASK,
} TaskId_t;

/*
 * @brief Crea todas las tareas.
 * */
extern void taskRtos_create(void);

#endif /* INCLUDE_TASKRTOS_H_ */
