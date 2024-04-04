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

/*==================[macros and typedef]=====================================*/
typedef enum{
	EST_RUTA_HPR = 0,
	EST_RUTA_CR,
}estMefRuta_enum;

typedef enum{
	SAL_RUTA_CRUCE = 0,
	SAL_RUTA_SEC,
	SAL_RUTA_ACUM,
	SAL_RUTA_IDLE,
}salMefRuta_enum;

/*==================[internal functions declaration]=========================*/

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

	return;
}

extern uint8_t mefRuta(void){
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
				return SalMefRuta = SAL_RUTA_CRUCE;

			/* Permite la transición */
			if (detautos_getTotal() >= 3)
				return SalMefRuta = SAL_RUTA_ACUM;

			break;
		case EST_RUTA_CR:
			if (!DELAY_200ms_RUTA)
				DELAY_200ms_RUTA = DELAY_200ms, LVR(TOGGLE);

			LRS(ON);
			LRR(OFF);
			LVS(OFF);

			if (!DELAY_5Seg_RUTA)
				return SalMefRuta = SAL_RUTA_SEC;
			break;
	}

	return SalMefRuta = SAL_RUTA_IDLE;
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
