/*
 * IntMma.h
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_INTMMA_H_
#define INCLUDE_INTMMA_H_

#include "stdint.h"
#include "stdbool.h"

extern void taskRtos_INTFF(void *pvParameters);
extern void taskRtos_INTDRDY(void *pvParameters);
extern bool intMma_getIFFreeFall(void);
extern uint32_t queueRtos_receiveNormaMaxCuad(void);

#endif /* INCLUDE_INTMMA_H_ */
