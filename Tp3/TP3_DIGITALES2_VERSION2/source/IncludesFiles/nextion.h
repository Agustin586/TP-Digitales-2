#ifndef NEXTION_H_
#define NEXTION_H_

#include "mefNextion.h"
#include <stdint.h>

#define CANT_TOTAL_PASOS	1
#define MUESTRAS			5

extern void nextion_init(void);
extern void nextion_putObj(uint8_t paso, uint8_t muestra, uint16_t colorNew);
extern void nextion_reset(void);
extern estMefNextion_enum nextion_getPage(estMefNextion_enum page_actual);
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

#endif /* NEXTION_H_ */
