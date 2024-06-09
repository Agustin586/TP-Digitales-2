#include "IncludesFiles/nextion.h"
#include "IncludesFiles/Uart1.h"
#include "fsl_uart.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define CANT_MAX_PIXEL_X	320
#define CANT_MAX_PIXEL_Y	240
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
const char *reset = "rest";
const char *color_Red = "RED";

/*
 * @brief Parametros que son leidos desde algún sensor
 * y angulo. Sirven para tener ambas coordenadas
 * cartesianas y polares.
 * */
typedef struct {
	float posX; /*!<Posicion del eje X --> Conversión en coordenadas cartesianas>*/
	float posY; /*!<Posicion del eje Y --> Conversión en coordenadas cartesianas>*/
	int8_t Angle; /*!<Angulo seteado al servo motor>*/
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
	char comando[10];
	char parametro[30];
	char trama[40];
} TipoTramaNextion_st;

static TipoObjeto_st Objeto[CANT_TOTAL_PASOS][MUESTRAS];
static uint8_t muestra_nrm[CANT_TOTAL_PASOS], paso_nrm = 0;

/*
 * @brief Envia la trama que se carga.
 *
 * @param char *
 * */
static void nextion_sendTrama(char *str);

/*
 * @brief Finaliza la trama con 0xFF 0xFF 0xFF
 * */
static void nextion_EndTrama(void);

static int16_t nextion_getAngleMuestraX(uint8_t muestra);

extern void nextion_init(void) {

	return;
}

extern void nextion_reset(void) {
	TipoTramaNextion_st Trama;

	strcpy(Trama.trama, reset);
	nextion_sendTrama(Trama.trama);
	nextion_EndTrama();

	return;
}

extern estMefNextion_enum nextion_getPage(void) {
	char rev_buffer[7];

	Uart1_read((uint8_t*) rev_buffer);

	switch ((uint8_t) rev_buffer[2]) {
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

	return EST_NEXTION_RESET;
}

extern void nextion_putObj(uint8_t paso, uint8_t muestra, uint16_t colorNew) {
	TipoTramaNextion_st Trama;

	strcpy(Trama.comando, draw_cirs);
	sprintf(Trama.parametro, "%d,%d,%d,%d",
			Objeto[paso][muestra].Display.pixelX,
			Objeto[paso][muestra].Display.pixelY,
			Objeto[paso][muestra].Display.radio, colorNew);
	strcpy(Trama.trama, Trama.comando);
	strcat(Trama.trama, Trama.parametro);

	nextion_sendTrama(Trama.trama);
	nextion_EndTrama();

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
	if (nextion_getAngleMuestraX(muestra) <= (angle_actual + 105 + 15)
			&& nextion_getAngleMuestraX(muestra) >= (angle_actual - 15 + 105))
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
	TipoTramaNextion_st Trama;
	char buffer[15];

	sprintf(buffer, "p%d.", picId);
	strcpy(Trama.trama, buffer);
	strcat(Trama.trama, pic);
	buffer[0] = '\0';
	sprintf(buffer, "%d", newPic);
	strcat(Trama.trama, buffer);

	nextion_sendTrama(Trama.trama);
	nextion_EndTrama();

	return;
}

static void nextion_sendTrama(char *trama) {
	Uart1_send(trama);

	return;
}

static void nextion_EndTrama(void) {
	uint8_t end_cmd[] = { 0xFF, 0xFF, 0xFF };
	UART_WriteBlocking(UART1, end_cmd, sizeof(end_cmd));

	return;
}
