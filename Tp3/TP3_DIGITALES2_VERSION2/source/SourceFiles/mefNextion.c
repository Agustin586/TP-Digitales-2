#include "IncludesFiles/mefNextion.h"
#include "IncludesFiles/nextion.h"
#include "fsl_debug_console.h"
#include "IncludesFiles/taskRtosNextion.h"
#include "IncludesFiles/Uart1.h"
#include <stdint.h>

static estMefNextion_enum estMefNextion;

extern void mefNextion_init(void) {
	estMefNextion = EST_NEXTION_pRADAR;

	return;
}

extern void mefNextion(void) {
	static uint8_t idPic=22;

	switch (estMefNextion) {
	case EST_NEXTION_RESET:
		PRINTF("No se detecto la pagina de la pantalla\r\n");
//		taskRtosNextion_error();

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pMAIN:

		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pRADAR:
		for (int16_t angle=30;angle<=150;angle=angle+15){
			nextion_putPicture(0, idPic);
			taskRtosNextion_delay(100);
			for(uint8_t radio=10;radio<=50;radio+=10){
				nextion_putObj(angle, radio, 1);
				taskRtosNextion_delay(200);
			}
			idPic++;
			if(idPic==29)	idPic=22;
			taskRtosNextion_delay(1200);
		}

//		estMefNextion = nextion_getPage();
		break;
	case EST_NEXTION_pSERVO:

		estMefNextion = nextion_getPage();
		break;
	default:
		break;
	}

	return;
}
