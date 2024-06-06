#include "mefInt2.h"
#include "SD2_board.h"
#include "mma8451.h"
#include "tareasRtos.h"

#include "MKL46Z4.h"
#include "arm_math.h"

#define G_THS				0.3
#define THS_FF 				G_THS*100
#define THS_FF_CUAD			THS_FF*THS_FF
#define MAX_RANGE_2G_CUAD	200*200
#define MAX_BUFFER			20


#define ORDEN 40+1
#define BLOCK_SIZE 1
const float32_t firCoeffs[ORDEN] = {
		0.0000000000000000068588967306406387,
		 0.0010142791605072493               ,
		-0.0018119310391016533               ,
		 0.0017221375942620678               ,
		-0.0000000000000000049048992088418132,
		-0.0032204697778627726               ,
		 0.0061270734907224516               ,
		-0.0057308129611424192               ,
		 0.000000000000000011625853328371184 ,
		 0.0095720818332516583               ,
		-0.017180062498624481                ,
		 0.015296652634883705                ,
		-0.000000000000000019933409493766732 ,
		-0.024066029340695731                ,
		 0.042970370183729199                ,
		-0.038931145442356006                ,
		 0.000000000000000026654363613296099 ,
		 0.071228811592422073                ,
		-0.1554932456343299                  ,
		 0.22369146983008356                 ,
		 0.74962164074850224                 ,
		 0.22369146983008356                 ,
		-0.1554932456343299                  ,
		 0.071228811592422073                ,
		 0.000000000000000026654363613296099 ,
		-0.038931145442356006                ,
		 0.042970370183729199                ,
		-0.024066029340695731                ,
		-0.000000000000000019933409493766732 ,
		 0.015296652634883705                ,
		-0.017180062498624481                ,
		 0.0095720818332516583               ,
		 0.000000000000000011625853328371184 ,
		-0.0057308129611424192               ,
		 0.0061270734907224516               ,
		-0.0032204697778627726               ,
		-0.0000000000000000049048992088418132,
		 0.0017221375942620678               ,
		-0.0018119310391016533               ,
		 0.0010142791605072493               ,
		 0.0000000000000000068588967306406387,
};

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
static bool IF_Freefall, Fin_Drdy = 0;

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
	static uint32_t buffer[MAX_BUFFER];
	static uint32_t readNormCuad = 0;

//	float32_t input[BLOCK_SIZE];
//	float32_t output[BLOCK_SIZE];
//	float32_t firState[BLOCK_SIZE + ORDEN - 1];
//
//	arm_fir_instance_f32 S;
//	arm_fir_init_f32(&S, ORDEN, (float32_t *)&firCoeffs[0], &firState[0], BLOCK_SIZE);

	if (!mma8451_readDRDY()) {
		return;
	}

	readNormCuad = mma8451_cuadNorm();

//	input[0] = mma8451_cuadNorm();
//	arm_fir_f32(&S,input,output,BLOCK_SIZE);

//	readNormCuad = output[0];

	switch (estMefInt2) {
	/*
	 * DESCRIPCION: Interpreta si se encuentra en freefall
	 * o hubo algun error casual.
	 * */
	case EST_ISR_INT2_FREEFALL:
//		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
//						mma8451_getAcY(), mma8451_getAcZ());

//		if (readNormCuad < THS_FF_CUAD)
			estMefInt2 = EST_ISR_INT2_MAXNORM;
			indice=0;
//		else if (readNormCuad > THS_FF_CUAD)
//			estMefInt2 = EST_ISR_INT2_RESET;

		break;

		/*
		 * DESCRIPCION: Toma los datos de la caida hasta que
		 * se entran (los datos) en una banda de valores, donde
		 * se considerará que se terminó la caida libre.
		 * */
	case EST_ISR_INT2_MAXNORM:
//		PRINTF("TOMANDO DATOS DEL MMA8451\r\n");

		buffer[indice] = readNormCuad;
		indice++;

		if (readNormCuad > Max_NormCuad && readNormCuad <= MAX_RANGE_2G_CUAD) {
			Max_NormCuad = readNormCuad;
			PRINTF("Valor Norma cuadrado:%d\r\n", readNormCuad);
//			PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
//					mma8451_getAcY(), mma8451_getAcZ());
		}

		if (indice == MAX_BUFFER)
			indice = 0;

//		PRINTF("Valor Norma cuadrado:%d\r\n", readNormCuad);
//		PRINTF("EjeX:%d\r\nEjeY:%d\r\nEjeZ:%d\r\n", mma8451_getAcX(),
//				mma8451_getAcY(), mma8451_getAcZ());

		if (Fin_Freefall(buffer, MAX_BUFFER))
			estMefInt2 = EST_ISR_INT2_ENDFREEFALL;

		break;
	case EST_ISR_INT2_ENDFREEFALL:
		PRINTF("FIN FREEFALL\r\n");

		xQueueSendToBack(queue_NormaMaxima, &Max_NormCuad, DELAY_10ms);

		IF_Freefall = true;

		estMefInt2 = EST_ISR_INT2_RESET;

		break;
	case EST_ISR_INT2_RESET:
		mma8451_disableDRDYInt();

		Max_NormCuad = 0;
		indice = 0;
		readNormCuad = 0;
		clrBuffer(buffer, MAX_BUFFER);
		Fin_Drdy = true;

		PRINTF("Reiniciando MEF\r\n");

		estMefInt2 = EST_ISR_INT2_FREEFALL;

		break;
	default:
		break;
	}

	return;
}

extern bool mefInt2_getFDrdy(void) {
	return Fin_Drdy;
}

extern void mefInt2_clrFDrdy(void) {
	Fin_Drdy = false;
	return;
}

extern bool mefInt2_getIF_Freefall(void) {
	return IF_Freefall;
}

extern void mefInt2_clrIF_Freefall(void) {
	IF_Freefall = false;
	return;
}

static bool Fin_Freefall(uint32_t buffer[], uint8_t max_indice) {
#define THS_FIN_FREEFALL_MIN	99*99
#define THS_FIN_FREEFALL_MAX	102*102
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
