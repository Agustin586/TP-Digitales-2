#include "IncludesFiles/queueRtos.h"

typedef struct {
	QueueHandle_t xQueueHandle;
	UBaseType_t uxQueueLength;
	UBaseType_t uxItemSize;
} DataQueue_t;

// Arreglo de colas
static DataQueue_t xDataQueues[QUEUE_ID_MAX] = {
#define LENGTH_QUEUE_ID_1	20
#define LENGTH_QUEUE_ID_2	5

		[QUEUE_ID_1] = {.uxQueueLength = LENGTH_QUEUE_ID_1, .uxItemSize = sizeof(char) }, 		/*< Ring buffer del UART1 >*/
		[QUEUE_ID_2] = {.uxQueueLength = LENGTH_QUEUE_ID_2, .uxItemSize = sizeof(uint8_t) }, 	/*< Datos de distancia >*/
};

extern void queueRtos_create(void) {
	// Inicializar cada cola
	for (QueueId_t i = 0; i < QUEUE_ID_MAX; i++) {
		xDataQueues[i].xQueueHandle = xQueueCreate(xDataQueues[i].uxQueueLength,
				xDataQueues[i].uxItemSize);

		/*< Verificamos que la cola se creo correctamente >*/
		if (xDataQueues[i].xQueueHandle == NULL) {
			while(1);
		}
	}

	return;
}

extern BaseType_t queueRtos_sendToQueue(QueueId_t xQueueId, const void *pvItemToQueue, TickType_t xTicksToWait) {
	if (xQueueId < QUEUE_ID_MAX && xDataQueues[xQueueId].xQueueHandle != NULL) {
		return xQueueSend(xDataQueues[xQueueId].xQueueHandle, pvItemToQueue,
				xTicksToWait);
	}

	return pdFAIL;
}

extern BaseType_t queueRtos_receiveFromQueue(QueueId_t xQueueId, void *pvBuffer, TickType_t xTicksToWait) {
	if (xQueueId < QUEUE_ID_MAX && xDataQueues[xQueueId].xQueueHandle != NULL) {
		return xQueueReceive(xDataQueues[xQueueId].xQueueHandle, pvBuffer,
				xTicksToWait);
	}

	return pdFAIL;
}

extern UBaseType_t queueRtos_msgWaiting(QueueId_t xQueueId){
	if (xQueueId < QUEUE_ID_MAX && xDataQueues[xQueueId].xQueueHandle != NULL){
		return uxQueueMessagesWaiting(xDataQueues[xQueueId].QueueHandle_t);
	}

	return pdFALSE;
}
