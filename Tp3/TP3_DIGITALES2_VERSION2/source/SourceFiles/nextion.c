#include "IncludesFiles/nextion.h"
#include "IncludesFiles/Uart1.h"
#include "fsl_uart.h"
#include "IncludesFiles/queueRtos.h"
#include "queue.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define CANT_MAX_PIXEL_X	320
#define CANT_MAX_PIXEL_Y	240
#define MAX_CARACTERES_TRAMA	50
#define EJE_X_TRANSFORMACION_LINEAL	160
#define EJE_Y_TRANSFORMACION_LINEAL	170
#define CONVERSION_PIXEL_RADIO	25/10
#define COLOR_RED	63488
#define COLOR_GREY	33840
#define RADIO_CIRCULO	2

#define PAGE_MAIN	0x01
#define PAGE_RADAR	0x02
#define PAGE_SERVO	0x03

const char *pic = "pic=";
const char *draw_cirs = "cirs ";
const char *waveform = "add";
const char *reset = "rest";

/*
 * @brief Parametros que son leidos desde algún sensor
 * y angulo. Sirven para tener ambas coordenadas
 * cartesianas y polares.
 * */
typedef struct {
	float posX; /*!<Posicion del eje X --> Conversión en coordenadas cartesianas>*/
	float posY; /*!<Posicion del eje Y --> Conversión en coordenadas cartesianas>*/
	int16_t Angle; /*!<Angulo seteado al servo motor>*/
	float Radio; /*!<Radio o distancia medida por el sensor al objeto detectado>*/
} TipoObjeto_pos;

/*
 * @brief Parametros que son utilizados o mandados directamente
 * a la pantalla Nextion.
 * */
typedef struct {
	uint8_t radio; /*!<Radio del circulo que se va a dibujar>*/
	uint16_t Color; /*!<Color con que se muestra el objeto detectado>*/
	uint16_t pixelX; /*!<Valor en pixeles en el eje X, para posicionar el objeto>*/
	uint8_t pixelY; /*!<Idem pero en el eje Y>*/
} TipoObjeto_display;

typedef struct {
	TipoObjeto_pos Posicion;
	TipoObjeto_display Display;
} TipoObjeto_st;

typedef struct {
	char trama[MAX_CARACTERES_TRAMA];
} TipoTramaNextion_st;

static TipoObjeto_st Objeto[CANT_TOTAL_PASOS][MUESTRAS];
static uint8_t muestra_nrm[CANT_TOTAL_PASOS], paso_nrm = 0;
static TipoTramaNextion_st Trama;

/*
 * @brief Envia la trama que se carga.
 *
 * @param char *
 * */
static void nextion_sendTrama(char *str);

static int16_t nextion_getAngleMuestraX(uint8_t muestra);

extern void nextion_init(void) {

	return;
}

extern void nextion_reset(void) {
	sprintf(Trama.trama, "%s\xFF\xFF\xFF", reset);
	nextion_sendTrama(Trama.trama);

	return;
}

extern estMefNextion_enum nextion_getPage(estMefNextion_enum page_actual) {
#define CANT_MAX_BUFFER	5
#define CANT_DE_BYTES_A_RECIBIR	1

	uint8_t rev_buffer[CANT_MAX_BUFFER];
	QueueHandle_t queueRx = queueRtos_getQueueByName("Uart1_Rx");

	/*Hay que pedirle que solo guarde si es 0x66*/
	if (queueRtos_msgWaiting(queueRx) == pdTRUE) {
		queueRtos_receiveFromQueue(queueRx, rev_buffer, CANT_DE_BYTES_A_RECIBIR,
				pdMS_TO_TICKS(200));

		switch (rev_buffer[0]) {
		case PAGE_MAIN:
			return EST_NEXTION_pMAIN;
			break;
		case PAGE_RADAR:
			return EST_NEXTION_pRADAR;
			break;
		case PAGE_SERVO:
			return EST_NEXTION_pSERVO;
			break;
		default:
			return EST_NEXTION_RESET;
			break;
		}
	}

	return page_actual;
}

extern void nextion_putObj(uint8_t paso, uint8_t muestra, uint16_t colorNew) {
	sprintf(Trama.trama, "%s%d,%d,%d,%d\xFF\xFF\xFF", draw_cirs,
			Objeto[paso][muestra].Display.pixelX,
			Objeto[paso][muestra].Display.pixelY, 2, colorNew);

	nextion_sendTrama(Trama.trama);

	return;
}

extern void nextion_setDataObj(int16_t angle, float radio) {
	/* Parametros propios del objeto */
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.Angle = angle;
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.Radio = radio;
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.posX = radio
			* cos(angle * 3.14 / 180);
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.posY = radio
			* sin(angle * 3.14 / 180);

	/* Parametros a enviar en el display */
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.Color = COLOR_RED;
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.radio = RADIO_CIRCULO;
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelX =
	EJE_X_TRANSFORMACION_LINEAL
			- (Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.posX
					* CONVERSION_PIXEL_RADIO);
	Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelY =
	EJE_Y_TRANSFORMACION_LINEAL
			- (Objeto[paso_nrm][muestra_nrm[paso_nrm]].Posicion.posY
					* CONVERSION_PIXEL_RADIO);

	if (Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelX > 320)
		Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelX = 0;
	if (Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelY > 240)
		Objeto[paso_nrm][muestra_nrm[paso_nrm]].Display.pixelY = 0;

	if (muestra_nrm[paso_nrm] < MUESTRAS)
		muestra_nrm[paso_nrm]++;

	if (muestra_nrm[paso_nrm] == MUESTRAS)
		muestra_nrm[paso_nrm] = 0;

	return;
}

static int16_t nextion_getAngleMuestraX(uint8_t muestra) {
	return Objeto[paso_nrm][muestra].Posicion.Angle;
}

extern uint16_t nextion_getColorAngle(uint8_t muestra, int16_t angle_actual) {
	if (nextion_getAngleMuestraX(muestra) <= (-angle_actual + 15)
			&& nextion_getAngleMuestraX(muestra) >= (-angle_actual - 15))
		return COLOR_RED;
	return COLOR_GREY;
}

extern void nextion_AumentaCantPasos(void) {
	paso_nrm++;

	if (paso_nrm > CANT_TOTAL_PASOS)
		paso_nrm = 0;

	return;
}

extern void nextion_DisminuirCantPasos(void) {
	if (paso_nrm > 0)
		paso_nrm--;

	return;
}

extern uint8_t nextion_getMuestrasTomadas(uint8_t paso) {
	return muestra_nrm[paso];
}

extern uint8_t nextion_getPasosTomados(void) {
	return paso_nrm;
}

extern void nextion_clrPaso(void) {
	paso_nrm = 0;
	muestra_nrm[paso_nrm] = 0;

	return;
}

extern void nextion_clrDatos(void) {
	for (uint8_t i = 0; i < MUESTRAS; i++) {
		nextion_setDataObj(0, 0);
	}

	return;
}

extern void nextion_putPicture(uint8_t picId, uint8_t newPic) {
	sprintf(Trama.trama, "p%d.pic=%d\xFF\xFF\xFF", picId, newPic);
	nextion_sendTrama(Trama.trama);

	return;
}

extern void nextion_sendPwmValue(uint8_t IdWaveform, uint8_t channel,
		uint8_t val) {
	sprintf(Trama.trama, "%s %d,%d,%d\xFF\xFF\xFF", waveform, IdWaveform,
			channel, val);

	nextion_sendTrama(Trama.trama);

	return;
}

static void nextion_sendTrama(char *trama) {
	Uart1_send(trama);

	return;
}
