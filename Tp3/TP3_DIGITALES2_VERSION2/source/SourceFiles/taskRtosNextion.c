#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/mefNextion.h"
#include "task.h"
#include "fsl_debug_console.h"

extern void taskRtosNextion(void) {
	PRINTF("Tarea: Manejo de pantalla Nextion\r\n");

	Uart1_init();
	mefNextion_init();

	vTaskDelay(DELAY_3s);

	for (;;) {
		mefNextion();

		vTaskDelay(DELAY_10ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosNextion_error(void) {
	vTaskSuspend(NULL);

	return;
}

extern void taskRtosNextion_delay(uint16_t delay) {
	vTaskDelay(pdMS_TO_TICKS(delay));

	return;
}

