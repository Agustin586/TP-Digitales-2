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

/*< Archivos de freertos >*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*< DEFINICIONES >*/
#define G_THS				0.3
#define THS_FF 				G_THS*100
#define THS_FF_CUAD			THS_FF*THS_FF
#define MAX_RANGE_2G_CUAD	200*200
#define MAX_BUFFER			20

#define MAXCOUNT_COUNTING_SEMAPHORE	10	/*< Cantidad de eventos que puede detectar >*/
#define	INITIALCOUNT				0	/*< Cantidad inicial de eventos >*/

/*< VARIBALES >*/
typedef enum {
	EST_ISR_INT2_IDLE = 0, EST_ISR_INT2_RESET, EST_ISR_INT2_ADQ_DATOS,
} estMefInt2_enum;

static estMefInt2_enum estMefInt2;

static uint32_t Max_Norm;
static uint32_t ReadNorma;

QueueHandle_t queueNormMax, queueDatosEjes;
SemaphoreHandle_t DrdySemaphore, FFSemaphore;

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

/*
 * @brief	Activa las interrupciones desde el micro
 * */
static void activar_intFreeFall(void);
static void desactivar_intFreeFall(void);
static void activar_intDRDY(void);
static void desactivar_intDRDY(void);

static void activar_intFreeFall(void) {
	PORT_ClearPinsInterruptFlags(INT2_PORT, 1 << INT2_PIN);
	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptLogicZero);

	return;
}

static void desactivar_intFreeFall(void) {
	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN,
			kPORT_InterruptOrDMADisabled);
	PORT_ClearPinsInterruptFlags(INT2_PORT, 1 << INT2_PIN);	// Limpia bandera de interrupcion 2

	return;
}

static void activar_intDRDY(void) {
	PORT_ClearPinsInterruptFlags(INT1_PORT, 1 << INT1_PIN);
	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptLogicZero);

	return;
}

static void desactivar_intDRDY(void) {
	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN,
			kPORT_InterruptOrDMADisabled);
	PORT_ClearPinsInterruptFlags(INT1_PORT, 1 << INT1_PIN);

	return;
}

extern void taskRtos_INTFF(void *pvParameters) {
	PRINTF("> Tarea: Interrupcion FreeFall\r\n");

	FFSemaphore = xSemaphoreCreateBinary();

	if (FFSemaphore == NULL)
		vTaskDelete(NULL);

	mma8451_init();

	for (;;) {
		if (xSemaphoreTake(FFSemaphore, portMAX_DELAY)) {
			PRINTF("FreeFall!!!\r\n");

//			energia_SetClockRunFromVlpr();	/*< Modo run >*/

			mma8451_IntFF(); 			/*< Lee la bandera de interrupcion por freefall del mma8451 >*/

			mma8451_enableDRDYInt(); 	/*< Habilita la interrupcion por data ready >*/
			activar_intDRDY();
		}
	}

	vTaskDelete(NULL);

	return;
}

extern void taskRtos_INTDRDY(void *pvParameters) {
	PRINTF("> Tarea: Interrucion Dato Listo\r\n");

	DrdySemaphore = xSemaphoreCreateCounting(MAXCOUNT_COUNTING_SEMAPHORE,
			INITIALCOUNT);

	if (DrdySemaphore == NULL)
		vTaskDelete(NULL);

	energia_init();
	mefIntDRDY_init();

	for (;;) {
		if (xSemaphoreTake(DrdySemaphore, portMAX_DELAY)) {
//			PRINTF("Dato Listo!!!\r\n");
			mefIntDRDY();
			activar_intDRDY();
		}
	}

	vTaskDelete(NULL);

	return;
}

static void mefIntDRDY_init(void) {
#define CANT_ELEMENTOS_COLA		1
	estMefInt2 = EST_ISR_INT2_IDLE;
	IF_FinFreefall = false;

	/* Creamos la cola de datos */
	queueNormMax = xQueueCreate(CANT_ELEMENTOS_COLA, sizeof(uint32_t));
	if (queueNormMax == NULL) {
		PRINTF("No se puedo crear la cola de datos\r\n");
		while (1)
			;
	}

	queueDatosEjes = xQueueCreate(MAX_QUEUE_LONG, sizeof(DatosMMA8451_t));
	if (queueDatosEjes == NULL) {
		PRINTF("No se puedo crear la cola de datos\r\n");
		while (1)
			;
	}

	return;
}

static DatosMMA8451_t ReadEjes;

static void mefIntDRDY(void) {
	static uint8_t indice = 0;
	static uint32_t buffer[MAX_BUFFER];

	if (mma8451_readDRDY() == false) {
		PRINTF("\r\nError: no se leyo el mma8451\r\n");
		return;
	}

	switch (estMefInt2) {
	case EST_ISR_INT2_IDLE:
		ReadNorma = mma8451_cuadNorm();
		IF_FinFreefall = false;

		if (ReadNorma < THS_FF_CUAD)
			estMefInt2 = EST_ISR_INT2_ADQ_DATOS;
		else {
			/*
			 * Si no se detecto freefall correctamente vuelve a habilitar
			 * las interrupciones por freefall.
			 * */
			activar_intFreeFall(); 		/*< Habilita int freefall del micro >*/
			mma8451_disableDRDYInt(); 	/*< Deshabilita int dato listo del mma8451 >*/
		}
		break;
	case EST_ISR_INT2_RESET:
		mma8451_disableDRDYInt();
		mma8451_readDRDY();			/*< IMPORTANTE: Borra un ultimo dato listo >*/

		/* Carga el dato en la cola */
		if (uxQueueMessagesWaiting(queueNormMax) != 0) {
			/* Limpia el dato anterior */
			PRINTF("\r\nError: esta cola no deberia estar llenar\r\n");
//			while(1);
			queueRtos_receiveNormaMaxCuad();
		}
		xQueueSendToBack(queueNormMax, &Max_Norm, pdMS_TO_TICKS(10));

		/* Limpia las variables */
		Max_Norm = 0;
		indice = 0;
		ReadNorma = 0;
		IF_FinFreefall = true;
		clrBuffer(buffer, MAX_BUFFER);

		activar_intFreeFall();

		/*< Activamos el bajo consumo >*/
//		energia_SetClockVlpr();
		estMefInt2 = EST_ISR_INT2_IDLE;
		break;
	case EST_ISR_INT2_ADQ_DATOS:
//		mma8451_readDRDY();
		ReadNorma = mma8451_cuadNorm();

		/*< Lectura de los ejes >*/
		ReadEjes.ReadX = mma8451_getAcX();
		ReadEjes.ReadY = mma8451_getAcY();
		ReadEjes.ReadZ = mma8451_getAcZ();
		ReadEjes.NormaCuad = ReadNorma; /*< En la pantalla debemos hacer la raiz cuadrada >*/

		PRINTF("\r\nEje X: %d\r\n", ReadEjes.ReadX);
		PRINTF("Eje Y: %d\r\n", ReadEjes.ReadY);
		PRINTF("Eje Z: %d\r\n", ReadEjes.ReadZ);

		xQueueSendToBack(queueDatosEjes, &ReadEjes, pdMS_TO_TICKS(10));

		/*< Cargamos en el buffer para detectar el fin de la caida libre >*/
		buffer[indice] = ReadNorma, indice++;

		if (ReadNorma > Max_Norm)
			Max_Norm = ReadNorma;
		if (indice == MAX_BUFFER)
			indice = 0;

		if (Fin_Freefall(buffer, MAX_BUFFER)) {
			estMefInt2 = EST_ISR_INT2_RESET;
		}

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

extern bool intMma_getIFFreeFall(void) {
	return IF_FinFreefall;
}

extern void intMma_clrIFFreeFall(void) {
	IF_FinFreefall = false;

	return;
}

extern uint32_t queueRtos_receiveNormaMaxCuad(void) {
	static uint32_t read;

	if (uxQueueMessagesWaiting(queueNormMax)) {
		xQueueReceive(queueNormMax, &read, pdMS_TO_TICKS(10)); // No elimina el dato de la cola
		return read;
	}

	return read;
}

extern void queueRtos_receiveDatosEjes(DatosMMA8451_t *DatosEjes,
		uint8_t *longitud) {
	*longitud = uxQueueMessagesWaiting(queueDatosEjes);

	if (*longitud != 0) {
		xQueueReceive(queueDatosEjes, (DatosMMA8451_t *)DatosEjes, pdMS_TO_TICKS(10));
	} else {
		PRINTF("No hay datos en la cola\r\n");
	}

	return;
}

void PORTC_PORTD_IRQHandler(void) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* INTERRUPCION POR DATOS LISTOS */
	if (PORT_GetPinsInterruptFlags(INT1_PORT)) {
		xSemaphoreGiveFromISR((QueueHandle_t ) DrdySemaphore,
				&xHigherPriorityTaskWoken);
		desactivar_intDRDY();
	}

	/* INTERRUPCION POR FREEFALL */
	if (PORT_GetPinsInterruptFlags(INT2_PORT)) {
		xSemaphoreGiveFromISR((QueueHandle_t ) FFSemaphore,
				&xHigherPriorityTaskWoken);
		desactivar_intFreeFall();
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	return;
}
