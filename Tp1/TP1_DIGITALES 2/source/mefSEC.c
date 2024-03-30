/*
 * mefSEC.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */


/*==================[inclusions]=============================================*/
#include "mefSEC.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/
typedef enum{
	EST_SEC_HPS = 0,
	EST_SEC_CS,
}estMefSec_enum;

#define DELAY_200ms	200
#define DELAY_5Seg	5000
#define DELAY_30Seg	5000

#define LVR_id	BOARD_LED_ID_LVR
#define LRR_id	BOARD_LED_ID_LRR
#define LVS_id	BOARD_LED_ID_LVS
#define LRS_id BOARD_LED_ID_LRS

#define ON		BOARD_LED_MSG_ON
#define OFF		BOARD_LED_MSG_OFF
#define TOGGLE 	BOARD_LED_MSG_TOGGLE

#define LVR(X) 	board_setLed(LVR_id, X)
#define LRR(X)	board_setLed(LRR_id, X)
#define LVS(X)	board_setLed(LVS_id, X)
#define LRS(X)	board_setLed(LRS_id, X)

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static estMefSec_enum estMefSec;
static uint8_t DELAY_200ms_SEC;
static uint16_t DELAY_5Seg_SEC, DELAY_30Seg_SEC;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mefSec_init(void){
	mefSec_reset();

	return;
}

extern void mefSec_reset(void){
	estMefSec = EST_SEC_HPS;
	DELAY_30Seg_SEC = DELAY_30Seg;

	return;
}

extern bool mefSec(void){
	switch(estMefSec){
		case EST_SEC_HPS:
			LRR(ON);
			LVS(ON);
			LVR(OFF);
			LRS(OFF);

			if (!DELAY_30Seg_SEC){
				estMefSec = EST_SEC_CS;

				DELAY_200ms_SEC = DELAY_200ms;
				DELAY_5Seg_SEC = DELAY_5Seg;
			}

			break;
		case EST_SEC_CS:
			if (!DELAY_200ms_SEC)
				DELAY_200ms_SEC = DELAY_200ms, LVS(TOGGLE);

			LRR(ON);
			LRS(OFF);
			LVR(OFF);

			if (!DELAY_5Seg_SEC)
				return 1;
			break;
	}

	return 0;
}

extern void mefSec_task1ms(void){
	if (estMefSec == EST_SEC_HPS){
		if (DELAY_30Seg_SEC)
			DELAY_30Seg_SEC--;
	}

	if (estMefSec == EST_SEC_CS){
		if (DELAY_200ms_SEC)
			DELAY_200ms_SEC--;
		if (DELAY_5Seg_SEC)
			DELAY_5Seg_SEC--;
	}

	return;
}
