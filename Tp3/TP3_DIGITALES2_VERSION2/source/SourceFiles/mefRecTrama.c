/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, 2018, Gustavo Muro
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

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "IncludesFiles/mefRecTrama.h"
#include "IncludesFiles/procTrama.h"
#include "IncludesFiles/uart0_dma.h"

/*==================[macros and definitions]=================================*/
typedef enum {
	MEF_REC_ESPERANDO_INICIO = 0, MEF_REC_RECIBIENDO, MEF_REC_PROCESANDO,
} est_mefRecTrama_enum;

#define BUFFER_SIZE		8
#define CHAR_CR			0xd
#define CHAR_LF			0xa

/*==================[internal data declaration]==============================*/
static char bufferRec[BUFFER_SIZE];

static est_mefRecTrama_enum mefRecTrama_est;
static uint8_t indexRec;

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void mefRecTrama_init(void) {
	mefRecTrama_est = MEF_REC_ESPERANDO_INICIO;

	return;
}

void mefRecTrama_task(void) {
	uint32_t flagRec;
	uint8_t byteRec;

	uint8_t enter[2];

	enter[0] = '\n';
	enter[1] = '\r';

	flagRec = uart0_recDatos(&byteRec, 1);

	/*<Entrada en consolar>*/
	if (flagRec) {
		if (byteRec != CHAR_LF && byteRec != CHAR_CR) {
			uart0_envDatos(&byteRec, 1);
		}

		else {
			uart0_envDatos(enter, 2);
		}
	}

	switch (mefRecTrama_est) {
	case MEF_REC_ESPERANDO_INICIO:
		if (flagRec != 0 && byteRec == ':') {
			indexRec = 0;
			mefRecTrama_est = MEF_REC_RECIBIENDO;
		}

		break;

	case MEF_REC_RECIBIENDO:
		if (flagRec != 0 && byteRec != CHAR_CR) {
			if (indexRec < BUFFER_SIZE)
				bufferRec[indexRec] = byteRec;
			indexRec++;
		}

		if (flagRec != 0 && byteRec == ':')
			indexRec = 0;

		if (flagRec != 0 && byteRec == CHAR_LF) {
			mefRecTrama_est = MEF_REC_PROCESANDO;
		}

		if (indexRec > BUFFER_SIZE || (flagRec != 0 && byteRec == CHAR_CR)) {
			mefRecTrama_est = MEF_REC_ESPERANDO_INICIO;
		}

		break;

	case MEF_REC_PROCESANDO:
		if (flagRec != 0 && byteRec == ':') {
			indexRec = 0;
			mefRecTrama_est = MEF_REC_RECIBIENDO;
		}

		else {
			procTrama(bufferRec, indexRec);
			mefRecTrama_est = MEF_REC_ESPERANDO_INICIO;
		}

		break;
	}
}

/*==================[end of file]============================================*/

