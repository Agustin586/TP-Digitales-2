/*
 * spiRtos.h
 *
 *  Created on: 1 jul. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_SPIRTOS_H_
#define INCLUDE_SPIRTOS_H_

/*< Archivos >*/
#include <stdint.h>

/*< Funciones >*/
extern void spiRtos_init(void);
extern void spiRtos_xfer(uint8_t *tx_Data, uint8_t *rx_Data);

#endif /* INCLUDE_SPIRTOS_H_ */
