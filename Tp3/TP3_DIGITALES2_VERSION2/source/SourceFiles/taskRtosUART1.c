#include "IncludesFiles/taskRtosUART1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/semaphore.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/queueRtos.h"
#include "fsl_debug_console.h"

extern void taskRtosUART1(void *pvParameters) {
#define MAX_READ_LENGH	1	/*< Cargo de a un dato >*/

	PRINTF("Tarea: Uart1\r\n");

	char bufferRx[2];
	size_t cant_bytes;

	Uart1_init();

	for (;;) {
		if (Uart1_read((uint8_t*) bufferRx, MAX_READ_LENGH, &cant_bytes)
				== kStatus_Success) {
			queueRtos_sendToQueue(QUEUE_ID_1, bufferRx, pdMS_TO_TICKS(200));
		}
	}

	vTaskDelete(NULL);

	return;
}
