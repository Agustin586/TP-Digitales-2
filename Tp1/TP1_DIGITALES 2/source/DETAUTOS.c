/*
 * DETAUTOS.c
 *
 *  Created on: 3 abr. 2024
 *      Author: aguat
 */

#include "DETAUTOS.h"
#include "key.h"

static uint8_t ContAutos = 0;

extern void detautos_reset(void){
	ContAutos = 0;

	key_getPressEv(BOARD_SW_ID_SENSOR);

	return;
}

extern void detautos_inc(){
	if (key_getPressEv(BOARD_SW_ID_SENSOR))
		ContAutos++;

	return;
}

extern uint8_t detautos_getTotal(){
	return ContAutos;
}

extern void detautos_dec(){
	if (key_getPressEv(BOARD_SW_ID_SENSOR))
		ContAutos--;

	return;
}
