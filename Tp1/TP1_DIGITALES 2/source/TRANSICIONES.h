/*
 * TRANSICIONES.h
 *
 *  Created on: 5 abr. 2024
 *      Author: aguat
 */

#ifndef TRANSICIONES_H_
#define TRANSICIONES_H_

#include <stdint.h>

typedef enum{
	SAL_RUTA_CRUCE = 0,
	SAL_RUTA_SEC,
	SAL_RUTA_ACUM,
	SAL_RUTA_IDLE,
}salMefRuta_enum;

extern void trans_reset(void);
extern void trans_setSalida(salMefRuta_enum TipoSalidaSet);
extern uint8_t trans_getSalida(void);

#endif /* TRANSICIONES_H_ */
