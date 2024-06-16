#ifndef TASKRTOSPERIFERICOS_H_
#define TASKRTOSPERIFERICOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <semphr.h>

extern void taskRtosPERIFERICOS_Sensor(void *pvParameters);
extern void taskRtosPERIFERICOS_Servo(void *pvParameters);
extern void taskRtosPERIFERICOS_Hcsr04(void *pvParameters);

#endif /* TASKRTOSPERIFERICOS_H_ */
