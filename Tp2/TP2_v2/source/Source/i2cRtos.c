/*
 * i2cConfig.c
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

/*< Archivos >*/
#include "Include/i2cRtos.h"
#include "fsl_debug_console.h"
#include "fsl_i2c_freertos.h"

/*< Definiciones >*/
#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U

#define I2C_BAUDRATE 			100000U

extern void i2cRtos_init(void) {
	i2c_rtos_handle_t master_rtos_handle;
	i2c_master_config_t masterConfig;
	uint32_t sourceClock;
    status_t status;

	/*
	 * masterConfig.baudRate_Bps = 100000U;
	 * masterConfig.enableStopHold = false;
	 * masterConfig.glitchFilterWidth = 0U;
	 * masterConfig.enableMaster = true;
	 */
	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = I2C_BAUDRATE;
	sourceClock = CLOCK_GetFreq(I2C0_CLK_SRC);

	status = I2C_RTOS_Init(&master_rtos_handle, I2C0,
			&masterConfig, sourceClock);

	if (status != kStatus_Success) {
		PRINTF("I2C master: error during init, %d", status);
	}

	return;
}
