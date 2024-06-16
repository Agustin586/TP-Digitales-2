#ifndef MEFSERVO_H_
#define MEFSERVO_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/*
 * @brief Incializa la mef del servo motor.
 * */
extern void mefServo_init(void);

/*
 * @brief Mef del servo motor.
 * */
extern void mefServo(void);

/*
 * @brief Devuelve el angulo en el que se encuentra el servo motor.
 *
 * @return int8_t.
 * */
extern int8_t mefServo_getAngle(void);

#endif /* MEFSERVO_H_ */
