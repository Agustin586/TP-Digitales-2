#include <fsl_common.h>
#include <IncludesFiles/HCSR04.h>
#include <MKL46Z4.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_TPM_CHANNEL kTPM_Chnl_1
#define BOARD_TPM_CHANNEL_FLAG	kTPM_Chnl1Flag
#define BOARD_TPM_BASEADDR TPM1
#define HIGH	1
#define LOW		0

// Las ultimas mediciones de distancia y tiempo, expresadas en milimetros,
// y microsegundos respecitvamente.
//static uint32_t duracion_us = 0;
//static float distancia_mm = 0.0;
volatile uint32_t g_echoDuration = 0;
volatile bool g_captureFlag = false;
volatile bool distanceReady = false;

static const float velocidad_sonido_mm_us = 0.0343;

// Se pone en 0 cuando se genera un disparo del sensor.
// Se pone en 1 cuando se completa la lectura de la senal 'echo'.
//static uint8_t echo_flag = true;

// Se pone en 1 cuando se elevo el pulso de trigger.
// Se pone en 0 cuando ya termino el pulso.
//static uint8_t trigger_flag = false;

//-------------------------------------------------------------------------

extern void HCSR04_init() {
	tpm_config_t tpmInfo;

	/* Initialize TPM module */
	TPM_GetDefaultConfig(&tpmInfo);
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

	/* Set up input capture on the specified channel */
	TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL,
			kTPM_RisingEdge);

	/* Set the timer overflow interrupt */
//	TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);
	TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_Chnl1InterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(TPM1_IRQn);

	/* Start the TPM counter */
	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

	return;
}

extern void HCSR04_setTrigger(void) {
	GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, HIGH);

	return;
}

extern void HCSR04_clrTrigger(void) {
	GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, LOW);

	return;
}

extern bool HCSR04_distanceReady(void) {
	return distanceReady;
}

extern float HCSR04_getDistance(void) {
	distanceReady = false;
	return g_echoDuration * velocidad_sonido_mm_us / 2.0;
}

void TPM1_IRQHandler(void) {
	static uint32_t start_time = 0, end_time = 0;

	// Verifica si la captura de entrada ha ocurrido
	if (TPM_GetStatusFlags(BOARD_TPM_BASEADDR) & BOARD_TPM_CHANNEL_FLAG) {
		if (!g_captureFlag) {
			start_time = TPM_GetCurrentTimerCount(BOARD_TPM_BASEADDR);
			g_captureFlag = true;
			TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL,
					kTPM_FallingEdge);
		} else {
			end_time = TPM_GetCurrentTimerCount(BOARD_TPM_BASEADDR);
			g_echoDuration = abs(end_time - start_time);
			g_captureFlag = false;
			TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL,
					kTPM_RisingEdge);
			distanceReady = true;
		}

	}

	// Borra la bandera de interrupción
	TPM_ClearStatusFlags(TPM1, BOARD_TPM_CHANNEL_FLAG);

	return;
}
