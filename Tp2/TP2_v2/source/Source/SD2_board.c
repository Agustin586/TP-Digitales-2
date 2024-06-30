/* Copyright 2023, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * Copyright 2023, Guido Cicconi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*==================[inclusions]=============================================*/
#include "Include/SD2_board.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "fsl_spi.h"
#include "Include/energia.h"
#include "Include/mma8451.h"

/*==================[macros and definitions]=================================*/

#define SPI_MASTER              SPI0
#define SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)

/*==================[internal data declaration]==============================*/
static const board_gpioInfo_type board_gpioLeds[] =
{
    {PORTE, GPIOE, 29},     /* LED ROJO */
    {PORTD, GPIOD, 5},      /* LED VERDE */
	{PORTE, GPIOE, 2},		/* LED ROJO EXTERNO */
	{PORTE, GPIOE, 3},		/* LED AZUL EXTERNO */
};

static const board_gpioInfo_type board_gpioSw[] =
{
    {PORTC, GPIOC, 3},      /* SW1 */
    {PORTC, GPIOC, 12},     /* SW3 */
};

static const board_gpioInfo_type board_gpioOled[] =
{
    {PORTE, GPIOE, 19},      /* RST */
    {PORTE, GPIOE, 31},      /* DATA/CMD */
};

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void board_init(void)
{
	int32_t i;

	gpio_pin_config_t gpio_led_config =
	{
		.outputLogic = 1,
		.pinDirection = kGPIO_DigitalOutput,
	};
	gpio_pin_config_t gpio_sw_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
	};
	gpio_pin_config_t gpio_oled_config =
	{
		.outputLogic = 0,
		.pinDirection = kGPIO_DigitalOutput,
	};

	const port_pin_config_t port_led_config = {
			/* Internal pull-up/down resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Slow slew rate is configured */
		.slewRate = kPORT_SlowSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTC3 */
		.mux = kPORT_MuxAsGpio,
	};
	const port_pin_config_t port_sw_config = {
		/* Internal pull-up resistor is enabled */
		.pullSelect = kPORT_PullUp,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTC3 */
		.mux = kPORT_MuxAsGpio,
	};
	const port_pin_config_t port_oled_config = {
			/* Internal pull-up/down resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as GPIO */
		.mux = kPORT_MuxAsGpio,
	};

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	/* inicialización de leds */
	for (i = 0 ; i < BOARD_LED_ID_TOTAL ; i++)
	{
		PORT_SetPinConfig(board_gpioLeds[i].port, board_gpioLeds[i].pin, &port_led_config);
		GPIO_PinInit(board_gpioLeds[i].gpio, board_gpioLeds[i].pin, &gpio_led_config);
	}

	/* inicialización de SWs */
	for (i = 0 ; i < BOARD_SW_ID_TOTAL ; i++){
		PORT_SetPinConfig(board_gpioSw[i].port, board_gpioSw[i].pin, &port_sw_config);
		GPIO_PinInit(board_gpioSw[i].gpio, board_gpioSw[i].pin, &gpio_sw_config);
	}

	/*Inicialización de los pines GPIO necesarios para manejar el display OLED*/
	for (i = 0 ; i < OLED_TOTAL ; i++){
		PORT_SetPinConfig(board_gpioOled[i].port, board_gpioOled[i].pin, &port_oled_config);
		GPIO_PinInit(board_gpioOled[i].gpio, board_gpioOled[i].pin, &gpio_oled_config);
	}
}

void board_setLed(board_ledId_enum id, board_ledMsg_enum msg)
{
    switch (msg)
    {
        case BOARD_LED_MSG_OFF:
        	GPIO_PortSet(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        case BOARD_LED_MSG_ON:
        	GPIO_PortClear(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        case BOARD_LED_MSG_TOGGLE:
        	GPIO_PortToggle(board_gpioLeds[id].gpio, 1<<board_gpioLeds[id].pin);
            break;

        default:
            break;
    }
}

void board_setOledPin(board_oledPin_enum oledPin, uint8_t state)
{
	GPIO_PinWrite(board_gpioOled[oledPin].gpio, board_gpioOled[oledPin].pin, state);
}

bool board_getSw(board_swId_enum id)
{
    return !GPIO_ReadPinInput(board_gpioSw[id].gpio, board_gpioSw[id].pin);
}

void board_configSPI0(){
	const port_pin_config_t port_spi_config = {
		/* Internal pull-up resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as SPI0_x */
		.mux = kPORT_MuxAlt2,
	};

	PORT_SetPinConfig(PORTE, 16, &port_spi_config); //SPI0_SS cs
	PORT_SetPinConfig(PORTE, 17, &port_spi_config); //SPI0_SCK d0
	PORT_SetPinConfig(PORTE, 18, &port_spi_config); //SPI0_MOSI d1
	//PORT_SetPinConfig(PORTE, 19, &port_spi_config); //SPI0_MISO

	CLOCK_EnableClock(kCLOCK_Spi0);

	spi_master_config_t userConfig;

	SPI_MasterGetDefaultConfig(&userConfig);

	/*
	userConfig.enableMaster         = true;
	userConfig.enableStopInWaitMode = false;
	userConfig.polarity             = kSPI_ClockPolarityActiveHigh;
	userConfig.phase                = kSPI_ClockPhaseFirstEdge;
	userConfig.direction            = kSPI_MsbFirst;
	userConfig.dataMode 			 = kSPI_8BitMode;
	userConfig.txWatermark 		 = kSPI_TxFifoOneHalfEmpty;
	userConfig.rxWatermark 		 = kSPI_RxFifoOneHalfFull;
	userConfig.pinMode      		 = kSPI_PinModeNormal;
	userConfig.outputMode   		 = kSPI_SlaveSelectAutomaticOutput;
	userConfig.baudRate_Bps 		 = 500000U;
	*/

	userConfig.polarity             = kSPI_ClockPolarityActiveLow;
	userConfig.phase                = kSPI_ClockPhaseSecondEdge;
	userConfig.baudRate_Bps 		= 4000000U;

	SPI_MasterInit(SPI_MASTER, &userConfig, SPI_MASTER_CLK_FREQ);
}

void board_SPISend(uint8_t* buf, size_t len){
	spi_transfer_t xfer;

	xfer.txData = buf;
	xfer.rxData = NULL;
	xfer.dataSize  = len;

	SPI_MasterTransferBlocking(SPI_MASTER, &xfer);
}
/*==================[end of file]============================================*/
