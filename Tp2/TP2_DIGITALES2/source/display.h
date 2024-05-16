#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "oled.h"
#include <stdint.h>
#include <stdio.h>

/*
 * @brief Pantalla de presentación
 * */
extern void display_init(void);

/*
 * @brief Pantalla de reposo
 * */
extern void display_reposo(void);

/*
 * @brief Borrado de pantalla
 * */
extern void display_frame(void);

/*
 * @brief Muestra de resultados
 * */
extern void display_mostrarResultado(float res);

#endif /* DISPLAY_H_ */
