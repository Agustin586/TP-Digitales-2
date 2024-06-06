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
 * @file    TP3_DIGITALES2_VERSION2.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_tpm.h"

/* TODO: insert other include files here. */
#include "IncludesFiles/taskRtos.h"
#include "IncludesFiles/semaphore.h"
#include "IncludesFiles/SD2_board.h"
#include "FreeRTOS.h"
#include "task.h"

/* TODO: insert other definitions and declarations here. */

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

	board_init();

	semaphore_create();
	taskRtos_create();

	vTaskStartScheduler();

	for (;;)
		;

	return 0;
}




//#define UART UART1
//#define UART_CLKSRC kCLOCK_BusClk
//#define UART_CLK_FREQ CLOCK_GetFreq(UART_CLKSRC)
//#define UART_RX_TX_IRQn UART1_RX_TX_IRQn
//#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

//PRINTF("Hello World\n");
//
//	uart_config_t config;
//
//	CLOCK_EnableClock(kCLOCK_Uart1);
//
//	UART_GetDefaultConfig(&config);
//	config.baudRate_Bps = 115200;
//	config.enableTx = true;
//	config.enableRx = true;
//
//	UART_Init(UART, &config, UART_CLK_FREQ);
//
//	UART_WriteBlocking(UART, (uint8_t*) "Hola Uart1 Anda bien", 1);
//
//	tpm_config_t tpmInfo;
//	tpm_chnl_pwm_signal_param_t tpmParam;
//
//	CLOCK_SetTpmClock(1U); // Selecciona el reloj para TPM
//
//	// Configuración del PWM
//	tpmParam.chnlNumber = kTPM_Chnl_4; // Canal 4 para PTE31
//	tpmParam.level = kTPM_HighTrue;
//	tpmParam.dutyCyclePercent = 50U; // Ciclo de trabajo del 50%
//
//	TPM_GetDefaultConfig(&tpmInfo);
//	tpmInfo.prescale = kTPM_Prescale_Divide_128;
//
//	TPM_Init(TPM0, &tpmInfo);
//
//	// Configurar PWM en el canal 4 de TPM0
//	TPM_SetupPwm(TPM0, &tpmParam, 1U, kTPM_EdgeAlignedPwm, 50U,
//			TPM_SOURCE_CLOCK);
//	TPM_StartTimer(TPM0, kTPM_SystemClock);
//	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_4, kTPM_EdgeAlignedPwm, 25);
//
//	while (1) {
//
//	}
