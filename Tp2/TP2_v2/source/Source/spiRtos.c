/*
 * spiRtos.c
 *
 *  Created on: 1 jul. 2024
 *      Author: aguat
 */

/*< Archivos >*/
#include "Include/spiRtos.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "fsl_spi_freertos.h"
#include "board.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <string.h>

/*< Definiciones >*/
#define BUFFER_SIZE (25)
#define SPI_MASTER_BASE (SPI0_BASE)
#define SPI_MASTER_IRQN (SPI0_IRQn)
#define SPI_MASTER_CLK_SRC (SPI0_CLK_SRC)
#define SPI_MASTER_CLK_FREQ CLOCK_GetFreq((SPI0_CLK_SRC))
#define SPI_MASTER_BASEADDR ((SPI_Type *)SPI_MASTER_BASE)
#define SPI_NVIC_PRIO 2

/*< Variables >*/
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
spi_rtos_handle_t master_rtos_handle;

/*< Funciones>*/
extern void spiRtos_init(void) {
	spi_master_config_t masterConfig;
	uint32_t sourceClock;
	status_t status;

	NVIC_SetPriority(SPI_MASTER_IRQN, SPI_NVIC_PRIO);

	/*
	 * masterConfig.enableStopInWaitMode = false;
	 * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
	 * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
	 * masterConfig.direction = kSPI_MsbFirst;
	 * masterConfig.dataMode = kSPI_8BitMode;
	 * masterConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
	 * masterConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
	 * masterConfig.pinMode = kSPI_PinModeNormal;
	 * masterConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
	 * masterConfig.baudRate_Bps = 500000U;
	 */
	SPI_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = 400000;

	sourceClock = SPI_MASTER_CLK_FREQ;
	status = SPI_RTOS_Init(&master_rtos_handle, SPI_MASTER_BASEADDR,
			&masterConfig, sourceClock);

	if (status != kStatus_Success) {
		PRINTF("DSPI master: error during initialization. \r\n");
		while (1)
			;
	}

	return;
}

extern void spiRtos_xfer(uint8_t *tx_Data, uint8_t *rx_Data) {
	spi_transfer_t masterXfer = { 0 };
	status_t status;

	memcpy(srcBuff, tx_Data, sizeof(tx_Data));

	masterXfer.txData = srcBuff;
	masterXfer.rxData = destBuff;
	masterXfer.dataSize = BUFFER_SIZE;

	status = SPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);

	if (status == kStatus_Success) {
		PRINTF("SPI transfer completed successfully. \r\n");
		rx_Data = masterXfer.rxData;
	} else {
		PRINTF("SPI transfer completed with error. \r\n");
	}

	return;
}
