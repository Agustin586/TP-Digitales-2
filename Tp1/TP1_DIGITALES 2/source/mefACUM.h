#ifndef MEF_ACUM_H
#define MEF_ACUM_H

/*==================[inclusions]=============================================*/
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefACUM_reset(void);
extern void mefACUM_init(void);
extern bool mefACUM(void);
extern void mefACUM_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_ACUM_H */
