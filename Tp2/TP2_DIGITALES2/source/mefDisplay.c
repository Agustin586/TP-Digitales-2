#include "mefDisplay.h"
#include "display.h"
#include "SD2_board.h"
#include "oled.h"
#include "MACROS.h"

typedef enum {
	EST_DISPLAY_REPOSO = 0,
	EST_DISPLAY_RESULTADO_FINAL,
}estMefDisplay_enum;

static estMefDisplay_enum estMefDisplay;
static uint16_t Delay_ms, Delay_Clr;

/*
 * Nombre: mefDisplay_init
 *
 * Entrada: void
 * Salida: void
 *
 * Descripción: Inicializa la mef del display.
 *
 * */
extern void mefDisplay_init(void){
//	/* Modulo spi */
//	board_configSPI0();
//
//	/* Modulo pantalla oled */
//	oled_reset();

//	display_init();
//
//	estMefDisplay = EST_DISPLAY_REPOSO;
//
//	Delay_ms = DELAY_500ms;
//	Delay_Clr = FRAME_HZ_ticks(5);

	return;
}

/*
 * Nombre: mefDisplay
 *
 * Entrada: void
 * Salida: void
 *
 * Descripción: Mef del display.
 *
 * */
extern void mefDisplay(void){
//	switch (estMefDisplay) {
//		case EST_DISPLAY_REPOSO:
//			if (!Delay_Clr) {
//				if (!Delay_ms) {
//					display_frame();
//					display_reposo();
//				}
//				Delay_Clr = FRAME_HZ_ticks(5);
//			}
//
//			/* CAMBIO DE ESTADO */
//			if (mefSecuencia_getEstado() == EST_SECUENCIA_RESULTADO) {
//				estMefDisplay = EST_DISPLAY_RESULTADO_FINAL;
//			}
//
//			break;
//		case EST_DISPLAY_RESULTADO_FINAL:
//			if (!Delay_Clr) {
//				display_frame();
//				display_mostrarResultado(mefSecuencia_getNormaMaxima()/100.0);
//				Delay_Clr = FRAME_HZ_ticks(1.5);
//			}
//
//			/* CAMBIO DE ESTADO */
//			if (mefSecuencia_getEstado() == EST_SECUENCIA_REPOSO) {
//				estMefDisplay = EST_DISPLAY_REPOSO;
//				display_frame();
//			}
//			break;
//	}

	return;
}

extern void mefDisplay_task1ms(void){
	if (Delay_ms) {
		Delay_ms--;
	}
	if (Delay_Clr) {
		Delay_Clr--;
	}
}
