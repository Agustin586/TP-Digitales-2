#include "IncludesFiles/taskRtos.h"

#include "FreeRTOS.h"
#include "task.h"

#include "IncludesFiles/taskRtosUART0.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "fsl_debug_console.h"

#define UART0_RX_PRIORITY		configMAX_PRIORITIES - 1
#define UART0_TX_PRIORITY		configMAX_PRIORITIES - 1
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
/*
 * NOTA: NUM_TASK LO PUSE EN 3 APROPOSITOOOOOOOO!!!!!!!
 *
 * */
static const TaskConfig_t taskConfigs[NUM_TASK] = {
//    {taskRtosUART0_Rx, "ISR_RX_U0", STACK_SIZE_GENERAL, UART0_RX_PRIORITY},
//    {taskRtosUART0_Tx, "ISR_TX_U0", STACK_SIZE_GENERAL, UART0_TX_PRIORITY},
    {(void (*)(void*))taskRtosNextion, "Nextion", STACK_SIZE_GENERAL+1000, NULL, NEXTION_PRIORITY+1},
//    {(void (*)(void*))taskRtosPERIFERICOS_Sensor, "Sensor", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
//	{(void (*)(void*))taskRtosPERIFERICOS_Servo, "Servo", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
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

