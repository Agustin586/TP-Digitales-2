#include "mefDisplay.h"
#include "display.h"
#include "mefSEC.h"
#include "tareasRtos.h"
#include "MACROS.h"
#include "math.h"
#include "mma8451.h"
#include "oled.h"
#include "queue.h"

typedef enum {
	EST_DISPLAY_REPOSO = 0,
	EST_DISPLAY_RESULTADOS,
	EST_DISPLAY_ERROR,
}estMefDisplay_enum;

static estMefDisplay_enum estMefDisplay;
extern xQueueHandle queueNormaRaiz;

extern void mefDisplay_init(void){
	estMefDisplay = EST_DISPLAY_REPOSO;

	return;
}
extern void mefDisplay(void){
	switch (estMefDisplay) {
	case EST_DISPLAY_REPOSO:
		display_frame();
		display_reposo();

		if (mefSEC_getEstado() == EST_SECUENCIA_RESULTADO)
			estMefDisplay = EST_DISPLAY_RESULTADOS;
		else if (mefSEC_getEstado() == EST_SECUENCIA_ERROR)
			estMefDisplay = EST_DISPLAY_ERROR;

		break;

	case EST_DISPLAY_RESULTADOS:
		display_frame();
		display_mostrarResultado((float)mefSEC_getNormaMaxima()/100.0);

		if (mefSEC_getEstado() == EST_SECUENCIA_REPOSO)
			estMefDisplay = EST_DISPLAY_REPOSO;

		break;

	case EST_DISPLAY_ERROR:
		display_frame();
		display_error();

		if (mefSEC_getEstado() == EST_SECUENCIA_REPOSO)
			estMefDisplay = EST_DISPLAY_REPOSO;

		break;

	default:
		estMefDisplay = EST_DISPLAY_REPOSO;
		break;
	}

	return;
}
