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

#define DELAY_200ms	200
#define DELAY_5Seg	5000
#define DELAY_2Min	1200000

#define LVR_id	BOARD_LED_ID_LVR
#define LRR_id	BOARD_LED_ID_LRR
#define LVS_id	BOARD_LED_ID_LVS
#define LRS_id 	BOARD_LED_ID_LRS

#define ON		BOARD_LED_MSG_ON
#define OFF		BOARD_LED_MSG_OFF
#define TOGGLE 	BOARD_LED_MSG_TOGGLE

#define LVR(X) 	board_setLed(LVR_id, X)
#define LRR(X)	board_setLed(LRR_id, X)
#define LVS(X)	board_setLed(LVS_id, X)
#define LRS(X)	board_setLed(LRS_id, X)

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static estMefRuta_enum estMefRuta;
static uint16_t DELAY_2Min_RUTA,DELAY_5Seg_RUTA,DELAY_200ms_RUTA;
static uint8_t ContAutos_Secundario;
static bool PULS;

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
			LVR(ON);
			LRS(ON);
			LRR(OFF);
			LVS(OFF);

			if (!DELAY_2Min_RUTA)
				DELAY_5Seg_RUTA = DELAY_5Seg, DELAY_200ms_RUTA = DELAY_200ms;
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
