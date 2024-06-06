#ifndef TASKRTOSPERIFERICOS_H_
#define TASKRTOSPERIFERICOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <semphr.h>

extern void taskRtosPERIFERICOS_Sensor(void);
extern void taskRtosPERIFERICOS_Servo(void);
extern void taskRtosPERIFERICOS_delayServo(void);

#endif /* TASKRTOSPERIFERICOS_H_ */
