#ifndef UART1_H_
#define UART1_H_

#include <stdint.h>
#include <stdbool.h>

extern void Uart1_init(void);
extern void Uart1_send(char* to_send);
extern int Uart1_read(uint8_t* receive, uint8_t length);

#endif /* UART1_H_ */
