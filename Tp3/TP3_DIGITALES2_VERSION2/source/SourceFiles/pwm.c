#include "IncludesFiles/pwm.h"
#include "board.h"
#include "fsl_tpm.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgFllClk)
#define TPM_CHANNEL	kTPM_Chnl_4

extern void pwm_init(void) {
	tpm_config_t tpmInfo;
	tpm_chnl_pwm_signal_param_t tpmParam;

	// Configuración del PWM
	tpmParam.chnlNumber = kTPM_Chnl_4; // PTE31
	tpmParam.level = kTPM_LowTrue;
	tpmParam.dutyCyclePercent = 7.5;

	CLOCK_EnableClock(kCLOCK_PortD);
	PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt4);

	CLOCK_SetTpmClock(1U);

	// Inicializa el módulo TPM0
	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_128;

	TPM_Init(TPM0, &tpmInfo);

	// Calcular el valor del divisor del reloj para lograr la frecuencia deseada
	TPM_SetupPwm(TPM0, &tpmParam, 1, kTPM_EdgeAlignedPwm, 50U,
	TPM_SOURCE_CLOCK);

	// Iniciar el TPM
	TPM_StartTimer(TPM0, kTPM_SystemClock);

	return;
}

extern void pwm_setDuty(uint8_t dutty) {
	TPM_UpdatePwmDutycycle(TPM0, TPM_CHANNEL, kTPM_EdgeAlignedPwm, dutty);

	return;
}

extern void pwm_stop(void) {
	TPM_UpdatePwmDutycycle(TPM0, TPM_CHANNEL, kTPM_EdgeAlignedPwm, 0);
	TPM_StopTimer(TPM0);

	return;
}

extern void pwm_reStart(void) {
	TPM_StartTimer(TPM0, kTPM_SystemClock);
	TPM_UpdatePwmDutycycle(TPM0, kTPM_Chnl_1, kTPM_EdgeAlignedPwm, 50);

	return;
}
