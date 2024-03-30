
/*==================[inclusions]=============================================*/
#include "mefACUM.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/

typedef enum {
    EST_ACUM_RESET = 0,
    EST_ACUM_HPS,
    EST_ACUM_CS
}estMefACUM_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMefACUM_enum estMefACUM;
static int32_t tim5sACUM;
static int32_t tim200msACUM;
static int8_t contAutosACUM = 2;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefACUM_init(void)
{
    mefACUM_reset();
}

extern void mefACUM_reset(void)
{
    estMefACUM = EST_ACUM_RESET;
    
    tim5sACUM = 300000;
    tim200msACUM = 200;
}


extern bool mefACUM(void)
{
    bool ret = 0;

    switch (estMefSW1)
    {
        case EST_ACUM_RESET:
            
            if (tim200msACUM == 0)
            {
                tim200msACUM = 200;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_ON);
                board_setLed(LRR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_OFF);
            }

            if( tim5sACUM == 0 )
            {
                tim5sACUM = 300000;
                estMefACUM = EST_ACUM_HPS;
            }

        break;

        case EST_ACUM_HPS:
            
            if( contAutosACUM != 0 )
            {
                if( key_getPressEv(SENSOR) ) contAutosACUM--;
                
                board_setLed(LRR, BOARD_LED_MSG_ON);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_ON);
            }

            else
            {
                board_setLed(LRR, BOARD_LED_MSG_OFF);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_OFF);

                estMefACUM = EST_ACUM_CS;
            }

        break;

        case EST_ACUM_CP:
            
            if (tim200msACUM == 0)
            {
                tim200msACUM = 200;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LRR, BOARD_LED_MSG_ON);
            }

            if (tim5sACUM == 0)
            {
                estMefACUM = EST_ACUM_RESET;
                ret = 1;
            }

        break;

    }

    return ret;
}

extern void mefACUM_task1ms(void)
{
    if (tim1msACUM)
        tim1msACUM--;
}


/*==================[end of file]============================================*/

