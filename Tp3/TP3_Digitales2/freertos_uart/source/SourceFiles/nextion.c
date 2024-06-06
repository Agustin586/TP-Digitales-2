#include "IncludesFiles/nextion.h"
#include "IncludesFiles/Uart1.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define CANT_MAXIMA_OBJETOS	7
#define CANT_MAX_PIXEL_X	320
#define CANT_MAX_PIXEL_Y	240
#define EJE_X_TRANSFORMACION_LINEAL	160
#define EJE_Y_TRANSFORMACION_LINEAL	170
#define CONVERSION_PIXEL_RADIO	25/10
#define COLOR_RED	63488

#define PAGE_MAIN	0x01
#define PAGE_RADAR	0x02
#define PAGE_SERVO	0x03

const char *draw_cirs = "cirs ";
const char *reset = "rest\r\n";

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

static TipoObjeto_st Objeto[CANT_MAXIMA_OBJETOS];

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

/*
 * @brief Setea los parametros del objeto detectado.
 *
 * @param int8_t angle	Angulo en que se encuentra el sensor o servo
 * @param float radio	Radio o distancia detectada por el sensor
 * @param uint8_t id	Identificador del nro de objeto
 * */
static void nextion_setDataObj(int8_t angle, float radio, uint8_t id);

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

	Uart1_read((uint8_t*)rev_buffer);

	switch ((uint8_t)rev_buffer[2]) {
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

extern void nextion_putObj(int8_t angle, float radio, uint8_t id) {
	TipoTramaNextion_st Trama;

	nextion_setDataObj(angle, radio, id);

	strcpy(Trama.comando, draw_cirs);
	sprintf(Trama.parametro, "%d,%d,%d,%d\r\n", Objeto[id].Display.pixelX,
			Objeto[id].Display.pixelY, Objeto[id].Display.radio,
			Objeto[id].Display.Color);
	strcpy(Trama.trama, Trama.comando);
	strcat(Trama.trama, Trama.parametro);

	nextion_sendTrama(Trama.trama);
	nextion_EndTrama();

	return;
}

static void nextion_setDataObj(int8_t angle, float radio, uint8_t id) {
	/* Parametros propios del objeto */
	Objeto[id].Posicion.Angle = angle;
	Objeto[id].Posicion.Radio = radio;
	Objeto[id].Posicion.posX = radio * cos(angle);
	Objeto[id].Posicion.posY = radio * sin(angle);

	/* Parametros a enviar en el display */
	Objeto[id].Display.Color = COLOR_RED;
	Objeto[id].Display.radio = 1;
	Objeto[id].Display.pixelX = EJE_X_TRANSFORMACION_LINEAL
			- (Objeto[id].Posicion.posX * CONVERSION_PIXEL_RADIO);
	Objeto[id].Display.pixelY = EJE_Y_TRANSFORMACION_LINEAL
			- (Objeto[id].Posicion.posY * CONVERSION_PIXEL_RADIO);

	return;
}

static void nextion_sendTrama(char *trama) {
	Uart1_send(trama);

	return;
}

static void nextion_EndTrama(void) {
	Uart1_send(0xFF);
	Uart1_send(0xFF);
	Uart1_send(0xFF);

	return;
}
