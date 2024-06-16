#ifndef NEXTION_H_
#define NEXTION_H_

#include "mefNextion.h"
#include <stdint.h>

#define CANT_TOTAL_PASOS	1
#define MUESTRAS			15

/*
 * @brief Inicializa la pantalla nextion.
 * */
extern void nextion_init(void);

/*
 * @brief 	Pone un objeto detectado sobre la pantalla de la nextion.
 *
 * @param 	uint8_t paso: sin uso dado la velocidad
 * 			uint8_t muestra: numero de la muestra tomada
 * 			uint16_t colorNew: color que debe mostrarse sobre el objeto que se dibuja
 * */
extern void nextion_putObj(uint8_t paso, uint8_t muestra, uint16_t colorNew);

/*
 * @brief 	Reinicia la pantalla con el comando adecuado.
 * */
extern void nextion_reset(void);

/*
 * @brief 	Setea la pagina actual en la que se encuentra la pantalla
 *
 * @param	uint8_t pageID_: numero de la pagina detectada
 * */
extern void nextion_setPageID(uint8_t pageID_);

/*
 * @brief 	Devuelve la pagina actual en la que se encuentra la pantalla.
 *
 * @return	estMefNextion_enum
 * */
extern estMefNextion_enum nextion_getPage(void);

/*
 * @brief 	Pone una imagen sobre la pantalla.
 *
 * @param	uint8_t picId: id de la picture sobre la que se carga la imagen
 * 			uint8_t newPic: numero de la imagen a cargar
 * */
extern void nextion_putPicture(uint8_t picId, uint8_t newPic);
/*
 * @brief Setea los parametros del objeto detectado.
 *
 * @param int8_t angle	Angulo en que se encuentra el sensor o servo
 * @param float radio	Radio o distancia detectada por el sensor
 * @param uint8_t id	Identificador del nro de objeto
 * */
extern void nextion_setDataObj(int16_t angle, float radio);
extern void nextion_AumentaCantPasos(void);
extern uint8_t nextion_getMuestrasTomadas(uint8_t paso);
extern void nextion_clrPaso(void);
extern void nextion_DisminuirCantPasos(void);
extern uint8_t nextion_getPasosTomados(void);
extern void nextion_clrDatos(void);
extern uint16_t nextion_getColorAngle(uint8_t muestra, int16_t angle_actual);
extern void nextion_sendPwmValue(uint8_t IdWaveform, uint8_t channel, uint8_t val);
extern void nextion_setDistanciaProm(void);
extern void nextion_setAngle(int16_t angle);
extern void nextion_refs(void);
extern void nextion_sendFreqPwm(uint8_t idNumber,uint8_t val);
extern void nextion_setMsPwm(float val);

#endif /* NEXTION_H_ */
