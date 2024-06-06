#ifndef MACROS_H_
#define MACROS_H_

#include "FreeRTOS.h"
#include "SD2_board.h"
#include "key.h"

/* Bloque de macros para los delays */
/* DELAYS TO TICKS */
#define DELAY_5ms	pdMS_TO_TICKS(5)
#define DELAY_10ms	pdMS_TO_TICKS(10)
#define DELAY_50ms	pdMS_TO_TICKS(50)
#define DELAY_100ms	pdMS_TO_TICKS(100)
#define DELAY_250ms	pdMS_TO_TICKS(250)
#define DELAY_500ms	pdMS_TO_TICKS(500)
#define DELAY_2s	pdMS_TO_TICKS(2000)
#define DELAY_3s	pdMS_TO_TICKS(3000)
/*-----------------*/

/* Macros para los leds */

#define ON		BOARD_LED_MSG_ON
#define OFF 	BOARD_LED_MSG_OFF
#define TOGGLE	BOARD_LED_MSG_TOGGLE

#define LRojo_id		BOARD_LED_ID_ROJO
#define LED_ROJO(x)		board_setLed(LRojo_id, x)

#define LAzul_id		BOARD_LED_ID_VERDE
#define LED_AZUL(x)		board_setLed(LAzul_id, x)

/* Macros para los switch */

#define SW1				BOARD_SW_ID_1
#define SW3				BOARD_SW_ID_3
#define SW_Pulsado(x)	key_getPressEv(x)

#endif /* MACROS_H_ */
