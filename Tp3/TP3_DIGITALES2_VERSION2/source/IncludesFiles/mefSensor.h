#ifndef MEFSENSOR_H_
#define MEFSENSOR_H_

#include <stdbool.h>

/*
 * @brief Incializa a la mef del sensor.
 * */
extern void mefSensor_init(void);

/*
 * @brief Mef del sensor.
 * */
extern void mefSensor(void);

/*
 * @brief Devuelve el valor de la ultima distancia.
 *
 * @return float Valor de la ultima distancia medida.
 * */
extern float mefSensor_getDistance(void);

/*
 * @brief Setea el valor de la ultima distancia medida.
 *
 * @param float distancia
 * */
extern void mefSensor_setDistancia(float distancia);

/*
 * @brief Pone el dato recivido en listo para ser leido.
 * */
extern void mefSensor_setDatoListo(void);

/*
 * @brief Obtiene el estado del estado de dato listo.
 * Sirve para detectar si fue procesador por la mef.
 *
 * @return bool
 * */
extern bool mefSensor_getDatoListo(void);

/*
 * @brief Limpia el dato listo.
 * */
extern void mefSensor_clrDatoListo(void);

/* @brief Envia el pulso de trigger al sensor de ultrasonido.
* */
extern void TriggerPulse(void);

#endif /* MEFSENSOR_H_ */
