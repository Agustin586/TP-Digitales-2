#include "IncludesFiles/Uart1.h"
#include "MKL46Z4.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "IncludesFiles/SD2_board.h"
#include "board.h"
#include "IncludesFiles/semaphore.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "fsl_uart_freertos.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_uart_dma.h"
#include <string.h>

#define UART UART1
#define UART_CLKSRC BUS_CLK
#define UART_CLK_FREQ CLOCK_GetFreq(BUS_CLK)
#define UART_RX_TX_IRQn UART1_RX_TX_IRQn

/* Variables del entorno de uart_freertos */
const char *send_ring_overrun = "\r\nRing buffer overrun!\r\n";
const char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";

uint8_t background_buffer[32];
uint8_t recv_buffer[4];
uart_rtos_handle_t handle;
struct _uart_handle t_handle;
uart_dma_handle_t uart_dma_handle;
dma_handle_t uart_tx_dma_handle;

uart_rtos_config_t uart_config = {
	.base = UART1,
    .baudrate = 115200,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

void init_pins(void) {
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3); // UART1_TX
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3); // UART1_RX
}

extern void Uart1_init(void) {
#define CHANNEL_DMA	1
    init_pins();

    uart_config.srcclk = UART_CLK_FREQ;
    uart_config.base = UART;

    if (UART_RTOS_Init(&handle, &t_handle, &uart_config) != kStatus_Success) {
    	PRINTF("Error al configurar UART1");
    }

    DMAMUX_Init(DMAMUX0);
	DMAMUX_SetSource(DMAMUX0, CHANNEL_DMA, kDmaRequestMux0UART1Tx);
	DMAMUX_EnableChannel(DMAMUX0, CHANNEL_DMA);
	DMA_Init(DMA0);

	DMA_CreateHandle(&uart_tx_dma_handle, DMA0, CHANNEL_DMA);
	UART_TransferCreateHandleDMA(UART1, &uart_dma_handle, NULL, NULL, &uart_tx_dma_handle, NULL);

    return;
}

extern int Uart1_send(char *to_send) {
	uart_transfer_t sendXfer;
	sendXfer.data = (uint8_t *)to_send;
	sendXfer.dataSize = strlen(to_send);

	return UART_TransferSendDMA(UART1, &uart_dma_handle, &sendXfer);
}

extern int Uart1_read(uint8_t *receive, uint8_t length, size_t *n) {
    int error;

    error = UART_RTOS_Receive(&handle, receive, length, n);

    if (error == kStatus_UART_RxHardwareOverrun) {
    	PRINTF("%s",send_hardware_overrun);
    }
    if (error == kStatus_UART_RxRingBufferOverrun) {
    	PRINTF("%s",send_ring_overrun);
    }
    return error;
}
