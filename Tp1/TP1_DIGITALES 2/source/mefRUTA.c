/*
 * mefRUTA.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#include "mefRUTA.h"
#include <stdint.h>

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

static estMefRuta_enum estMefRuta;
static uint16_t DELAY_2Min_RUTA,DELAY_5Seg_RUTA,DELAY_200ms_RUTA;
static uint8_t ContAutos_Secundario;
static bool PULS;

extern void mefRuta_init(void){
	mefRuta_reset ();

	return;
}

extern void mefRuta_reset(void){
	estMefRuta = EST_RUTA_HPR;
	DELAY_2Min_RUTA = 12000;
	ContAutos_Secundario = 0;
	PULS = 0;

	return;
}

extern uint8_t mefRuta(void){
	static salMefRuta_enum SalMefRuta;

	if (key_getPressEv(BOARD_SW_ID_PULS) && !PULS){
		PULS=1;
		return SalMefRuta = SAL_RUTA_CRUCE;
	}

	if (key_getPressEv(BOARD_SW_ID_SENSOR))
		ContAutos_Secundario++;

	if (ContAutos_Secundario == 3)
		return SalMefRuta = SAL_RUTA_ACUM;

	switch (estMefRuta){
		case EST_RUTA_HPR:
			board_setLed(BOARD_LED_ID_LVR, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_LRS, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_LRR, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_LVS, BOARD_LED_MSG_OFF);

			if (!DELAY_2Min_RUTA)
				DELAY_5Seg_RUTA = 5000, DELAY_200ms_RUTA = 200;
			break;
		case EST_RUTA_CR:
			if (!DELAY_200ms_RUTA)
				DELAY_200ms_RUTA = 200, board_setLed(BOARD_LED_ID_LVR, BOARD_LED_MSG_TOGGLE);
			board_setLed(BOARD_LED_ID_LRS, BOARD_LED_MSG_ON);
			board_setLed(BOARD_LED_ID_LRR, BOARD_LED_MSG_OFF);
			board_setLed(BOARD_LED_ID_LVS, BOARD_LED_MSG_OFF);

			if (!DELAY_5Seg_RUTA)
				return SalMefRuta = SAL_RUTA_SEC;
			break;
	}

	return SalMefRuta = SAL_RUTA_IDLE;
}

extern void mefRuta_task1ms(void){
	if (estMefRuta == EST_RUTA_HPR && DELAY_2Min_RUTA)
		DELAY_2Min_RUTA--;
	if (estMefRuta == EST_RUTA_HPR){
		if (DELAY_5Seg_RUTA)
			DELAY_5Seg_RUTA--;
		if (DELAY_200ms_RUTA)
			DELAY_200ms_RUTA--;
	}

	return;
}
