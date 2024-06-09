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

volatile uint32_t g_echoDuration = 0;
volatile bool g_captureFlag = false;
volatile bool distanceReady = false;
//-------------------------------------------------------------------------

extern void HCSR04_init() {
	tpm_config_t tpmInfo;

	/* Initialize TPM module */
	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_16;  // Set preescaler to 2
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

	TPM_SetTimerPeriod(BOARD_TPM_BASEADDR, 65535);

	/* Set up input capture on the specified channel */
	TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL,
			kTPM_RisingEdge);

	/* Set the timer overflow interrupt */
	TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_Chnl1InterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(TPM1_IRQn);

	return;
}

extern void HCSR04_setTrigger(void) {
	GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, HIGH);

	return;
}

extern void HCSR04_clrTrigger(void) {
	GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, LOW);
	TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);

	return;
}

extern bool HCSR04_distanceReady(void) {
	return distanceReady;
}

extern float HCSR04_getDistance(void) {
	distanceReady = false;
	return g_echoDuration * 0.333525 * 0.0343 / 2.0;
}

void TPM1_IRQHandler(void) {
	static uint16_t end_time = 0;

	// Verifica si la captura de entrada ha ocurrido
	if (TPM_GetStatusFlags(BOARD_TPM_BASEADDR) & BOARD_TPM_CHANNEL_FLAG) {
		if (!g_captureFlag) {
			TPM_StopTimer(BOARD_TPM_BASEADDR);
			BOARD_TPM_BASEADDR->CNT = 0;
			TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
			TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_CHANNEL,
					kTPM_FallingEdge);
			g_captureFlag = true;
		} else {
			TPM_StopTimer(BOARD_TPM_BASEADDR);
			end_time = TPM_GetCurrentTimerCount(BOARD_TPM_BASEADDR);
			g_echoDuration = end_time;
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
