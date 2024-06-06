#include "IncludesFiles/semaphore.h"

#include "queue.h"
#include "semphr.h"

#include <stdint.h>

static SemaphoreHandle_t semaphores[NUM_SEMAPHORES];

extern void semaphore_create(void) {
	for (uint8_t semp = 0; semp < NUM_SEMAPHORES; semp++) {
		semaphores[semp] = xSemaphoreCreateBinary();
		if (semaphores[semp] == NULL) {
			// Error al crear el semaforo
		}
	}

	return;
}

extern BaseType_t semaphore_take(SemaphoreId_t id, TickType_t ticksToWait) {
	if (id < NUM_SEMAPHORES && semaphores[id] != NULL) {
		return xSemaphoreTake(semaphores[id], ticksToWait);
	}
	return pdFALSE;
}

extern BaseType_t semaphore_giveFromISR(SemaphoreId_t id,
		BaseType_t xHigherPriorityTaskWoken) {
	if (id < NUM_SEMAPHORES && semaphores[id] != NULL) {
		return xSemaphoreGiveFromISR(semaphores[id], &xHigherPriorityTaskWoken);
	}
	return pdFALSE;
}

extern BaseType_t semaphore_give(SemaphoreId_t id) {
	if (id < NUM_SEMAPHORES && semaphores[id] != NULL) {
		return xSemaphoreGive(semaphores[id]);
	}
	return pdFALSE;
}
