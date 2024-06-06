/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "IncludesFiles/taskRtos.h"
#include "IncludesFiles/semaphore.h"
#include "IncludesFiles/SD2_board.h"

/*!
 * @brief Application entry point.
 */
int main(void) {
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	board_init();

	semaphore_create();
	taskRtos_create();

	vTaskStartScheduler();
	for (;;)
		;
}
