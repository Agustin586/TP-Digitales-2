/*
 * mefRUTA.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

/*==================[inclusions]=============================================*/
#include "mefRUTA.h"
#include "SD2_board.h"
#include "key.h"
#include "DELAYS.h"
#include "LXX.h"
#include "DETAUTOS.h"
#include "DETPULSADOR.h"
#include "TRANSICIONES.h"

/*==================[macros and typedef]=====================================*/
typedef enum{
	EST_RUTA_HPR = 0,
	EST_RUTA_CR,
}estMefRuta_enum;

/*==================[internal functions declaration]=========================*/
#define CANT_MAX_AUTOS_ACUM	3

/*==================[internal data definition]===============================*/
static estMefRuta_enum estMefRuta;
static uint32_t DELAY_2Min_RUTA,DELAY_5Seg_RUTA,DELAY_200ms_RUTA;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mefRuta_init(void){
	mefRuta_reset ();

	return;
}

extern void mefRuta_reset(void){
	estMefRuta = EST_RUTA_HPR;
	DELAY_2Min_RUTA = DELAY_2Min;

	detpulsador_reset();
	detautos_reset();

	return;
}

extern void mefRuta(void){
	static salMefRuta_enum SalMefRuta;

	/* Detecta si debe incrementar */
	detautos_inc();

	switch (estMefRuta){
		case EST_RUTA_HPR:
			LVR(ON);
			LRS(ON);
			LRR(OFF);
			LVS(OFF);

			if (!DELAY_2Min_RUTA){
				DELAY_5Seg_RUTA = DELAY_5Seg, DELAY_200ms_RUTA = DELAY_200ms;

				estMefRuta = EST_RUTA_CR;
			}

			if (detpulsador_getPress())
				trans_setSalida(SalMefRuta = SAL_RUTA_CRUCE);

			/* Permite la transición */
			if (detautos_getTotal() >= CANT_MAX_AUTOS_ACUM)
				trans_setSalida(SalMefRuta = SAL_RUTA_ACUM);

			break;
		case EST_RUTA_CR:
			if (!DELAY_200ms_RUTA)
				DELAY_200ms_RUTA = DELAY_200ms, LVR(TOGGLE);

			LRS(ON);
			LRR(OFF);
			LVS(OFF);

			if (!DELAY_5Seg_RUTA)
				trans_setSalida(SalMefRuta = SAL_RUTA_SEC);
			break;
	}

	return;
}

extern void mefRuta_task1ms(void){
	if (estMefRuta == EST_RUTA_HPR){
		if (DELAY_2Min_RUTA)
			DELAY_2Min_RUTA--;
	}

	if (estMefRuta == EST_RUTA_CR){
		if (DELAY_5Seg_RUTA)
			DELAY_5Seg_RUTA--;
		if (DELAY_200ms_RUTA)
			DELAY_200ms_RUTA--;
	}

	return;
}
