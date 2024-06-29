#ifndef MACROS_H_
#define MACROS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "SD2_board.h"
#include "key.h"

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

/* Delays freertos */
#define delay_ms(x)		vTaskDelay(x)

#endif /* MACROS_H_ */
