/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    TP2_DIGITALES2.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <semphr.h>

#include "tareasRtos.h"
#include "MACROS.h"
#include "mma8451.h"
#include "mefSEC.h"

#define INT1_PORT       PORTC
#define INT1_GPIO       GPIOC
#define INT1_PIN        5

#define INT2_PORT       PORTD
#define INT2_GPIO       GPIOD
#define INT2_PIN        1

#define QUEUE_LONGITUD	3

/* TODO: insert other definitions and declarations here. */
#define mainSYSTICK_SOFTWARE_TIMER_PERIOD	pdMS_TO_TICKS(10)

/*
 * @brief Configura las tareas de freertos
 * */
static void Tareas_init(void);

/*
 * @brief Configura los timers por software en freertos
 * 		Timer 1 --> De 1 ms para detectar las pulsaciones
 * 					Tipo autoreload.
 * */
static void Timers_init(void);

/*
 * @brief Software Timer que represente al SysTick
 * 		NOTA: No es util ahora que se usa un RTOS.
 * */
static void Software_SysTick(void);

/*
 * @brief Configura los semaforos necesarios
 * 		Binario --> Para la interrupcion por freefall
 * */
static void Semaphore_init(void);

/*
 * @brief Inicializa las colas de datos que va a utilizar
 * */
static void Queue_init(void);

xSemaphoreHandle FreefallSemaphore,DrydSemaphore;
xQueueHandle queue_NormaMaxima,queueNormaRaiz;

/*
 * @brief   Application entry point.
 */
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* INICIALIZACIÓN DE MÓDULOS */
    board_init();
	key_init();
	mma8451_init();

	/* INICIALIZACIÓN DE FREERTOS */
    Tareas_init();
    Semaphore_init();
    Timers_init();
    Queue_init();

    vTaskStartScheduler();

    for (;;);

    return 0 ;
}

static void Tareas_init(void){
	/* TAREA DE MEF */
	if (xTaskCreate(tareasRtos_TaskMEF, "Tarea MEF", STACK_SIZE+200, NULL, TASK_MEF_PRIORITY, NULL) != pdPASS)
		PRINTF("No se pudo crear tarea de MEF");

	/* TAREA DE DISPLAY */
	if (xTaskCreate(tareasRtos_TaskDisplay, "Tarea Display", STACK_SIZE+200, NULL, TASK_DISPLAY_PRIORITY, NULL) != pdPASS)
		PRINTF("No se pudo crear Tarea display");

	return;
}

static void Semaphore_init(void){
	FreefallSemaphore = xSemaphoreCreateBinary();

	if (FreefallSemaphore != NULL) {
		xTaskCreate(tareasRtos_Freefall_Interrupt, "Interrupcion Freefall", STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	}

	DrydSemaphore = xSemaphoreCreateBinary();

	if (DrydSemaphore != NULL) {
		xTaskCreate(tareasRtos_TaskRxMMA8451, "Recepcion Continua", STACK_SIZE+200, NULL, configMAX_PRIORITIES-1, NULL);
	}

	return;
}

void PORTC_PORTD_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* INTERRUPCION POR DATOS LISTOS */
    if (PORT_GetPinsInterruptFlags(INT1_PORT)){
    	xSemaphoreGiveFromISR((QueueHandle_t) DrydSemaphore, &xHigherPriorityTaskWoken);
    	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptOrDMADisabled);
    	PORT_ClearPinsInterruptFlags(INT1_PORT, 1 << INT1_PIN);
    }

    /* INTERRUPCION POR FREEFALL */
    if (PORT_GetPinsInterruptFlags(INT2_PORT)){
    	xSemaphoreGiveFromISR((QueueHandle_t) FreefallSemaphore, &xHigherPriorityTaskWoken);
    	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptOrDMADisabled);
    	PORT_ClearPinsInterruptFlags(INT2_PORT, 1 << INT2_PIN);	// Limpia bandera de interrupcion 2
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    /* ====================================================================================================================
     * NOTA: Detectamos la interrupcion por algunos de los casos, pero a la vez deshabilitamos las interrupciones por ese
     * pin hasta que sea procesada la interrupcion en la mefAcelerometro.
     * ====================================================================================================================
     *  */

    /* ====================================================================================================================
	 * NOTA 2: Debido a problemas que se presentaron y que tuvieron solución desde las 7 de la mañana se tuvo que corregir
	 * la lógica de la obtención de datos y ahora se toman directamente sobre este interrupción. Resultados: anda mucho
	 * que la otra lógica. Posdata: le pegué tremenda pata a la pared. Posposdata: Quiero mis cariñosas. Saludos.
	 * ====================================================================================================================
	 *  */

    return;
}

static void Queue_init(void){
	queue_NormaMaxima = xQueueCreate(QUEUE_LONGITUD, sizeof(uint32_t));

	if (queue_NormaMaxima == NULL)
		PRINTF("No se pudo crear la cola de datos\r\n");

//	queueNormaRaiz = xQueueCreate(QUEUE_LONGITUD, sizeof(uint32_t));

	return;
}

static void Timers_init(void){
	TimerHandle_t Soft_SysTick;

	/* NOTA: El uso de un Software SysTick ahora no tiene sentido, ya que los retardos y el mismo key se encuentran
	 * bien implementados para no tener que realizar una evaluación periódica de estados.
	 * */
	Soft_SysTick = xTimerCreate("Timer 10 ms", mainSYSTICK_SOFTWARE_TIMER_PERIOD, pdTRUE, NULL, (TimerCallbackFunction_t) Software_SysTick);

	if (xTimerStart(Soft_SysTick, 0) == pdPASS)
		PRINTF("Inicia el Timer\r\n");

	return;
}

static void Software_SysTick(void){
	mefSEC_periodicTask1ms();

	return;
}
