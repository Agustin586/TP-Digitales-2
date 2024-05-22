/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
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

#ifndef MMA8451_H_
#define MMA8451_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "energia.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

typedef enum
{
    DR_800hz = 0b000,
    DR_400hz = 0b001,
    DR_200hz = 0b010,
    DR_100hz = 0b011,
    DR_50hz = 0b100,
    DR_12p5hz = 0b101,
    DR_6p25hz = 0b110,
    DR_1p56hz = 0b111,
}DR_enum;

/** \brief puerto I2C utilizado en el acelerómetro  */
#define MMA8451_I2C     I2C0

#define INT1_PORT       PORTC
#define INT1_GPIO       GPIOC
#define INT1_PIN        5

#define INT2_PORT       PORTD
#define INT2_GPIO       GPIOD
#define INT2_PIN        1

#define G				9.8
#define t_s(d)			sqrt(((2*d)/100.0)/G)
#define THS_cm(d)		((d/100.0)*2)/(G*t_s(d)*t_s(d))
#define THS_CM_TO_COUNT	0.063

#define G_THS		0.3
#define THS_MAX_FF 	G_THS*100
#define THS_MAX_FF_CUADRADO	THS_MAX_FF*THS_MAX_FF
#define THS_REF_RANGO_2G_CUADRADO	200*200

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/** \brief configura acelerometro MMA8451
 **	
 **/
void mma8451_init(void);

/** \brief Lee lectura del acelerometro en el eje X
 **
 ** \return Lectura del acelerometro en centecimas de g
 **/
int16_t mma8451_getAcX(void);
int16_t mma8451_getAcY(void);
int16_t mma8451_getAcZ(void);

void mma8451_activar(void);
void mma8451_desactivar(void);

void mma8451_setDataRate(DR_enum rate);

void mma8451_FFinit(void);
void mma8451_DRDYinit(void);

uint32_t mma8451_norma_cuadrado(void);

void mma8451_enableDRDYInt(void);
void mma8451_disableDRDYInt(void);

extern void mma8451_IntFF(void);
extern void mma8451_IntDRYD(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MMA8451_H_ */
