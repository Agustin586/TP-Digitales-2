#include "IncludesFiles/taskRtos.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "IncludesFiles/taskRtosUART0.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/taskRtosPERIFERICOS.h"
#include "IncludesFiles/taskRtosUART1.h"
#include "IncludesFiles/taskRtosAudio.h"
#include "fsl_debug_console.h"

#define UART0_PRIORITY_TX_RX	configMAX_PRIORITIES - 1
#define NEXTION_PRIORITY		tskIDLE_PRIORITY + 1
#define PERIFERICOS_PRIORITY	tskIDLE_PRIORITY + 1
#define STACK_SIZE_GENERAL		configMINIMAL_STACK_SIZE
#define UART1_PRIORITY_TX_RX	configMAX_PRIORITIES - 1

typedef struct {
	TaskFunction_t taskFunction;	/*<Nombre de la funcion callback>*/
	const char *taskName;			/*<Nombre como identificador de la tarea>*/
	uint16_t stackDepth;			/*<Stack de la tarea>*/
	void *const pvParameters;		/*<Parametros que se envian de inicializacion>*/
	UBaseType_t priority;			/*<Prioridad de la tarea>*/
} TaskConfig_t;

static const TaskConfig_t taskConfigs[NUM_TASK] = {
	{taskRtosUART1, "UART1_task", STACK_SIZE_GENERAL+50, NULL, UART1_PRIORITY_TX_RX},
    {taskRtosUART0, "UART0_task", STACK_SIZE_GENERAL, NULL, UART0_PRIORITY_TX_RX},
    {taskRtosNextion, "Nextion_task", STACK_SIZE_GENERAL+300, NULL, NEXTION_PRIORITY},
    {taskRtosPERIFERICOS_Sensor, "Sensor_task", STACK_SIZE_GENERAL+100, NULL, PERIFERICOS_PRIORITY},
	{taskRtosPERIFERICOS_Servo, "Servo_task", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
	{taskRtosPERIFERICOS_Hcsr04, "HCSR04_task", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
//	{taskRtosAudio, "Audio_task", STACK_SIZE_GENERAL, NULL, PERIFERICOS_PRIORITY},
};

extern void taskRtos_create(void) {
	if (xTaskCreate(taskConfigs[TASK_ISR_UART1].taskFunction, taskConfigs[TASK_ISR_UART1].taskName,
					taskConfigs[TASK_ISR_UART1].stackDepth,
					taskConfigs[TASK_ISR_UART1].pvParameters, taskConfigs[TASK_ISR_UART1].priority,
					NULL) != pdPASS)
		while(1);
	if (xTaskCreate(taskConfigs[TASK_ISR_UART0].taskFunction, taskConfigs[TASK_ISR_UART0].taskName,
					taskConfigs[TASK_ISR_UART0].stackDepth,
					taskConfigs[TASK_ISR_UART0].pvParameters, taskConfigs[TASK_ISR_UART0].priority,
					NULL)!=pdPASS)
		while(1);
	if (xTaskCreate(taskConfigs[TASK_NEXTION].taskFunction, taskConfigs[TASK_NEXTION].taskName,
					taskConfigs[TASK_NEXTION].stackDepth,
					taskConfigs[TASK_NEXTION].pvParameters, taskConfigs[TASK_NEXTION].priority,
					NULL)!=pdPASS)
		while(1);
	if (xTaskCreate(taskConfigs[TASK_PERIFERICOS_SENSOR].taskFunction, taskConfigs[TASK_PERIFERICOS_SENSOR].taskName,
					taskConfigs[TASK_PERIFERICOS_SENSOR].stackDepth,
					taskConfigs[TASK_PERIFERICOS_SENSOR].pvParameters, taskConfigs[TASK_PERIFERICOS_SENSOR].priority,
					NULL)!=pdPASS)
		while(1);
	if (xTaskCreate(taskConfigs[TASK_PERIFERICOS_SERVO].taskFunction, taskConfigs[TASK_PERIFERICOS_SERVO].taskName,
					taskConfigs[TASK_PERIFERICOS_SERVO].stackDepth,
					taskConfigs[TASK_PERIFERICOS_SERVO].pvParameters, taskConfigs[TASK_PERIFERICOS_SERVO].priority,
					NULL)!=pdPASS)
		while(1);
	if (xTaskCreate(taskConfigs[TASK_PERIFERICOS_HCSR04].taskFunction, taskConfigs[TASK_PERIFERICOS_HCSR04].taskName,
					taskConfigs[TASK_PERIFERICOS_HCSR04].stackDepth,
					taskConfigs[TASK_PERIFERICOS_HCSR04].pvParameters, taskConfigs[TASK_PERIFERICOS_HCSR04].priority,
					NULL)!=pdPASS)
		while(1);

	return;
}

