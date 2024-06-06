#include "IncludesFiles/taskRtosUART1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/semaphore.h"

extern void taskRtosUART1_Tx(void *pvParameters) {

	for (;;) {
//		if (semaphore_take(SEMAPHORE_TX_UART1, portMAX_DELAY)) {
//
//		}
		vTaskSuspend(NULL);
	}

	vTaskDelete(NULL);

	return;
}
