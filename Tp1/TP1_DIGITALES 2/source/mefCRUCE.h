#ifndef MEF_CRUCE_H
#define MEF_CRUCE_H

/*==================[inclusions]=============================================*/
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefCRUCE_reset(void);
extern void mefCRUCE_init(void);
extern bool mefCRUCE(void);
extern void mefCRUCE_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_CRUCE_H */
