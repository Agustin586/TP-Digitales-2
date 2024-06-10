#include "IncludesFiles/taskRtosUART1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/semaphore.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/queueRtos.h"
#include "fsl_debug_console.h"

extern void taskRtosUART1_Rx(void *pvParameters) {
	PRINTF("Tarea: Uart1\r\n");
#define LONG_RINGBUFFER_RX_UART1	20
#define MAX_READ_LENGH	1

	const char *queue_Uart1 = "Uart1_Rx";
	char bufferRx[2];
	bool page = false;

	Uart1_init();

	PRINTF("Uart 1 Inicializado\r\n");

	for (;;) {
		if (Uart1_read((uint8_t *)bufferRx, MAX_READ_LENGH)
				== kStatus_Success) {
			if (bufferRx[0] == 0x66 || page) {
				page = true;

				if (bufferRx[0] != 0x66) {
					queueRtos_sendToQueue(queueRtos_getQueueByName(queue_Uart1),
							bufferRx, MAX_READ_LENGH, pdMS_TO_TICKS(500));
					page = false;
				}
			}

			/*Faltaria poner un semaforo pero es para probar*/
		}
		vTaskDelay(DELAY_100ms);
	}

	vTaskDelete(NULL);

	return;
}
