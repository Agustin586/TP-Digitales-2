/*
 * mefCruce.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */


/*==================[inclusions]=============================================*/
#include "mefCruce.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
    EST_CRUCE_RESET = 0,
    EST_CRUCE_HCP,
    EST_CRUCE_CP,
}estMefCruce_enum;

#define DELAY_200ms	200
#define DELAY_10Seg	10000
#define DELAY_1Min	5000

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
static estMefCruce_enum estMefCruce;
static int32_t DELAY_200ms_CRUCE;
static int32_t DELAY_10Seg_CRUCE;
static int32_t DELAY_1Min_CRUCE;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void mefCruce_init(void){
    mefCruce_reset();

    return;
}

extern void mefCruce_reset(void){
    estMefCruce = EST_CRUCE_RESET;
    
    DELAY_200ms_CRUCE = DELAY_200ms;
    DELAY_10Seg_CRUCE = DELAY_10Seg;
    DELAY_1Min_CRUCE = DELAY_1Min;

    return;
}


extern bool mefCruce(void){
    switch (estMefCruce){
        case EST_CRUCE_RESET:
            if (!DELAY_200ms_CRUCE){
                DELAY_200ms_CRUCE = DELAY_200ms;

                LVR(TOGGLE);
            }

            LRS(ON);
            LRR(OFF);
            LVS(OFF);

            if (!DELAY_10Seg_CRUCE){
                estMefCruce = EST_CRUCE_HCP;
                DELAY_1Min_CRUCE = DELAY_1Min;
            }

        break;

        case EST_CRUCE_HCP:
        	LVR(OFF);
        	LRS(OFF);
        	LRR(ON);
        	LVS(ON);

            if (!DELAY_1Min_CRUCE){
                estMefCruce = EST_CRUCE_CP;

                DELAY_10Seg_CRUCE = DELAY_10Seg;
                DELAY_200ms_CRUCE = DELAY_200ms;
            }

        break;

        case EST_CRUCE_CP:
            if (!DELAY_200ms_CRUCE){
                DELAY_200ms_CRUCE = DELAY_200ms;
                LRR(TOGGLE);
            }

            LRS(OFF);
            LVR(OFF);
            LVS(ON);

            if (!DELAY_10Seg_CRUCE){
                estMefCruce = EST_CRUCE_RESET;
                return 1;
            }

        break;

    }

    return 0;
}

extern void mefCruce_task1ms(void){
	if (estMefCruce == EST_CRUCE_RESET){
		if (DELAY_200ms_CRUCE)
			DELAY_200ms_CRUCE--;
		if (DELAY_10Seg_CRUCE)
			DELAY_10Seg_CRUCE--;
	}

	if (estMefCruce == EST_CRUCE_HCP){
		if (DELAY_1Min_CRUCE)
			DELAY_1Min_CRUCE--;
	}

	if (estMefCruce == EST_CRUCE_CP){
		if (DELAY_200ms_CRUCE)
			DELAY_200ms_CRUCE--;
		if (DELAY_10Seg_CRUCE)
			DELAY_10Seg_CRUCE--;
	}

	return;
}


/*==================[end of file]============================================*/

