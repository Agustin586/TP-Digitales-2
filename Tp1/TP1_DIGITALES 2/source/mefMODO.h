/*
 * mefMODO.h
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#ifndef MEFMODO_H_
#define MEFMODO_H_

/*==================[inclusions]=============================================*/
#include <stdbool.h>

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefModo_init(void);
extern void mefModo(void);
extern void mefModo_task1ms(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/

#endif /* MEFMODO_H_ */
