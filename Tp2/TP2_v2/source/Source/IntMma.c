/*
 * IntMma.c
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

/*< Archvios varios >*/
#include "Include/IntMma.h"
#include "Include/MACROS.h"
#include "Include/mma8451.h"
#include "Include/energia.h"
#include "stdint.h"
#include "stdbool.h"

/*< Archivos de freertos >*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*< DEFINICIONES >*/
#define G_THS				0.3
#define THS_FF 				G_THS*100
#define THS_FF_CUAD			THS_FF*THS_FF
#define MAX_RANGE_2G_CUAD	200*200
#define MAX_BUFFER			20
#define MAX_QUEUE_LONG		100

/*< VARIBALES >*/
typedef enum {
	EST_ISR_INT2_IDLE = 0, EST_ISR_INT2_RESET, EST_ISR_INT2_ADQ_DATOS,
} estMefInt2_enum;

typedef struct {
	int16_t ReadX;
	int16_t ReadY;
	int16_t ReadZ;
	uint32_t NormaCuad;
} DatosMMA8451_t;

static estMefInt2_enum estMefInt2;

static uint32_t ReadNorma;
static uint32_t Max_Norm;

QueueHandle_t queueNormMax, queueDatosEjes;

/*< Banderas >*/
static bool IF_FinFreefall = false;

/*< FUNCIONES >*/
static void mefIntDRDY_init(void);
static void mefIntDRDY(void);

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

extern void taskRtos_INTFF(void *pvParameters) {

	for (;;) {

	}
	vTaskDelete(NULL);

	return;
}

extern void taskRtos_INTDRDY(void *pvParameters) {
	energia_init();
	mefIntDRDY_init();

	for (;;) {
		mefIntDRDY();
	}
	vTaskDelete(NULL);

	return;
}

static void mefIntDRDY_init(void) {
#define CANT_ELEMENTOS_COLA		2
	estMefInt2 = EST_ISR_INT2_IDLE;
	IF_FinFreefall = false;

	/* Creamos la cola de datos */
	xQueueCreate(queueNormMax, CANT_ELEMENTOS_COLA);
	xQueueCreate(queueDatosEjes, MAX_QUEUE_LONG);

	return;
}
static void mefIntDRDY(void) {
	static uint8_t indice = 0;
	static uint32_t buffer[MAX_BUFFER];

	switch (estMefInt2) {
	case EST_ISR_INT2_IDLE:
		mma8451_readDRDY();
		ReadNorma = mma8451_cuadNorm();

		if (ReadNorma < THS_FF_CUAD)
			estMefInt2 = EST_ISR_INT2_ADQ_DATOS;
		break;
	case EST_ISR_INT2_RESET:
		mma8451_disableDRDYInt();

		/* Carga el dato en la cola */
		xQueueSendToBack(queueNormMax, &Max_Norm, delay_ms(100));

		/* Limpia las variables */
		Max_Norm = 0;
		indice = 0;
		ReadNorma = 0;
		clrBuffer(buffer, MAX_BUFFER);
		energia_SetClockVlpr();

		break;
	case EST_ISR_INT2_ADQ_DATOS:
		DatosMMA8451_t ReadEjes;

		mma8451_readDRDY();
		ReadNorma = mma8451_cuadNorm();

		/*< Lectura de los ejes >*/
		ReadEjes.ReadX = mma8451_getAcX();
		ReadEjes.ReadY = mma8451_getAcY();
		ReadEjes.ReadZ = mma8451_getAcZ();
		ReadEjes.NormaCuad = ReadNorma; /*< En la pantalla debemos hacer la raiz cuadrada >*/

		xQueueSendToBack(queueDatosEjes, &ReadEjes, delay_ms(100));

		/*< Cargamos en el buffer para detectar el fin de la caida libre >*/
		buffer[indice] = ReadNorma, indice++;

		if (ReadNorma > Max_Norm)
			Max_Norm = ReadNorma;
		if (indice == MAX_BUFFER)
			indice = 0;

		if (Fin_Freefall(buffer, MAX_BUFFER))
			estMefInt2 = EST_ISR_INT2_RESET;

		break;
	default:
		break;
	}
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
