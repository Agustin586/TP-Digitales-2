/*
 * TRANSICIONES.c
 *
 *  Created on: 5 abr. 2024
 *      Author: aguat
 */

#include "TRANSICIONES.h"

static salMefRuta_enum TransicionRuta;

extern void trans_reset(void){
	TransicionRuta = SAL_RUTA_IDLE;

	return;
}

extern void trans_setSalida(salMefRuta_enum TipoSalidaSet){
	TransicionRuta = TipoSalidaSet;

	return;
}

extern uint8_t trans_getSalida(void){

	return TransicionRuta;
}

