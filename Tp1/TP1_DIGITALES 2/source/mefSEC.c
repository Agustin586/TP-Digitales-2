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
#include "DELAYS.h"
#include "LXX.h"

/*==================[macros and typedef]=====================================*/
typedef enum{
	EST_SEC_HPS = 0,
	EST_SEC_CS,
}estMefSec_enum;

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
