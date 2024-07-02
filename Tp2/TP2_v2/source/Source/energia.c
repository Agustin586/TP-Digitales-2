/* Copyright 2019, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2019, Esteban Almiron
 * Copyright 2019, Gustavo Muro
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

#include "Include/energia.h"

/**
 *	Función: energia_SetClockVlpr
 *
 * 	Entradas: void
 * 	Salidas:  void
 *
 * 	Descripción:
 * 		Configura el microcontrolador para que trabaje en modo de
 * 		bajo consumo.
 */
extern void energia_SetClockVlpr(void) {
	const sim_clock_config_t simConfig = { .pllFllSel = 0U, /* PLLFLLSEL select FLL */
	.er32kSrc = 3U, /* ERCLK32K selection, use LPO */
	.clkdiv1 = 0x00040000U, /* SIM_CLKDIV1 */
	};

	CLOCK_SetSimSafeDivs();
	CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

	/* MCG works in PEE mode now, will switch to BLPI mode. */
	CLOCK_ExternalModeToFbeModeQuick(); /* Enter FBE. */
	CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
	CLOCK_SetLowPowerEnable(true); /* Enter BLPI. */

	CLOCK_SetSimConfig(&simConfig);

	SMC_SetPowerModeVlpr(SMC);
}

/**
 *	Función: energia_SetClockRunFromVlpr
 *
 * 	Entradas: void
 * 	Salidas:  void
 *
 * 	Descripción:
 * 		Configura el microcontrolador para que trabaje en modo de
 * 		alto consumo.
 */
extern void energia_SetClockRunFromVlpr(void) {
	const sim_clock_config_t simConfig = { .pllFllSel = 1U, /* PLLFLLSEL select PLL */
	.er32kSrc = 3U, /* ERCLK32K selection, use LPO */
	.clkdiv1 = 0x10010000U, /* SIM_CLKDIV1 */
	};

	const mcg_pll_config_t pll0Config = { .enableMode = 0U, .prdiv = 0x1U,
			.vdiv = 0x0U, };

	SMC_SetPowerModeRun(SMC);

	CLOCK_SetSimSafeDivs();

	/* Currently in BLPI mode, will switch to PEE mode. */
	/* Enter FBI. */
	CLOCK_SetLowPowerEnable(false);
	/* Enter FBE. */
	CLOCK_SetFbeMode(3U, kMCG_Dmx32Default, kMCG_DrsLow, NULL);
	/* Enter PBE. */
	CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &pll0Config);
	/* Enter PEE. */
	CLOCK_SetPeeMode();

	CLOCK_SetSimConfig(&simConfig);
}

energia_modo_enum energia_GetPowerMode(void) {
	smc_power_state_t currentPowerState = SMC_GetPowerModeState(SMC);

	switch (currentPowerState) {
	case kSMC_PowerStateRun:
        	PRINTF("    Power mode: RUN\r\n");
		return ENERGIA_MODO_RUN;
		break;
	case kSMC_PowerStateVlpr:
       	PRINTF("    Power mode: VLPR\r\n");
		return ENERGIA_MODO_VLPR;
		break;
	default:
        	PRINTF("    Power mode wrong\r\n");
		return ENERGIA_MODO_ERROR;
		break;
	}
}

uint32_t energia_GetClock(void) {
	return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}

void energia_init(void) {
	/* Init board hardware. */
//	BOARD_InitBootClocks();

	// Se habilita el poder usar todos los modos de consumo.
	SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

	return;
}
