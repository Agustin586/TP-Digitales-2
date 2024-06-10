#include "IncludesFiles/taskRtos.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "IncludesFiles/taskRtosUART0.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "fsl_debug_console.h"

#define UART0_PRIORITY_TX_RX	configMAX_PRIORITIES - 1
#define NEXTION_PRIORITY		tskIDLE_PRIORITY + 1
#define PERIFERICOS_PRIORITY	tskIDLE_PRIORITY + 1
#define STACK_SIZE_GENERAL		configMINIMAL_STACK_SIZE

typedef struct {
	TaskFunction_t taskFunction;
	const char *taskName;
	uint16_t stackDepth;
	void *pvParameters;
	UBaseType_t priority;
} TaskConfig_t;

static const TaskConfig_t taskConfigs[NUM_TASK] = {
    {(void (*)(void*))taskRtosUART0, "UART0_task", STACK_SIZE_GENERAL+200, UART0_PRIORITY_TX_RX},
    {(void (*)(void*))taskRtosNextion, "Nextion", STACK_SIZE_GENERAL+500, NULL, NEXTION_PRIORITY},
    {(void (*)(void*))taskRtosPERIFERICOS_Sensor, "Sensor", STACK_SIZE_GENERAL+300, NULL, PERIFERICOS_PRIORITY},
	{(void (*)(void*))taskRtosPERIFERICOS_Servo, "Servo", STACK_SIZE_GENERAL+150, NULL, PERIFERICOS_PRIORITY},
};

extern void taskRtos_create(void) {
	for (int i = 0; i < NUM_TASK; i++) {
		xTaskCreate(taskConfigs[i].taskFunction, taskConfigs[i].taskName,
				taskConfigs[i].stackDepth,
				taskConfigs[i].pvParameters, taskConfigs[i].priority,
				NULL);
	}

	return;
}

