#include "stdio.h"

#include "SD2_board.h"

#include "taskRtosUART0.h"
#include "semaphore.h"
#include "MACROS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART0.h"
#include "mefRecTrama.h"

extern void taskRtosUART0(void) {

	uart0_init();

	mefRecTrama_init();

	for (;;) {
		//if (semaphore_take(SEMAPHORE_UART0, portMAX_DELAY)) {


			mefRecTrama_task();

			vTaskDelay(DELAY_10ms);

		//}
	}

	vTaskDelete(NULL);
	return;
}
