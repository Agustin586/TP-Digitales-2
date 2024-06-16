#ifndef DATA_QUEUES_H
#define DATA_QUEUES_H

#include "FreeRTOS.h"
#include "queue.h"

// Enumeración para identificar las colas
typedef enum {
    QUEUE_ID_1,
    QUEUE_ID_2,
    QUEUE_ID_MAX,
} QueueId_t;

/*
 * @brief 	Incializa la cola de datos
 * */
extern void queueRtos_create(void);

/*
 * @brief 	Carga los datos en la cola
 *
 * @param 	QueueId_t xQueueId
 * 			const void *pvItemToQueue
 * 			TickType_t xTicksToWait
 * @return BaseType_t
 * */
extern BaseType_t queueRtos_sendToQueue(QueueId_t xQueueId, const void *pvItemToQueue, TickType_t xTicksToWait);

/*
 * @brief 	Carga los datos en la cola
 *
 * @param 	QueueId_t xQueueId
 * 			void *pvBuffer
 * 			TickType_t xTicksToWait
 * @return BaseType_t
 * */
extern BaseType_t queueRtos_receiveFromQueue(QueueId_t xQueueId, void *pvBuffer, TickType_t xTicksToWait);

/*
 * @brief 	Pregunta si hay algun dato cargado en la cola
 *
 * @param	QueueId_t xQueueId
 * @return	UBaseType_t
 * */
extern UBaseType_t queueRtos_msgWaiting(QueueId_t xQueueId);

#endif // DATA_QUEUES_H
