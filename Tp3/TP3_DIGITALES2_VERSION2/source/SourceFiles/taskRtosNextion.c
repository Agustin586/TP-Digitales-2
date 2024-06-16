#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/MACROS.h"
#include "IncludesFiles/Uart1.h"
#include "IncludesFiles/mefNextion.h"
#include "IncludesFiles/mefUart1Rec.h"
#include "task.h"
#include "fsl_debug_console.h"

extern void taskRtosNextion(void *pvParameters) {
	PRINTF("Tarea: Manejo de pantalla Nextion\r\n");

	mefNextion_init();
	mefUart1Rec_init();

	vTaskDelay(DELAY_2s);

	for (;;) {
		mefNextion();
		mefUart1Rec();

		vTaskDelay(DELAY_10ms);
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtosNextion_error(void) {
	vTaskSuspend(NULL);

	return;
}
