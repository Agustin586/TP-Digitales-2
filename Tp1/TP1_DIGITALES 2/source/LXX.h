/*
 * LXX.h
 *
 *  Created on: 3 abr. 2024
 *      Author: aguat
 */

#ifndef LXX_H_
#define LXX_H_

#define LVR_id	BOARD_LED_ID_LVR
#define LRR_id	BOARD_LED_ID_LRR
#define LVS_id	BOARD_LED_ID_LVS
#define LRS_id 	BOARD_LED_ID_LRS

#define ON		BOARD_LED_MSG_ON
#define OFF		BOARD_LED_MSG_OFF
#define TOGGLE 	BOARD_LED_MSG_TOGGLE

#define LVR(X) 	board_setLed(LVR_id, X)
#define LRR(X)	board_setLed(LRR_id, X)
#define LVS(X)	board_setLed(LVS_id, X)
#define LRS(X)	board_setLed(LRS_id, X)

#endif /* LXX_H_ */
