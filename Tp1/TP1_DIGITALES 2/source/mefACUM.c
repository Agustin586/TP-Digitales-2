
/*==================[inclusions]=============================================*/
#include <stdint.h>
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
static int32_t DELAY_5Seg_ACUM;
static int32_t DELAY_200ms_ACUM;
static int8_t contAutosACUM;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
#define DELAY_5Seg	5000
#define DELAY_200ms	200
#define LVR	BOARD_LED_ID_LVR
#define LRR	BOARD_LED_ID_LRR
#define LVS	BOARD_LED_ID_LVS
#define LRS BOARD_LED_ID_LRS

/*==================[external functions definition]==========================*/
extern void mefAcum_init(void){
    mefAcum_reset();

    return;
}

extern void mefAcum_reset(void){
    estMefACUM = EST_ACUM_RESET;
    
    DELAY_5Seg_ACUM = DELAY_5Seg;
    DELAY_200ms_ACUM = DELAY_200ms;

    return;
}


extern bool mefAcum(void){

    switch (estMefACUM){
        case EST_ACUM_RESET:
            if (DELAY_200ms_ACUM == 0){
                DELAY_200ms_ACUM = 200;
                board_setLed(LVR, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_ON);
                board_setLed(LRR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_OFF);
            }

            if( DELAY_5Seg_ACUM == 0 ){
                DELAY_5Seg_ACUM = 300000;
                estMefACUM = EST_ACUM_HPS;
                contAutosACUM = 2;
            }

            break;

        case EST_ACUM_HPS:
            if( contAutosACUM != 0 ){
                if(key_getPressEv(BOARD_SW_ID_SENSOR))
                	contAutosACUM--;
                
                board_setLed(LRR, BOARD_LED_MSG_ON);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_ON);
            }

            else{
                board_setLed(LRR, BOARD_LED_MSG_OFF);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LVS, BOARD_LED_MSG_OFF);

                estMefACUM = EST_ACUM_CS;

                DELAY_5Seg_ACUM = DELAY_5Seg;  DELAY_200ms_ACUM = DELAY_200ms;
            }
            break;

        case EST_ACUM_CS:
            if (!DELAY_200ms_ACUM){
                DELAY_200ms_ACUM = 200;
                board_setLed(LVS, BOARD_LED_MSG_TOGGLE);
                board_setLed(LRS, BOARD_LED_MSG_OFF);
                board_setLed(LVR, BOARD_LED_MSG_OFF);
                board_setLed(LRR, BOARD_LED_MSG_ON);
            }

            if (!DELAY_5Seg_ACUM){
                estMefACUM = EST_ACUM_RESET;
                return 1;
            }
            break;

    }

    return 0;
}

extern void mefAcum_task1ms(void){
    if (estMefACUM == EST_ACUM_RESET){
    	if (DELAY_200ms_ACUM)
    		DELAY_200ms_ACUM--;
    	if (DELAY_5Seg_ACUM)
    		DELAY_5Seg_ACUM--;
    }

    if (estMefACUM == EST_ACUM_CS){
    	if (DELAY_200ms_ACUM)
			DELAY_200ms_ACUM--;
		if (DELAY_5Seg_ACUM)
			DELAY_5Seg_ACUM--;
    }

    return;
}


/*==================[end of file]============================================*/

