#include "IncludesFiles/taskRtosAudio.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"

extern void taskRtosAudio(void *pvParameters){
	PRINTF("Tarea: Audio\r\n");

	for(;;){
		vTaskDelay(pdMS_TO_TICKS(100));
	}

	return;
}
