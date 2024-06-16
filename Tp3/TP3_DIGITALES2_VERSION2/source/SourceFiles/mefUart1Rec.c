#include "IncludesFiles/mefUart1Rec.h"
#include "IncludesFiles/queueRtos.h"
#include "IncludesFiles/nextion.h"
#include <stdint.h>
#include "fsl_debug_console.h"

typedef enum {
	EST_IDLE = 0, EST_WAIT_PAGE_ID, EST_WAIT_FF1, EST_WAIT_FF2, EST_WAIT_FF3,
} estMefUart1Rec_enum;

static estMefUart1Rec_enum estMefUart1Rec;
uint8_t pageID;

extern void mefUart1Rec_init(void) {
	estMefUart1Rec = EST_IDLE;

	return;
}

extern void mefUart1Rec(void) {
	uint8_t data;

	if (queueRtos_msgWaiting(QUEUE_ID_1))
		queueRtos_receiveFromQueue(QUEUE_ID_1, &data, pdMS_TO_TICKS(100));
	else
		return;

	switch (estMefUart1Rec) {
	case EST_IDLE:
		if (data == 0x66)
			estMefUart1Rec = EST_WAIT_PAGE_ID;

		break;

	case EST_WAIT_PAGE_ID:
		pageID = data;
		nextion_setPageID(pageID);

		estMefUart1Rec = EST_WAIT_FF1;
		break;

	case EST_WAIT_FF1:
		if (data == 0xFF) {
			estMefUart1Rec = EST_WAIT_FF2;
		} else {
			estMefUart1Rec = EST_IDLE;
		}

		break;

	case EST_WAIT_FF2:
		if (data == 0xFF) {
			estMefUart1Rec = EST_WAIT_FF3;
		} else {
			estMefUart1Rec = EST_IDLE;
		}

		break;

	case EST_WAIT_FF3:
		if (data == 0xFF) {
			PRINTF("Fin de la trama recivida, pagina:%d\r\n", pageID);
		}
		estMefUart1Rec = EST_IDLE;

		break;
	default:
		break;
	}

	return;
}
