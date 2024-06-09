#include "stdio.h"

#include "IncludesFiles/SD2_board.h"

#include "IncludesFiles/taskRtosUART0.h"
#include "IncludesFiles/semaphore.h"
#include "IncludesFiles/MACROS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "IncludesFiles/mefRecTrama.h"
#include "IncludesFiles/uart0_dma.h"

extern void taskRtosUART0(void *pvParameter) {
	uart0_init();

	mefRecTrama_init();

	for (;;) {
		//if (semaphore_take(SEMAPHORE_UART0, portMAX_DELAY)) {
		mefRecTrama_task();

		vTaskDelay(DELAY_100ms);

		//}
	}

	vTaskDelete(NULL);
	return;
}
