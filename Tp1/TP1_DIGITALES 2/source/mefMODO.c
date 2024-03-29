/*
 * mefMODO.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#include "mefMODO.h"

typedef enum{
	EST_MODO_RUTA = 0,
	EST_MODO_CURCE,
	EST_MODO_ACUM,
}estMefModo_enum;

static estMefModo_enum estMefModo;

extern void mefModo_init(void){
	estMefModo = EST_MODO_RUTA;

	mefRUTA_init();
	mefCRUCE_init();
	mefACUM_init();

	return;
}

