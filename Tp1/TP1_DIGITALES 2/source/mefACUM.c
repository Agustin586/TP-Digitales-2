
/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "mefACUM.h"
#include "SD2_board.h"
#include "key.h"
#include "DELAYS.h"
#include "LXX.h"
#include "DETAUTOS.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
    EST_ACUM_RESET = 0,
    EST_ACUM_HPS,
    EST_ACUM_CS,
}estMefACUM_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static estMefACUM_enum estMefACUM;
static int32_t DELAY_5Seg_ACUM;
static int32_t DELAY_200ms_ACUM;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

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
            if (!DELAY_200ms_ACUM){
                DELAY_200ms_ACUM = DELAY_200ms;
                LVR(TOGGLE);
            }

            LRS(ON);
            LRR(OFF);
            LVS(OFF);

            detautos_inc();

            if(!DELAY_5Seg_ACUM){
                DELAY_5Seg_ACUM = DELAY_5Seg;
                estMefACUM = EST_ACUM_HPS;
            }

            break;

        case EST_ACUM_HPS:
            if(detautos_getTotal()){
				detautos_dec();
                
				LRR(ON);
				LVS(ON);
				LRS(OFF);
				LVR(OFF);
            }

            else{
            	LRR(OFF);
            	LRS(OFF);
            	LVR(OFF);
            	LVS(OFF);

                estMefACUM = EST_ACUM_CS;

                DELAY_5Seg_ACUM = DELAY_5Seg;  DELAY_200ms_ACUM = DELAY_200ms;
            }

            break;

        case EST_ACUM_CS:
            if (!DELAY_200ms_ACUM){
                DELAY_200ms_ACUM = DELAY_200ms,LVS(TOGGLE);

                LRS(OFF);
                LVR(OFF);
                LRR(ON);
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

