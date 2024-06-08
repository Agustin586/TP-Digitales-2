#include "IncludesFiles/HCSR04.h"

// Las ultimas mediciones de distancia y tiempo, expresadas en milimetros,
// y microsegundos respecitvamente.
static uint32_t duracion_us = 0;
static float distancia_mm = 0.0;

static const float velocidad_sonido_mm_us = 0.34;

// Se pone en 0 cuando se genera un disparo del sensor.
// Se pone en 1 cuando se completa la lectura de la senal 'echo'.
static uint8_t echo_flag = 0;

// Se pone en 1 cuando se elevo el pulso de trigger.
// Se pone en 0 cuando ya termino el pulso.
static uint8_t trigger_flag = 0;

//-------------------------------------------------------------------------

extern void HCSR04_init() {
#define BOARD_INITPINS_TPM_CHANNEL 1U
#define BOARD_TPM_INPUT_CAPTURE_CHANNEL kTPM_Chnl_1 // pin 13 puerto A
#define BOARD_TPM_BASEADDR TPM1

	// Configuracion del trigger
	gpio_pin_config_t gpio_trigger_config = { .outputLogic = 1, //logica invertida
			.pinDirection = kGPIO_DigitalOutput, };

	const port_pin_config_t port_trigger_config = {
	/* Internal pull-up/down resistor is disabled */
	.pullSelect = kPORT_PullDisable,
	/* Slow slew rate is configured */
	.slewRate = kPORT_SlowSlewRate,
	/* Passive filter is disabled */
	.passiveFilterEnable = kPORT_PassiveFilterDisable,
	/* Low drive strength is configured */
	.driveStrength = kPORT_LowDriveStrength,
	/* Pin is configured as PTC3 */
	.mux = kPORT_MuxAsGpio, };

	// Configuracion del echo
	gpio_pin_config_t gpio_echo_config = { .pinDirection = kGPIO_DigitalInput,
			.outputLogic = 0, };

	const port_pin_config_t port_echo_config = {
	/* Internal pull-up resistor is enabled */
	.pullSelect = kPORT_PullDown,
	/* Fast slew rate is configured */
	.slewRate = kPORT_FastSlewRate,
	/* Passive filter is disabled */
	.passiveFilterEnable = kPORT_PassiveFilterDisable,
	/* Low drive strength is configured */
	.driveStrength = kPORT_LowDriveStrength,
	/* Pin is configured as PTC3 */
	.mux = kPORT_MuxAsGpio, };

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	PORT_SetPinConfig(HCSR04_TRIGGER_PORT, HCSR04_TRIGGER_PIN,
			&port_trigger_config);
	GPIO_PinInit(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, &gpio_trigger_config);

	PORT_SetPinConfig(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN, &port_echo_config);
	GPIO_PinInit(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN, &gpio_echo_config);

	// Se inicializa el tpm1 para usar input capture en la senal de echo.
	tpm_config_t tpmInfo;

	/* Configura la estructura de información del TPM */
	TPM_GetDefaultConfig(&tpmInfo);
	tpmInfo.prescale = kTPM_Prescale_Divide_128;

	/* Inicializa el TPM */
	TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

	/* Configura el canal para captura de entrada, se detectan ambos flancos */
	TPM_SetupInputCapture(BOARD_TPM_BASEADDR, BOARD_TPM_INPUT_CAPTURE_CHANNEL,
			kTPM_RiseAndFallEdge);

	return;
}

extern void HCSR04_setTrigger(void) {
	echo_flag = 0;
	trigger_flag = 1;

	// Se pone en alto el trigger. Luego se pondra en bajo apenas se detecte un cambio en echo.
	GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, 1);

	return;
}

extern bool HCSR04_distanceReady(void) {
	return echo_flag;
}

extern float HCSR04_getDistance(void) {
	return distancia_mm;
}

void TPM1_IRQHandler(void) {
	// Verifica si la captura de entrada ha ocurrido
	if (TPM_GetStatusFlags(TPM1) & kTPM_Chnl1Flag) {
		static uint32_t captura_anterior = 0, captura_ahora = 0;

		// Borra la bandera de interrupción
		TPM_ClearStatusFlags(TPM1, kTPM_Chnl1Flag);

		// Si se detecta flanco descendente... (recordar logica invertida a las entradas)
		if (GPIO_ReadPinInput(HCSR04_ECHO_GPIO, HCSR04_ECHO_PIN) == 0) {

			// Pone en 0 el trigger y su bandera.
			GPIO_PinWrite(HCSR04_TRIGGER_GPIO, HCSR04_TRIGGER_PIN, 0);
			trigger_flag = 0;

			captura_anterior = TPM1->CONTROLS[BOARD_INITPINS_TPM_CHANNEL].CnV;
		}

		// Si se detecta flanco ascendente...
		else {
			captura_ahora = TPM1->CONTROLS[BOARD_INITPINS_TPM_CHANNEL].CnV;

			// Calcula la diferencia de tiempo
			duracion_us = captura_ahora - captura_anterior; // en microsegundos

			distancia_mm = ((float) duracion_us) * velocidad_sonido_mm_us / 2.0;

			echo_flag = 1;
		}
	}

	return;
}
