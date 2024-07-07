/*
 * nextion.c
 *
 *  Created on: 1 jul. 2024
 *      Author: aguat
 */

/*< Archivos >*/
#include "Include/nextion.h"
#include "Include/Uart1.h"
#include "Include/MACROS.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdio.h>

/*< Definiciones>*/
#define MAX_CARACTERES_TRAMA	50

/*< Variables >*/
const char *waveform = "add";
const char *reset = "rest";
const char *fin_trama = "\xFF\xFF\xFF";

typedef struct {
	char trama[MAX_CARACTERES_TRAMA];
} TipoTramaNextion_st;

static TipoTramaNextion_st Trama;

/*< Funciones >*/
static void nextion_sendTrama(char *str);

/*
 * @brief	Inicializa la pantalla nextion con un reset
 * */
extern void nextion_init(void) {
	sprintf(Trama.trama, "%s%s", reset, fin_trama);
	nextion_sendTrama(Trama.trama);

	return;
}

/*
 * @brief	Envia datos a un ploteador
 *
 * @param	uint8_t waveformId:	id del objeto
 * @param	uint8_t channel:	canal de la forma de onda
 * @param	uint8_t val:		valor a cargar
 * */
extern void nextion_waveform(uint8_t waveformId, uint8_t channel, uint8_t val) {
	sprintf(Trama.trama, "%s %d,%d,%d%s", waveform, waveformId, channel, val,
			fin_trama);
	nextion_sendTrama(Trama.trama);

	return;
}

/*
 * @brief	Envia texto a la pantalla
 *
 * @param	uint8_t textId:	Id del objeto texto
 * @param	char *buffer:	String a cargar
 * */
extern void nextion_text(uint8_t textId, char *buffer) {
	sprintf(Trama.trama, "t%d.txt=\"%s\"%s", textId, buffer, fin_trama);
	nextion_sendTrama(Trama.trama);

	return;
}

/*
 * @brief	Envia la trama por uart a la pantalla
 * */
static void nextion_sendTrama(char *str) {
	if (Uart1_send(str) != kStatus_Success)
		PRINTF("Error: no se pudo enviar\r\n");

	return;
}

extern void taskRtos_UART1(void *pvParameters) {
	PRINTF("Tarea: Uart 1\r\n");

	Uart1_init();

	Uart1_send("Mensaje de prueba\r\n");

	for (;;) {
		/* Utilizado para recepcion */
		delay_ms(100);
	}

	vTaskDelete(NULL);

	return;
}
