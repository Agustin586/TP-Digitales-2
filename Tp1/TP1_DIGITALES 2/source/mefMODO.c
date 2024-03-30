/*
 * mefMODO.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

/*==================[inclusions]=============================================*/
#include "mefMODO.h"
#include "mefRUTA.h"
#include "mefACUM.h"
#include "mefCRUCE.h"
#include "mefSEC.h"

/*==================[macros and typedef]=====================================*/
typedef enum{
	EST_MODO_RUTA = 0,
	EST_MODO_CRUCE,
	EST_MODO_SEC,
	EST_MODO_ACUM,
}estMefModo_enum;

typedef enum{
	SAL_RUTA_CRUCE = 0,
	SAL_RUTA_SEC,
	SAL_RUTA_ACUM,
	SAL_RUTA_IDLE,
}salMefRuta_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static estMefModo_enum estMefModo;
static salMefRuta_enum SalidaMefRuta;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mefModo_init(void){
	estMefModo = EST_MODO_RUTA;
	SalidaMefRuta = SAL_RUTA_IDLE;

	mefRuta_init();
	mefSec_init();
	mefCruce_init();
	mefAcum_init();

	return;
}

extern void mefModo(void){
	uint8_t TipoSalidaMefRuta;

	switch (estMefModo){
		case EST_MODO_RUTA:
			TipoSalidaMefRuta = mefRuta();

			if (TipoSalidaMefRuta == SAL_RUTA_CRUCE)
				estMefModo = EST_MODO_CRUCE, mefCruce_reset ();
			if (TipoSalidaMefRuta == SAL_RUTA_SEC)
				estMefModo = EST_MODO_SEC, mefSec_reset ();
			if (TipoSalidaMefRuta == SAL_RUTA_ACUM)
				estMefModo = EST_MODO_ACUM, mefAcum_reset ();

			break;
		case EST_MODO_SEC:
			if (mefSec ())
				estMefModo = EST_MODO_RUTA, mefRuta_reset ();
			break;
		case EST_MODO_CRUCE:
			if (mefCruce ())
				estMefModo = EST_MODO_RUTA;
			break;
		case EST_MODO_ACUM:
			if (mefAcum ())
				estMefModo = EST_MODO_RUTA, mefRuta_reset ();
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

