#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include "fsl_uart.h"
#include "FreeRTOS.h"
#include "queue.h"

extern void Uart1_init(void);
extern int Uart1_send(char* to_send);
extern int Uart1_read(uint8_t* receive, uint8_t length, size_t *n);

#endif /* UART1_H_ */
