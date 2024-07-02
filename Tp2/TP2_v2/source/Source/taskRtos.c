/*
 * taskRtos.c
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#include "Include/taskRtos.h"
#include "FreeRTOS.h"
#include "task.h"

/*<Archivos de tareas>*/
#include "Include/secuencia.h"
#include "Include/nextion.h"
#include "Include/IntMma.h"
#include "fsl_debug_console.h"

#define task_SECUENCIA_PRIORITY		tskIDLE_PRIORITY + 1
#define task_UART1_PRIORITY			configMAX_PRIORITIES - 1
#define task_INTFF_PRIORITY			configMAX_PRIORITIES - 1
#define task_INTDRDY_PRIORITY		configMAX_PRIORITIES - 2

#define STACK_SIZE_GENERAL		configMINIMAL_STACK_SIZE

typedef struct {
	TaskFunction_t taskFunction;	/*<Nombre de la funcion callback>*/
	const char *taskName;			/*<Nombre como identificador de la tarea>*/
	uint16_t stackDepth;			/*<Stack de la tarea>*/
	void *const pvParameters;		/*<Parametros que se envian de inicializacion>*/
	UBaseType_t priority;			/*<Prioridad de la tarea>*/
} TaskConfig_t;

static const TaskConfig_t taskConfigs[NUM_TASK] = {
	{taskRtos_UART1, "task Uart1", STACK_SIZE_GENERAL, NULL, task_UART1_PRIORITY},
    {taskRtos_INTFF, "task Freefall", STACK_SIZE_GENERAL, NULL, task_INTFF_PRIORITY},
	{taskRtos_INTDRDY, "task Ready", STACK_SIZE_GENERAL, NULL, task_INTDRDY_PRIORITY},
    {taskSecuencia, "task Secuencia", STACK_SIZE_GENERAL+250, NULL, task_SECUENCIA_PRIORITY},
};

extern void taskRtos_create(void) {
//	if (xTaskCreate(taskConfigs[TASK_ISR_UART1].taskFunction, taskConfigs[TASK_ISR_UART1].taskName,
//					taskConfigs[TASK_ISR_UART1].stackDepth,
//					taskConfigs[TASK_ISR_UART1].pvParameters, taskConfigs[TASK_ISR_UART1].priority,
//					NULL) != pdPASS)
//		while(1);
	if (xTaskCreate(taskConfigs[TASK_INT_FF].taskFunction, taskConfigs[TASK_INT_FF].taskName,
					taskConfigs[TASK_INT_FF].stackDepth,
					taskConfigs[TASK_INT_FF].pvParameters, taskConfigs[TASK_INT_FF].priority,
					NULL)!=pdPASS){
		PRINTF("No se pudo crear la tarea\r\n");
		while(1);
	}

	if (xTaskCreate(taskConfigs[TASK_INT_DRDY].taskFunction, taskConfigs[TASK_INT_DRDY].taskName,
					taskConfigs[TASK_INT_DRDY].stackDepth,
					taskConfigs[TASK_INT_DRDY].pvParameters, taskConfigs[TASK_INT_DRDY].priority,
					NULL)!=pdPASS){
		PRINTF("No se pudo crear la tarea\r\n");
		while(1);
	}

	if (xTaskCreate(taskConfigs[TASK_SEC].taskFunction, taskConfigs[TASK_SEC].taskName,
						taskConfigs[TASK_SEC].stackDepth,
						taskConfigs[TASK_SEC].pvParameters, taskConfigs[TASK_SEC].priority,
						NULL)!=pdPASS){
		PRINTF("No se pudo crear la tarea\r\n");
		while(1);
	}


	return;
}

