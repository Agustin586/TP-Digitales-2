#ifndef MEF_ACUM_H
#define MEF_ACUM_H

/*==================[inclusions]=============================================*/
#include <stdbool.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefAcum_reset(void);
extern void mefAcum_init(void);
extern bool mefAcum(void);
extern void mefAcum_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEF_ACUM_H */
