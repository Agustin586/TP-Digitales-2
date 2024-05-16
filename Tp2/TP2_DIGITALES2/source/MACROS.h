#ifndef MACROS_H_
#define MACROS_H_

#include "SD2_board.h"
#include "key.h"

/* Bloque de macros para los delays */

//#define SYSTICK_TIMER	0.001
//#define DELAY_500ms		0.5/SYSTICK_TIMER	// 500 ms
//#define DELAY_10seg		10/SYSTICK_TIMER	// 10 segundos
//#define DELAY_3seg		3/SYSTICK_TIMER		// 3 segundos
//#define DELAY_100ms		0.1/SYSTICK_TIMER
//#define DELAY_50ms		0.05/SYSTICK_TIMER
//#define FRAME_HZ_ticks(x)	(1/x)/SYSTICK_TIMER;

/* Bloque de macro para el muestreo de datos */

#define MUESTREO		0.05/SYSTICK_TIMER	// 50 ms

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
