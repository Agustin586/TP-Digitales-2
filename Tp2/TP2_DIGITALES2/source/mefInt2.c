#include "mefInt2.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "tareasRtos.h"

#define G_THS				0.3
#define THS_FF 				G_THS*100
#define THS_FF_CUAD			THS_FF*THS_FF
#define MAX_RANGE_2G_CUAD	200*200
#define MAX_BUFFER			10

typedef enum {
	EST_ISR_INT2_FREEFALL = 0,
	EST_ISR_INT2_MAXNORM,
	EST_ISR_INT2_ENDFREEFALL,
	EST_ISR_INT2_RESET,
} est_MefInt2_enum;

/*
 * @brief Pregunta si bajo ciertas condiciones termina la caida libre
 *
 * @param	uint32_t buffer[] Ultimos valores de buffer
 * 			uint8_t max_indice Cantidad maxima de buffer
 * @return bool
 * */
static bool Fin_Freefall(uint32_t buffer[], uint8_t max_indice);

/*
 * @brief Limpia el buffer que utilizamos para meter nuevos
 * valores de normas al cuadrado.
 *
 * @param	uint32_t buffer[] Ultimos valores de buffer
 * 			uint8_t max_indice Cantidad maxima de buffer
 * @return bool
 * */
static void clrBuffer(uint32_t buffer[], uint8_t max_indice);

static est_MefInt2_enum estMefInt2;
static uint32_t Max_NormCuad;
bool IF_Freefall;

extern xQueueHandle queue_NormaMaxima;

extern void mefInt2_init(void) {
	mefInt2_reset();

	return;
}

extern void mefInt2_reset(void) {
	estMefInt2 = EST_ISR_INT2_FREEFALL;

	return;
}

extern void mefInt2(void) {
	static uint8_t indice = 0;
	static uint32_t
	buffer[MAX_BUFFER];
	static uint32_t readNormCuad = 0;

//	mma8451_activar();

	switch (estMefInt2) {
	case EST_ISR_INT2_FREEFALL:
		mma8451_readDRDY();
		readNormCuad = mma8451_cuadNorm();

		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
						mma8451_getAcY(), mma8451_getAcZ());

		if (readNormCuad < THS_FF_CUAD)
			estMefInt2 = EST_ISR_INT2_MAXNORM;
		else if (readNormCuad > THS_FF_CUAD)
			estMefInt2 = EST_ISR_INT2_RESET;

		break;
	case EST_ISR_INT2_MAXNORM:
		mma8451_readDRDY();
		readNormCuad = mma8451_cuadNorm();

		buffer[indice] = readNormCuad;
		indice++;

		if (readNormCuad > Max_NormCuad && readNormCuad <= MAX_RANGE_2G_CUAD) {
			Max_NormCuad = readNormCuad;
		}

		if (indice == MAX_BUFFER) indice = 0;

		PRINTF("Valor Norma cuadrado:%d\r\n", readNormCuad);
		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
				mma8451_getAcY(), mma8451_getAcZ());

		if (Fin_Freefall(buffer,MAX_BUFFER)||readNormCuad==0) estMefInt2 = EST_ISR_INT2_ENDFREEFALL;

		break;
	case EST_ISR_INT2_ENDFREEFALL:
		mma8451_readDRDY();
		readNormCuad = mma8451_cuadNorm();


		xQueueSendToBack(queue_NormaMaxima, &Max_NormCuad, DELAY_100ms);

		IF_Freefall = 1;

		estMefInt2 = EST_ISR_INT2_RESET;

		break;
	case EST_ISR_INT2_RESET:
		mma8451_disableDRDYInt();
		mma8451_readDRDY();
//		readNormCuad = mma8451_cuadNorm();

		Max_NormCuad = 0;
		indice = 0;
		readNormCuad = 0;
		clrBuffer(buffer,MAX_BUFFER);

		estMefInt2 = EST_ISR_INT2_FREEFALL;

		break;
	default:
		break;
	}

	return;
}

extern bool mefInt2_getIF_Freefall(void) {
	return IF_Freefall;
}

extern void mefInt2_clrIF_Freefall(void) {
	IF_Freefall = 0;
	return;
}

static bool Fin_Freefall(uint32_t buffer[], uint8_t max_indice) {
#define THS_FIN_FREEFALL_MIN	99*99
#define THS_FIN_FREEFALL_MAX	101*101
	for (uint8_t i = 0; i < max_indice; i++) {
		if (buffer[i] > THS_FIN_FREEFALL_MAX || buffer[i] < THS_FIN_FREEFALL_MIN)
			return false;
	}

	return true;
}

static void clrBuffer(uint32_t buffer[], uint8_t max_indice) {
	for (uint8_t i = 0; i < max_indice; i++) {
		buffer[i] = 0;
	}

	return;
}
