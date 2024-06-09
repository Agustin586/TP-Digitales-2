/* Copyright 2022, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2022, 2019, 2018, Gustavo Muro - Daniel Márquez
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/

// Standard C Included Files
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Project Included Files
#include "SD2_board.h"
#include "fsl_lpsci.h"
#include "fsl_port.h"
#include "fsl_lpsci_dma.h"
#include "fsl_dmamux.h"
#include "board.h"
#include "MKL46Z4.h"
#include "pin_mux.h"
#include "uart0_dma.h"

#include "FreeRTOS.h"
#include "queue.h"

/*==================[macros and definitions]=================================*/
#define LPSCI_TX_DMA_CHANNEL 0U

/*==================[internal data declaration]==============================*/
static lpsci_dma_handle_t lpsciDmaHandle;
static dma_handle_t lpsciTxDmaHandle;
volatile bool txOnGoing = false;

static QueueHandle_t  Rx_Queue;
static QueueHandle_t  Tx_Queue;

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* UART user callback */
static void LPSCI_UserCallback(UART0_Type *base, lpsci_dma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPSCI_TxIdle == status)
    {
        txOnGoing = false;
        LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
    }
}


/*==================[external functions definition]==========================*/

void uart0_init(void)
{
    lpsci_config_t config;

    Rx_Queue = xQueueCreate(16, sizeof(uint8_t));
    Tx_Queue = xQueueCreate(16, sizeof(uint8_t));

    CLOCK_SetLpsci0Clock(0x1U);

    /* PORTA1 (pin 35) is configured as UART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 36) is configured as UART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /*
     * config.parityMode = kLPSCI_ParityDisabled;
     * config.stopBitCount = kLPSCI_OneStopBit;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.parityMode = kLPSCI_ParityDisabled;
    config.stopBitCount = kLPSCI_OneStopBit;
    config.enableTx = true;
    config.enableRx = true;

    LPSCI_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    /* Habilita interrupciones */
    //LPSCI_EnableInterrupts(UART0, kLPSCI_TransmissionCompleteInterruptEnable);
    LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
    LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
    EnableIRQ(UART0_IRQn);

    /* CONFIGURACIÓN DMA */
    /* Init DMAMUX */
    DMAMUX_Init(DMAMUX0);

    /* Set channel for LPSCI  */
    DMAMUX_SetSource(DMAMUX0, LPSCI_TX_DMA_CHANNEL, kDmaRequestMux0LPSCI0Tx);
    DMAMUX_EnableChannel(DMAMUX0, LPSCI_TX_DMA_CHANNEL);

    /* Init the DMA module */
    DMA_Init(DMA0);
    DMA_CreateHandle(&lpsciTxDmaHandle, DMA0, LPSCI_TX_DMA_CHANNEL);

    /* Create LPSCI DMA handle. */
    LPSCI_TransferCreateHandleDMA(
            UART0,
            &lpsciDmaHandle,
            LPSCI_UserCallback,
            NULL,
            &lpsciTxDmaHandle,
            NULL);
}


/** \brief envía datos por puerto serie accediendo a memoria RAM
 **
 ** \param[inout] pBuf buffer a donde estan los datos a enviar
 ** \param[in] size tamaño del buffer
 ** \return cantidad de bytes enviados
 **/
int32_t uart0_envDatos(uint8_t *pBuf, int32_t size)
{
    int32_t ret = 0;

    /* entra sección de código crítico */
    NVIC_DisableIRQ(UART0_IRQn);

    /* si la cola estaba vacía hay que habilitar la int TX */
    if (uxQueueMessagesWaiting(Tx_Queue) == 0) //ringBuffer_isEmpty(pRingBufferTx))
    	LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);

    while (uxQueueSpacesAvailable(Tx_Queue) && ret < size) //!ringBuffer_isFull(pRingBufferTx) && ret < size)
    {
    	xQueueSend(Tx_Queue, &pBuf[ret], NULL);  //ringBuffer_putData(pRingBufferTx, pBuf[ret]);
        ret++;
    }

    /* sale de sección de código crítico */
    NVIC_EnableIRQ(UART0_IRQn);

    return ret;
}


int32_t uart0_recDatos(uint8_t *pBuf, int32_t size)
{
    int32_t ret = 0;

    /* entra sección de código crítico */
    NVIC_DisableIRQ(UART0_IRQn);

    while (uxQueueMessagesWaiting(Rx_Queue) && ret < size)
    {
    	uint8_t dato;

        //ringBuffer_getData(pRingBufferRx, &dato);
        xQueueReceive(Rx_Queue, &dato, portMAX_DELAY);

        pBuf[ret] = dato;

        ret++;
    }

    /* sale de sección de código crítico */
    NVIC_EnableIRQ(UART0_IRQn);

    return ret;
}

extern void uart0_envByte(uint8_t byte)
{
    /* entra sección de código crítico */
    NVIC_DisableIRQ(UART0_IRQn);

    /* si la cola estaba vacía hay que habilitar la int TX */
    if (uxQueueMessagesWaiting(Tx_Queue) == 0) //ringBuffer_isEmpty(pRingBufferTx))
    	LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);

    if (uxQueueSpacesAvailable(Tx_Queue)) //!ringBuffer_isFull(pRingBufferTx) && ret < size)
    {
    	xQueueSend(Tx_Queue, &byte, NULL);  //ringBuffer_putData(pRingBufferTx, pBuf[ret]);
    }

    /* sale de sección de código crítico */
    NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void)
{
	uint8_t data;

	lpsci_transfer_t transferido = {&data , 1};


	if ( (kLPSCI_RxDataRegFullFlag)            & LPSCI_GetStatusFlags(UART0) &&
	   (kLPSCI_RxDataRegFullInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
	{
	        /* obtiene dato recibido por puerto serie */
		    data = LPSCI_ReadByte(UART0);

			/* pone dato en cola */
		    xQueueSendFromISR(Rx_Queue, &data, NULL);//ringBuffer_putData(pRingBufferRx, data);

			LPSCI_ClearStatusFlags(UART0, kLPSCI_RxDataRegFullFlag);
	}
/*
    if ( (kLPSCI_TransmissionCompleteFlag)            & LPSCI_GetStatusFlags(UART0) &&
         (kLPSCI_TransmissionCompleteInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
    {
        LPSCI_DisableInterrupts(UART0, kLPSCI_TransmissionCompleteInterruptEnable);
        LPSCI_ClearStatusFlags(UART0, kLPSCI_TransmissionCompleteFlag);

        //Enciende led verde para indicar finalización de transmisión por UART0
        board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);

    }
*/
	if ( (kLPSCI_TxDataRegEmptyFlag)            & LPSCI_GetStatusFlags(UART0) &&
	         (kLPSCI_TxDataRegEmptyInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
	{
		/*
			if (txOnGoing == false)
			{
				if(xQueueReceiveFromISR(Tx_Queue, &data, portMAX_DELAY)){
					// envía dato extraído del RB al puerto serie
					transferido.data = &data;
					transferido.dataSize = 1;

					txOnGoing = true;
					LPSCI_TransferSendDMA(UART0, &lpsciDmaHandle, &transferido);
					//LPSCI_WriteByte(UART0, data);
				}
			}
			*/
			if(!txOnGoing && uxQueueMessagesWaiting(Tx_Queue)){

					xQueueReceiveFromISR(Tx_Queue, &data, portMAX_DELAY);

						//LPSCI_WriteByte(UART0, data);
					transferido.data = &data;
					transferido.dataSize = 1;

					txOnGoing = true;
					LPSCI_TransferSendDMA(UART0, &lpsciDmaHandle, &transferido);
			}
			else
			{
				/* si el RB está vacío deshabilita interrupción TX */
			    LPSCI_DisableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
			}

			LPSCI_ClearStatusFlags(UART0, kLPSCI_TxDataRegEmptyFlag);
	}

}


/*==================[end of file]============================================*/

