/* Copyright 2023, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * Copyright 2023, Guido Cicconi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SD2_BOARD_H_
#define SD2_BOARD_H_

/*==================[inclusions]=============================================*/
#include "MKL46Z4.h"
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

typedef enum
{
    BOARD_LED_ID_ROJO = 0,
    BOARD_LED_ID_VERDE,
	BOARD_LED_ID_ROJO_EXT,
	BOARD_LED_ID_AZUL_EXT,
    BOARD_LED_ID_TOTAL
}board_ledId_enum;

typedef enum
{
    BOARD_SW_ID_1 = 0,
    BOARD_SW_ID_3,
    BOARD_SW_ID_TOTAL
}board_swId_enum;

typedef enum
{
    BOARD_LED_MSG_OFF = 0,
    BOARD_LED_MSG_ON,
    BOARD_LED_MSG_TOGGLE
}board_ledMsg_enum;

typedef enum
{
	OLED_RST_PIN = 0,
    OLED_DATA_CMD_PIN,
	OLED_TOTAL
}board_oledPin_enum;

typedef struct
{
    PORT_Type *port;
    GPIO_Type *gpio;
    uint32_t pin;
}board_gpioInfo_type;

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/** \brief inicialización del hardware
 **
 **/
void board_init(void);

/** \brief setea estado del led indicado
 **
 **/
void board_setLed(board_ledId_enum id, board_ledMsg_enum msg);

/** \brief Devuelve estado del pulsador indicado
 **
 ** \return true: si el pulsdor está apretado
 **         false: si el pulsador no está apretado
 **/
bool board_getSw(board_swId_enum id);

/** \brief Inicializa el SPI0
 **
 **/
void board_configSPI0();

/** \Envia un dato por SPI
 **
 **/
void board_SPISend(uint8_t* buf, size_t len);

/** \Setea al valor indicado el pin del OLED indicado
 **
 **/
void board_setOledPin(board_oledPin_enum oledPin, uint8_t state);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* SD2_BOARD_H_ */
