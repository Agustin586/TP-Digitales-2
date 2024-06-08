#ifndef NEXTION_H_
#define NEXTION_H_

#include "mefNextion.h"
#include <stdint.h>

extern void nextion_init(void);
extern void nextion_putObj(int16_t angle, float radio, uint8_t id);
extern void nextion_reset(void);
extern estMefNextion_enum nextion_getPage(void);
extern void nextion_putPicture(uint8_t picId, uint8_t newPic);
/*
 * @brief Setea los parametros del objeto detectado.
 *
 * @param int8_t angle	Angulo en que se encuentra el sensor o servo
 * @param float radio	Radio o distancia detectada por el sensor
 * @param uint8_t id	Identificador del nro de objeto
 * */
extern void nextion_setDataObj(int16_t angle, float radio, uint8_t id);

#endif /* NEXTION_H_ */
