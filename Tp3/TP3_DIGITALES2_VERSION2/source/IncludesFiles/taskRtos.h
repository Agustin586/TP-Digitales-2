#ifndef TASKRTOS_H_
#define TASKRTOS_H_

typedef enum {
	TASK_ISR_UART0 = 0,
	TASK_ISR_RX_UART1,
	TASK_NEXTION,
	TASK_PERIFERICOS_SENSOR,
	TASK_PERIFERICOS_SERVO,
	NUM_TASK,
} TaskId_t;

extern void taskRtos_create(void);

#endif /* TASKRTOS_H_ */
