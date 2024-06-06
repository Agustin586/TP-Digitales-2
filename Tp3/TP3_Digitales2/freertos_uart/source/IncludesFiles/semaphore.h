#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "FreeRTOS.h"

// Enumeración para identificar los semáforos
typedef enum {
	SEMAPHORE_TX_UART0, SEMAPHORE_RX_UART0, SEMAPHORE_TX_UART1, NUM_SEMAPHORES,

} SemaphoreId_t;

extern void semaphore_create(void);
extern BaseType_t semaphore_take(SemaphoreId_t id, TickType_t ticksToWait);
extern BaseType_t semaphore_giveFromISR(SemaphoreId_t id,
		BaseType_t xHigherPriorityTaskWoken);
extern BaseType_t semaphore_give(SemaphoreId_t id);

#endif /* SEMAPHORE_H_ */
