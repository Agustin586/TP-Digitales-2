/*
 * DETPULSADOR.c
 *
 *  Created on: 3 abr. 2024
 *      Author: aguat
 */

#include "DETPULSADOR.h"
#include "key.h"
#include "SD2_board.h"

static bool PULSADO;

extern void detpulsador_reset(void){
	PULSADO = 0;

	/* Reinicio el event switch */
	key_getPressEv(BOARD_SW_ID_PULS);

	return;
}

extern bool detpulsador_getPress(void){
	if (key_getPressEv(BOARD_SW_ID_PULS) && !PULSADO){
		PULSADO = 1;
		return PULSADO;
	}
	return 0;
}



