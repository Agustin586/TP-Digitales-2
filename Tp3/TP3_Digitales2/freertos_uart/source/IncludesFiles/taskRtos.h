#ifndef TASKRTOS_H_
#define TASKRTOS_H_

typedef enum {
	TASK_ISR_RX_UART0 = 0,
	TASK_ISR_TX_UART0,
	TASK_ISR_TX_UART1,
	TASK_NEXTION,
	TASK_PERIFERICOS_EXTERNOS,
	NUM_TASK=1,
} TaskId_t;

extern void taskRtos_create(void);

#endif /* TASKRTOS_H_ */
