#include <IncludesFiles/queueRtos.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"

#define MAX_QUEUES 3
#define MAX_DATA_SIZE 50 // Tamaño máximo de los datos a enviar

typedef struct {
    QueueHandle_t handle;
    char name[QUEUE_NAME_SIZE];
} QueueInfo;

static QueueInfo queueList[MAX_QUEUES];

const char *queueDatos_AngDist = "Queue1";
const char *queueUART1 = "Uart1_Rx";

QueueHandle_t queueRtos_create(const char *name, UBaseType_t queueLength) {
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (queueList[i].handle == NULL) {
            queueList[i].handle = xQueueCreate(queueLength, sizeof(QueueData));
            if (queueList[i].handle != NULL) {
                strncpy(queueList[i].name, name, QUEUE_NAME_SIZE - 1);
                queueList[i].name[QUEUE_NAME_SIZE - 1] = '\0';  // Ensure null-termination
                return queueList[i].handle;
            }
        }
    }
    return NULL; // No space left to create a new queue
}

QueueHandle_t queueRtos_getQueueByName(const char *name) {
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (queueList[i].handle != NULL && strncmp(queueList[i].name, name, QUEUE_NAME_SIZE) == 0) {
            return queueList[i].handle;
        }
    }
    return NULL;
}

BaseType_t queueRtos_sendToQueue(QueueHandle_t queue, void *data, size_t dataSize, TickType_t ticksToWait) {
    if (dataSize > MAX_DATA_SIZE) {
        return pdFAIL;  // Error si el tamaño de los datos es mayor que el tamaño máximo permitido
    }

    QueueData queueData;
    memcpy(queueData.data, data, dataSize);
    queueData.size = dataSize;

    return xQueueSend(queue, &queueData, ticksToWait);
}

BaseType_t queueRtos_receiveFromQueue(QueueHandle_t queue, void *buffer, size_t bufferSize, TickType_t ticksToWait) {
    QueueData queueData;
    BaseType_t result = xQueueReceive(queue, &queueData, ticksToWait);

    if (result == pdPASS) {
        if (bufferSize >= queueData.size) {
            memcpy(buffer, queueData.data, queueData.size);
        }
    }
    return result;
}

UBaseType_t queueRtos_msgWaiting(QueueHandle_t queue){
	return uxQueueMessagesWaiting(queue);
}

void deleteQueueByName(const char *name) {
    for (int i = 0; i < MAX_QUEUES; i++) {
        if (queueList[i].handle != NULL && strncmp(queueList[i].name, name, QUEUE_NAME_SIZE) == 0) {
            vQueueDelete(queueList[i].handle);
            queueList[i].handle = NULL;
            queueList[i].name[0] = '\0';
            break;
        }
    }
}
