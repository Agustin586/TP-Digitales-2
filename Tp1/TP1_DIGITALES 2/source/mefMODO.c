/*
 * mefMODO.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#include "mefMODO.h"

typedef enum{
	EST_MODO_RUTA = 0,
	EST_MODO_CRUCE,
	EST_MODO_SEC,
	EST_MODO_ACUM,
	SAL_RUTA_IDLE,
}estMefModo_enum;

typedef enum{
	SAL_RUTA_CRUCE = 0,
	SAL_RUTA_SEC,
	SAL_RUTA_ACUM,
}salMefRuta_enum;

static estMefModo_enum estMefModo;

extern void mefModo_init(void){
	estMefModo = EST_MODO_RUTA;

	mefRuta_init();
	mefSec_init();
	mefCruce_init();
	mefAcum_init();

	return;
}

extern void mefModo(void){
	switch (estMefModo){
		case EST_MODO_RUTA:
			static salMefRuta_enum SalidaMefRuta;

			if (mefRuta() == SAL_RUTA_CRUCE)
				estMefModo = EST_MODO_CRUCE, mefCruce_init ();
			if (mefRuta() == SAL_RUTA_SEC)
				estMefModo = EST_MODO_SEC, mefSec_init ();
			if (mefRuta() == SAL_RUTA_ACUM)
				estMefModo = EST_MODO_ACUM, mefAcum_init ();

			/* Redundante!!! */
			if (mefRuta() == SAL_RUTA_IDLE)
				estMefModo = EST_MODO_RUTA;

			break;
		case EST_MODO_SEC:
			if (mefSec ())
				estMefModo = EST_MODO_RUTA, mefRuta_init ();
			break;
		case EST_MODO_CRUCE:
			if (mefCruce ())
				estMefModo = EST_MODO_RUTA;
			break;
		case EST_MODO_ACUM:
			if (mefAcum ())
				estMefModo = EST_MODO_RUTA, mefRuta_init ();
			break;
	}
	return;
}

extern void mefModo_task1ms(void){
	if (estMefModo == EST_MODO_RUTA)
		mefRuta_task1ms ();
	if (estMefModo == EST_MODO_SEC)
		mefSec_task1ms ();
	if (estMefModo == EST_MODO_CRUCE)
		mefCruce_task1ms ();
	if (estMefModo == EST_MODO_ACUM)
		mefAcum_task1ms ();

	return;
}

