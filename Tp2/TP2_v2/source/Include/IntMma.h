/*
 * IntMma.h
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_INTMMA_H_
#define INCLUDE_INTMMA_H_

/*< Archivos >*/
#include "stdint.h"
#include "stdbool.h"

/*< Definiciones >*/
#define MAX_QUEUE_LONG		70

/*< Variables >*/
typedef struct {
	int16_t ReadX;
	int16_t ReadY;
	int16_t ReadZ;
	uint32_t NormaCuad;
} DatosMMA8451_t;

/*< Funciones >*/
extern void taskRtos_INTFF(void *pvParameters);
extern void taskRtos_INTDRDY(void *pvParameters);
extern bool intMma_getIFFreeFall(void);
extern uint32_t queueRtos_receiveNormaMaxCuad(void);
extern void queueRtos_receiveDatosEjes(DatosMMA8451_t *DatosEjes, uint8_t *longitud);

#endif /* INCLUDE_INTMMA_H_ */
