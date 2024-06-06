#include "IncludesFiles/mefProcData.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	EST_PROCESSING_IDLE = 0,
	EST_PROCESSING_DATA_LED,
	EST_PROCESSING_SENSOR,
	EST_PROCESSING_PUSH,
	EST_PROCESSING_INFOSENSOR,
	EST_PROCESSING_RESET,
} estMefProcData_enum;

static estMefProcData_enum estMefProcData;
static bool ProcData = false;

extern void mefProcData_init(void) {
	estMefProcData = EST_PROCESSING_IDLE;

	return;
}

extern void mefProcData(void) {
	switch (estMefProcData) {
	case EST_PROCESSING_IDLE:
		ProcData = true;

		break;
	case EST_PROCESSING_DATA_LED:

		break;
	case EST_PROCESSING_SENSOR:

		break;
	case EST_PROCESSING_PUSH:

		break;
	case EST_PROCESSING_INFOSENSOR:

		break;
	case EST_PROCESSING_RESET:

		break;
	default:
		break;
	}

	return;
}
