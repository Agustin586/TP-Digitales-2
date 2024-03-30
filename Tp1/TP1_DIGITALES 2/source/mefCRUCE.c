/*
 * mefCRUCE.c
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */


/*==================[inclusions]=============================================*/
#include "mefCRUCE.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/

typedef enum {
    EST_CRUCE_RESET = 0,
    EST_CRUCE_HCP,
    EST_CRUCE_CP
}estMefCRUCE_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMefCRUCE_enum estMefCRUCE;
static int32_t tim200msCRUCE;
static int32_t tim10sCRUCE;
static int32_t tim1mCRUCE;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefCRUCE_init(void)
{
    mefCRUCE_reset();
}

extern void mefCRUCE_reset(void)
{
    estMefCRUCE = EST_CRUCE_RESET;
    
    tim200msCRUCE = 200;
    tim10sCRUCE = 10000;
    tim1mCRUCE = 60000;

}


extern bool mefCRUCE(void)
{
    bool ret = 0;

    switch (estMefSW1)
    {
        case EST_CRUCE_RESET:
            
            if (tim200msCRUCE == 0)
            {
                tim200msCRUCE = 200;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_ON);
                board_setLed(LRR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_OFF);
            }

            if( tim10sCRUCE == 0 ) estMefCRUCE = EST_CRUCE_HCP;

        break;

        case EST_CRUCE_HCP:
            
            board_setLed(LVR, BOARD_LED_MSG_OFF);
            board_setLed(LRS, BOARD_LED_MSG_OFF);
            board_setLed(LRR, BOARD_LED_MSG_ON);
            board_setLed(LVS, BOARD_LED_MSG_ON);

            if( tim1mCRUCE == 0 ) estMefCRUCE = EST_CRUCE_CP;

        break;

        case EST_CRUCE_CP:
            
            if (tim200msCRUCE == 0)
            {
                tim200msCRUCE = 200;
                board_setLed(LRR, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_ON);
            }

            if (tim10sCRUCE == 0)
            {
                estMefCRUCE = EST_CRUCE_RESET;
                ret = 1;
            }

        break;

    }

    return ret;
}

extern void mefCRUCE_task1ms(void)
{
    if (tim1msCRUCE)
        tim1msCRUCE--;
}


/*==================[end of file]============================================*/

