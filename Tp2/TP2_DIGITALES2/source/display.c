#include "display.h"
#include "oled.h"
#include <stdint.h>
#include <stdio.h>

#define BLANCO	OLED_COLOR_WHITE
#define NEGRO	OLED_COLOR_BLACK

extern void display_init(void){
	oled_clearScreen(NEGRO);
//	oled_fillRect(5, 25, 125, 70, BLANCO);
	oled_putString(15, 32, (uint8_t *)"MEDICION DE CAIDA", BLANCO, NEGRO);
	oled_putString(50, 50, (uint8_t *)"LIBRE", BLANCO, NEGRO);
	oled_putString(10, 10, (uint8_t *)"Ballin, Zuliani", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	return;
}

extern void display_reposo(void){
	oled_putString(35, 20, (uint8_t *) "ESPERANDO", BLANCO, NEGRO);
	oled_putString(30, 30, (uint8_t *) "CAIDA LIBRE", BLANCO, NEGRO);

	return;
}

extern void display_frame(void){
	oled_clearScreen(NEGRO);

	return;
}

static char* intToString(float numero) {
  static char buffer[40];   // Buffer para almacenar la cadena resultante, ajusta el tamaño según sea necesario
  sprintf(buffer, "%.2f g", numero);   // Convierte el número a una cadena de caracteres

  return buffer;
}

extern void display_mostrarResultado(float res){
	oled_putString(15, 32, (uint8_t *)"ACELERACION MAXIMA", BLANCO, NEGRO);
	oled_putString(50, 50, (uint8_t *)intToString(res), BLANCO, NEGRO);

	return;
}





