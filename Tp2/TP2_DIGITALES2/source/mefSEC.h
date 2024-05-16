#ifndef MEFSEC_H_
#define MEFSEC_H_

#include <stdint.h>

typedef enum {
	EST_SECUENCIA_REPOSO = 0,
	EST_SECUENCIA_CAIDALIBRE,
	EST_SECUENCIA_RESULTADO,
}estMefSec_enum;

/*
 * @brief Inicialización de la MEF SECUENCIA
 * */
extern void mefSEC_init(void);

/*
 * @brief MEF SECUENCIA
 * */
extern void mefSEC(void);

/*
 * @brief Indica el estado en el que se encuentra la mefSEC
 *
 * @return estMefSec_enum Estado de la mefSEC
 * */
extern estMefSec_enum mefSEC_getEstado(void);

/*
 * @brief Obtiene la Norma máxima que se presentó durante la caída
 *
 * @return uint16_t Norma Máxima
 * */
extern uint16_t mefSEC_getNormaMaxima(void);

/*
 * @brief Sin sentido ahora que se usa un RTOS
 * */
extern void mefSEC_periodicTask1ms(void);

#endif /* MEFSEC_H_ */
