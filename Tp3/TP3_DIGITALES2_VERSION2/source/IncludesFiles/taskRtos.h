#ifndef TASKRTOS_H_
#define TASKRTOS_H_

typedef enum {
	TASK_ISR_UART1 = 0,
	TASK_ISR_UART0,
	TASK_NEXTION,
	TASK_PERIFERICOS_SENSOR,
	TASK_PERIFERICOS_SERVO,
	TASK_PERIFERICOS_HCSR04,
	NUM_TASK,
} TaskId_t;

/*
 * @brief Crea todas las tareas.
 * */
extern void taskRtos_create(void);

#endif /* TASKRTOS_H_ */
