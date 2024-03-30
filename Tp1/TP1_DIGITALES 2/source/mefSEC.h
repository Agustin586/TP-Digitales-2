/*
 * mefSEC.h
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#ifndef MEFSEC_H_
#define MEFSEC_H_

/*==================[inclusions]=============================================*/
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefSec_init(void);
extern void mefSec_reset(void);
extern bool mefSec(void);
extern void mefSec_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* MEFSEC_H_ */
