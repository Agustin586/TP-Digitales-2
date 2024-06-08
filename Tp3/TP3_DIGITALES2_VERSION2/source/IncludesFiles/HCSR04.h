#ifndef HCSR04_H_
#define HCSR04_H_

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include <stdbool.h>
#include "fsl_tpm.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/
#define HCSR04_TRIGGER_PORT PORTB
#define HCSR04_TRIGGER_GPIO GPIOB
#define HCSR04_TRIGGER_PIN 0

#define HCSR04_ECHO_PORT PORTB
#define HCSR04_ECHO_GPIO GPIOB
#define HCSR04_ECHO_PIN 1

#define HCSR04_ECHO_INT_PRIORITY 2

/*==================[external functions definition]==========================*/


/** \brief Inicializa lo necesario para usar el sensor.
 **
 **/

extern void HCSR04_init();

/** \brief Dispara un pulso de disparo sobre el sensor.
 **
 **/

extern void HCSR04_setTrigger(void);

/** \brief Devuelve la echo_flag, indicando que no hay mediciones en proceso.
 **
 **	\return 0 si hay una medicion en proceso, 1 si no hay mediciones en proceso.
 **
 **/

extern bool HCSR04_distanceReady(void);


/** \brief Devuelve la ultima distancia detectada por el sensor, en mm.
 **
 ** \return La ultima medicion detectada por el sensor en mm.
 **
 **/

extern float HCSR04_getDistance(void);


/*
 * @brief Pone en bajo el valor del pin de trigger
 * */
extern void HCSR04_clrTrigger(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* HCSR04_H_ */
