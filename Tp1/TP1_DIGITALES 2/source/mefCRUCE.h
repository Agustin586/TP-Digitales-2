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
extern void mefCruce_reset(void);
extern void mefCruce_init(void);
extern bool mefCruce(void);
extern void mefCruce_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_CRUCE_H */
