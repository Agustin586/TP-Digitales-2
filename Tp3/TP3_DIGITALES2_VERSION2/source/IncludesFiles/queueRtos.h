#ifndef COLADATOS_H_
#define COLADATOS_H_

#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_NAME_SIZE 32
#define MAX_DATA_SIZE 1  // Tamaño máximo de los datos a enviar

typedef struct {
    uint8_t data[MAX_DATA_SIZE];
    size_t size;
} QueueData;

QueueHandle_t queueRtos_create(const char *name, UBaseType_t queueLength);
QueueHandle_t queueRtos_getQueueByName(const char *name);
BaseType_t queueRtos_sendToQueue(QueueHandle_t queue, void *data, size_t dataSize, TickType_t ticksToWait);
BaseType_t queueRtos_receiveFromQueue(QueueHandle_t queue, void *buffer, size_t bufferSize, TickType_t ticksToWait);
UBaseType_t queueRtos_msgWaiting(QueueHandle_t queue);
void deleteQueueByName(const char *name);

#endif /* COLADATOS_H_ */
