/*
 * mefRUTA.h
 *
 *  Created on: 29 mar. 2024
 *      Author: aguat
 */

#ifndef MEFRUTA_H_
#define MEFRUTA_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdbool.h>

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefRuta_init(void);
extern void mefRuta_reset(void);
extern uint8_t mefRuta(void);
extern void mefRuta_task1ms(void);

/*==================[end of file]============================================*/
#endif /* MEFRUTA_H_ */
