#include "IncludesFiles/taskRtos.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "IncludesFiles/taskRtosUART0.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/taskRtosUART1.h"
#include "fsl_debug_console.h"

#define UART0_PRIORITY_TX_RX	configMAX_PRIORITIES - 1
#define NEXTION_PRIORITY		tskIDLE_PRIORITY + 1
#define PERIFERICOS_PRIORITY	tskIDLE_PRIORITY + 1
#define STACK_SIZE_GENERAL		configMINIMAL_STACK_SIZE
#define UART1_PRIORITY_TX_RX	configMAX_PRIORITIES - 1

typedef struct {
	TaskFunction_t taskFunction;
	const char *taskName;
	uint16_t stackDepth;
	void *const pvParameters;
	UBaseType_t priority;
} TaskConfig_t;

static const TaskConfig_t taskConfigs[NUM_TASK] = {
	{taskRtosUART1_Rx, "UART1_task", STACK_SIZE_GENERAL+50, NULL, UART1_PRIORITY_TX_RX},
    {taskRtosUART0, "UART0_task", STACK_SIZE_GENERAL, NULL, UART0_PRIORITY_TX_RX},
    {taskRtosNextion, "Nextion_task", STACK_SIZE_GENERAL+300, NULL, NEXTION_PRIORITY},
    {taskRtosPERIFERICOS_Sensor, "Sensor_task", STACK_SIZE_GENERAL+100, NULL, PERIFERICOS_PRIORITY},
	{taskRtosPERIFERICOS_Servo, "Servo_task", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
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

