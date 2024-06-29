#ifndef _ENERGIA_H
#define _ENERGIA_H

#include <stdio.h>
#include "board.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "fsl_smc.h"

#include "SD2_board.h"

typedef enum{
	ENERGIA_MODO_RUN = 0,
	ENERGIA_MODO_VLPR,
	ENERGIA_MODO_ERROR
} energia_modo_enum;


void energia_init(void);

uint32_t energia_GetClock(void);
energia_modo_enum energia_GetPowerMode(void);

extern void energia_SetClockRunFromVlpr(void);
extern void energia_SetClockVlpr(void);

#endif
