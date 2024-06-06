#ifndef COLADATOS_H_
#define COLADATOS_H_

#include "FreeRTOS.h"

typedef enum {
	QUEUE_TX_UART1 = 0, NUM_QUEUE,

} QueueId_t;

//extern void queue_create(void);
//extern BaseType_t queue_sentFromBack(QueueId_t id, TickType_t ticksToWait);

#endif /* COLADATOS_H_ */
