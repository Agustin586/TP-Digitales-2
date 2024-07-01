/*
 * nextion.h
 *
 *  Created on: 1 jul. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_NEXTION_H_
#define INCLUDE_NEXTION_H_

/*< Archivos >*/
#include <stdint.h>

/*< Definiciones >*/
#define TEXT_ID(x)				x
#define WAVEFORM_ID0(x)			x
#define WAVEFORM_CHANNEL(x)		x

/*< Funciones >*/
extern void nextion_init(void);
extern void nextion_waveform(uint8_t waveformId, uint8_t channel, uint8_t val);
extern void nextion_text(uint8_t textId, char *buffer);
extern void taskRtos_UART1(void *pvParameters);

#endif /* INCLUDE_NEXTION_H_ */
