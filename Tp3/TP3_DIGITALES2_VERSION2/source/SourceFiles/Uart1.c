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

uart_rtos_config_t uart_config = {
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
    init_pins();

    uart_config.srcclk = UART_CLK_FREQ;
    uart_config.base = UART;

    if (UART_RTOS_Init(&handle, &t_handle, &uart_config) != kStatus_Success) {
        // Handle error
        while (1);
    }
}

extern void Uart1_send(char *to_send) {
    UART_RTOS_Send(&handle, (uint8_t*) to_send, strlen(to_send));
}

extern int Uart1_read(uint8_t *receive, uint8_t length) {
    size_t n;
    int error;

    error = UART_RTOS_Receive(&handle, receive, length, &n);

    if (error == kStatus_UART_RxHardwareOverrun) {
        if (kStatus_Success != UART_RTOS_Send(&handle, (uint8_t*) send_hardware_overrun, strlen(send_hardware_overrun))) {
            vTaskSuspend(NULL);
        }
    }
    if (error == kStatus_UART_RxRingBufferOverrun) {
        if (kStatus_Success != UART_RTOS_Send(&handle, (uint8_t*) send_ring_overrun, strlen(send_ring_overrun))) {
            vTaskSuspend(NULL);
        }
    }
    return error;
}
