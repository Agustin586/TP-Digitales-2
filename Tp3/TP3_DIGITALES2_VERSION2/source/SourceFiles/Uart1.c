// Librería UART1_DMA.c
#include "IncludesFiles/Uart1.h"

// Declaraciones de variables globales
dma_handle_t dmaTxHandle;
uart_handle_t uartHandle;
QueueHandle_t uartTxQueue;
QueueHandle_t uartRxQueue;

static volatile bool txOnGoing = false;

#define TX_DMA_CHANNEL 1U

static void UART1_DMA_TxCallback(dma_handle_t *handle, void *userData) {

	txOnGoing = false;
	UART_EnableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);

}


extern void Uart1_init(void) {
    // Crear colas para transmitir y recibir datos
    uartTxQueue = xQueueCreate(30, sizeof(uint8_t));
    uartRxQueue = xQueueCreate(30, sizeof(uint8_t));

    // Configurar la UART1
    uart_config_t uartConfig;
    UART_GetDefaultConfig(&uartConfig);
    uartConfig.baudRate_Bps = 115200;
    uartConfig.enableTx = true;
    uartConfig.enableRx = true;
    UART_Init(UART1, &uartConfig, CLOCK_GetFreq(kCLOCK_BusClk));


    /* CONFIGURACIÓN DMA */
    /* Init DMAMUX */
    DMAMUX_Init(DMAMUX0);

    /* Set channel for LPSCI  */
    DMAMUX_SetSource(DMAMUX0, TX_DMA_CHANNEL, kDmaRequestMux0UART1Tx);
    DMAMUX_EnableChannel(DMAMUX0, TX_DMA_CHANNEL);

    // Inicializar el DMA
    DMA_Init(DMA0);

    // Configurar el handle del DMA para TX
    DMA_CreateHandle(&dmaTxHandle, DMA0, TX_DMA_CHANNEL);

    DMA_SetCallback(&dmaTxHandle, UART1_DMA_TxCallback, NULL);

    // Configurar el handle de la UART1
    UART_TransferCreateHandle(UART1, &uartHandle, NULL, NULL);

    // Habilitar interrupción por transmisión vacía (TX)
    UART_EnableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable);

    NVIC_EnableIRQ(UART1_IRQn);
}

extern void Uart1_send(char* to_send) {

	uint32_t size = strlen(to_send), ret = 0;
    // Colocar los datos en la cola para ser transmitidos

	NVIC_DisableIRQ(UART1_IRQn);

	if (uxQueueMessagesWaiting(uartTxQueue) == 0)
			UART_EnableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);


	while (uxQueueSpacesAvailable(uartTxQueue) && ret < size) {
			xQueueSendToBack(uartTxQueue, &to_send[ret], pdMS_TO_TICKS(100));
			ret++;
		}

	NVIC_EnableIRQ(UART1_IRQn);
}

extern void Uart1_read(uint8_t *receive) {
	int32_t ret = 0;

	/* entra sección de código crítico */
	NVIC_DisableIRQ(UART1_IRQn);

	while (uxQueueMessagesWaiting(uartRxQueue)) {
		uint8_t dato;

		xQueueReceive( uartRxQueue, &dato, portMAX_DELAY);
		receive[ret] = dato;
		ret++;
	}

	/* sale de sección de código crítico */
	NVIC_EnableIRQ(UART1_IRQn);
}

void UART1_IRQHandler() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t flags = UART_GetStatusFlags(UART1);

    // Manejar interrupción por transmisión vacía (TX)
    if (flags & kUART_TxDataRegEmptyFlag) {

        UART_ClearStatusFlags(UART1, kUART_TxDataRegEmptyFlag);
        uint8_t data;

        if(!txOnGoing && uxQueueMessagesWaiting(uartTxQueue)){

				xQueueReceiveFromISR(uartTxQueue, &data, &xHigherPriorityTaskWoken);
				UART_WriteByte(UART1, data);
				txOnGoing = true;

        }

        else {
        			/* si el RB está vacío deshabilita interrupción TX */
        			UART_DisableInterrupts(UART1,
        					kUART_TxDataRegEmptyInterruptEnable);
        }

        UART_DisableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);
    }

    // Manejar interrupción por recepción de datos (RX)
    if (flags & kUART_RxDataRegFullFlag) {
        UART_ClearStatusFlags(UART1, kUART_RxDataRegFullFlag);
        uint8_t data = UART_ReadByte(UART1);
        xQueueSendFromISR(uartRxQueue, &data, NULL);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
