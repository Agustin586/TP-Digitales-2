/*
 * DETAUTOS.h
 *
 *  Created on: 3 abr. 2024
 *      Author: aguat
 */

#ifndef DETAUTOS_H_
#define DETAUTOS_H_

#include <stdint.h>
#include "key.h"
#include "SD2_board.h"

extern void detautos_reset(void);
extern void detautos_inc();
extern uint8_t detautos_getTotal();
extern void detautos_dec();

#endif /* DETAUTOS_H_ */
