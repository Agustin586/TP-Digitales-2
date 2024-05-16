/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
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
#include "mma8451.h"
#include "math.h"
#include "SD2_I2C.h"

/*==================[macros and definitions]=================================*/
#define MMA8451_I2C_ADDRESS     (0x1d)

#define INT1_PORT       PORTC
#define INT1_GPIO       GPIOC
#define INT1_PIN        5

#define INT2_PORT       PORTD
#define INT2_GPIO       GPIOD
#define INT2_PIN        1

#define G				9.8
#define t_s(d)			sqrt(((2*d)/100.0)/G)
#define THS_cm(d)		((d/100.0)*2)/(G*t_s(d)*t_s(d))
#define THS_CM_TO_COUNT	0.063

typedef union
{
    struct
    {
        unsigned SRC_DRDY:1;
        unsigned :1;
        unsigned SRC_FF_MT:1;
        unsigned SRC_PULSE:1;
        unsigned SRC_LNDPRT:1;
        unsigned SRC_TRANS:1;
        unsigned SRC_FIFO:1;
        unsigned SRC_ASLP:1;
    };
    uint8_t data;
}INT_SOURCE_t;

typedef union
{
    struct
    {
        unsigned XDR:1;
        unsigned YDR:1;
        unsigned ZDR:1;
        unsigned ZYXDR:1;
        unsigned XOW:1;
        unsigned YOW:1;
        unsigned ZOW:1;
        unsigned ZYXOW:1;
    };
    uint8_t data;
}STATUS_t;

typedef union
{
    struct
    {
        unsigned ACTIVE:1;
        unsigned F_READ:1;
        unsigned LNOISE:1;
        unsigned DR:3;
        unsigned ASLP_RATE:2;
    };
    uint8_t data;
}CTRL_REG1_t;

#define CTRL_REG1_ADDRESS   0X2A

/* REGISTRO 4 */
/////////////////////////////////////////
typedef union
{
    struct
    {
        unsigned INT_EN_DRDY:1;
        unsigned :1;
        unsigned INT_EN_FF_MT:1;
        unsigned INT_EN_PULSE:1;
        unsigned INT_EN_LNDPRT:1;
        unsigned INT_EN_TRANS:1;
        unsigned INT_EN_FIFO:1;
        unsigned INT_EN_ASLP:1;
    };
    uint8_t data;
}CTRL_REG4_t;

#define CTRL_REG4_ADDRESS   0X2D
/////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned INT_CFG_DRDY:1;
        unsigned :1;
        unsigned INT_CFG_FF_MT:1;
        unsigned INT_CFG_PULSE:1;
        unsigned INT_CFG_LNDPRT:1;
        unsigned INT_CFG_TRANS:1;
        unsigned INT_CFG_FIFO:1;
        unsigned INT_CFG_ASLP:1;
    };
    uint8_t data;
}CTRL_REG5_t;

#define CTRL_REG5_ADDRESS   0X2E

/* FF/MT THRESHOLD */
/////////////////////////////////////////
typedef union
{
    struct
    {
        unsigned THRESHOLD:7;	// Threshold
        unsigned DBCNTM:1; 		// Detecta ruido por rebote
    };
    uint8_t data;
}FF_MT_THS_t;

#define FF_MT_THS_ADDRESS 0x17
/////////////////////////////////////////

/* FF/MT CONFIG */
/////////////////////////////////////////
typedef union
{
    struct
    {
    	unsigned :3;
    	unsigned XEFE:1;
    	unsigned YEFE:1;
    	unsigned ZEFE:1;
    	unsigned OAE:1;
        unsigned ELE:1;
    };
    uint8_t data;
}FF_MT_CFG_t;

#define FF_MT_CFG_ADDRESS 0x15
/////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned D:8;
    };
    uint8_t data;
}FF_MT_COUNT_t;

#define FF_MT_COUNT_ADDRESS 0x18

typedef union
{
    struct
    {
    	unsigned :7;
        unsigned EVENT_ACTIVE:1;
    };
    uint8_t data;
}FF_MT_SRC_t; //solo lectura

#define FF_MT_SRC_ADDRESS 0x16

#define INT_SOURCE_ADDRESS   0X0C
#define STATUS_ADDRESS       0X00

#define G_THS		0.7
#define THS_MAX_FF 	G_THS*100
#define THS_MAX_FF_CUADRADO	THS_MAX_FF*THS_MAX_FF
#define THS_REF_RANGO_2G_CUADRADO	200*200

volatile static int16_t readX, readY, readZ;
volatile static uint8_t ff_flag=0,dryd_flag=0;
static int32_t ValNorma_Max=0,ReadNorma=0;
static int16_t MaxX=0,MaxY=0,MaxZ=0;

static uint8_t mma8451_read_reg(uint8_t addr)
{
	i2c_master_transfer_t masterXfer;
    uint8_t ret;

	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &ret;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	return ret;
}

static void mma8451_write_reg(uint8_t addr, uint8_t data)
{
	i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &data;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C0, &masterXfer);
}

static void config_port_int1(void)
{
	const port_pin_config_t port_int1_config = {
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
	const gpio_pin_config_t gpio_int1_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
	};

	PORT_SetPinConfig(INT1_PORT, INT1_PIN, &port_int1_config);
	GPIO_PinInit(INT1_GPIO, INT1_PIN, &gpio_int1_config);

	/* Interrupt polarity active high, or active low. Default value: 0.
	   0: Active low; 1: Active high. VER REGISTRO CTRL_REG3 */
	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptLogicZero);

	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
}

static void config_port_int2(void)
{
	const port_pin_config_t port_int2_config = {
			/* Internal pull-up/down resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Slow slew rate is configured */
		.slewRate = kPORT_SlowSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as PTD1 */
		.mux = kPORT_MuxAsGpio,
	};
	const gpio_pin_config_t gpio_int2_config = {
		.pinDirection = kGPIO_DigitalInput,
		.outputLogic = 0U
	};

	PORT_SetPinConfig(INT2_PORT, INT2_PIN, &port_int2_config);
	GPIO_PinInit(INT2_GPIO, INT2_PIN, &gpio_int2_config);

	/* Interrupt polarity active high, or active low. Default value: 0.
	   0: Active low; 1: Active high. VER REGISTRO CTRL_REG3 */
	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptLogicZero);

	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
}

void mma8451_activar(void)
{
	CTRL_REG1_t ctrl_reg1;

	ctrl_reg1.data = mma8451_read_reg(CTRL_REG1_ADDRESS);

	ctrl_reg1.data |= 0b00000001;

	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	return;
}

void mma8451_desactivar(void)
{
	CTRL_REG1_t ctrl_reg1;

	ctrl_reg1.data = mma8451_read_reg(CTRL_REG1_ADDRESS);

	ctrl_reg1.data &= 0b11111110;

	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);
}

static void mma8451_read_mult_reg(uint8_t addr, uint8_t *pBuf, size_t size){
	i2c_master_transfer_t masterXfer;

	memset(&masterXfer, 0, sizeof(masterXfer));
	masterXfer.slaveAddress = MMA8451_I2C_ADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = addr;
	masterXfer.subaddressSize = 1;
	masterXfer.data = pBuf;
	masterXfer.dataSize = size;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);
}

void mma8451_init(void)
{
	/* CONFIG I2C */
	SD2_I2C_init();

    /* CONFIG DRDY Y FREEFALL */
    mma8451_DRDYinit ();
    mma8451_FFinit ();

    /* HABILITACIÓN DEL ACELEROMETRO */
	mma8451_setDataRate(DR_100hz);
	mma8451_activar();

	/* LEE FREEFALL ANTERIORES POR RESETEO */
	mma8451_IntFF();

    /* CONFIG GPIOS */
    config_port_int1();
    config_port_int2();

    return;
}

void mma8451_setDataRate(DR_enum rate)
{
    CTRL_REG1_t ctr_reg1;
    bool estAct;

    /* Antes de modificar data rate es necesario poner ACTIVE = 0 */
    ctr_reg1.data = mma8451_read_reg(CTRL_REG1_ADDRESS);

    /* Guarda valor que tiene ACTIVE y luego pone a cero */
    estAct = ctr_reg1.ACTIVE;
    ctr_reg1.ACTIVE = 0;

	mma8451_write_reg(CTRL_REG1_ADDRESS, ctr_reg1.data);

	/* actualiza DR y en la misma escritura va a restaurar ACTIVE */
	ctr_reg1.DR = rate;
	ctr_reg1.ACTIVE = estAct;

	mma8451_write_reg(CTRL_REG1_ADDRESS, ctr_reg1.data);

	/* verificación */
	ctr_reg1.data = mma8451_read_reg(0x2a);
}

int16_t mma8451_getAcX(void)
{
	return (int16_t)(((int32_t)readX * 100) / (int32_t)4096);
}

int16_t mma8451_getAcY(void)
{
	return (int16_t)(((int32_t)readY * 100) / (int32_t)4096);
}

int16_t mma8451_getAcZ(void)
{
	return (int16_t)(((int32_t)readZ * 100) / (int32_t)4096);
}

float mma8451_norma(void)
{
	float X,Y,Z;

	X = mma8451_getAcX();
	Y = mma8451_getAcY();
	Z = mma8451_getAcZ();

	PRINTF("eje X = %d\neje Y = %d\neje Z = %d\n",mma8451_getAcX(),mma8451_getAcY(),mma8451_getAcZ());
	PRINTF("Norma: %f\n",sqrt((X*X) + (Y*Y) + (Z*Z)));
	return sqrt( (X*X) + (Y*Y) + (Z*Z) );
}

uint32_t mma8451_norma_cuadrado(void)
{
	int32_t X,Y,Z;

	X = mma8451_getAcX();
	Y = mma8451_getAcY();
	Z = mma8451_getAcZ();

	return (uint32_t) (X*X) + (Y*Y) + (Z*Z); // (es más rapido de computar y permite maximizar la aceleracion igualmente)
}

extern uint32_t mma8451_getNormMax_cuad(void){
	return ValNorma_Max;
}

extern void mma8451_clrNormMax(void){
	ValNorma_Max = 0;
	ReadNorma = 0;

	MaxX = 0;
	MaxY = 0;
	MaxZ = 0;

	return;
}

/* FUNCIONES DE FREEFALL */
///////////////////////////

/*!
 * @brief Configura el freefall y la interrupcion por freefall
 *
 * @param void
 *
 * @return void
 * */
void mma8451_FFinit(void) {
	/* Configuración de interrupción */
	/*================================================================================*/
	CTRL_REG1_t ctrl_reg1;
	CTRL_REG4_t ctrl_reg4;
	CTRL_REG5_t ctrl_reg5;

	ctrl_reg1.ACTIVE = 0;
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	/* REGISTRO 4 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg4.INT_EN_DRDY   = 0;	// Interrupción por drdy
	ctrl_reg4.INT_EN_FF_MT  = 1; 	// Interrupción por freefall
	ctrl_reg4.INT_EN_PULSE  = 0;
	ctrl_reg4.INT_EN_LNDPRT = 0;
	ctrl_reg4.INT_EN_TRANS  = 0;
	ctrl_reg4.INT_EN_FIFO   = 0;
	ctrl_reg4.INT_EN_ASLP   = 0;

	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);
	ctrl_reg4.data = mma8451_read_reg(CTRL_REG4_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	/* REGISTRO 5 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg5.INT_CFG_DRDY   = 1;
	ctrl_reg5.INT_CFG_FF_MT  = 0; // se deja en 0 para conectarlo al INT2.
	ctrl_reg5.INT_CFG_PULSE  = 0;
	ctrl_reg5.INT_CFG_LNDPRT = 0;
	ctrl_reg5.INT_CFG_TRANS  = 0;
	ctrl_reg5.INT_CFG_FIFO   = 0;
	ctrl_reg5.INT_CFG_ASLP   = 0;

	mma8451_write_reg(CTRL_REG5_ADDRESS, ctrl_reg5.data);
	ctrl_reg5.data = mma8451_read_reg(CTRL_REG5_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	/*================================================================================*/

	/* CONFIGURACIONES INICIALES PARA TRABAJAR EN FREEFALL */
	/*================================================================================*/
	FF_MT_THS_t ff_mt_ths;
	FF_MT_CFG_t ff_mt_cfg;
	FF_MT_COUNT_t ff_mt_count;

	/* FF/MT CONFIG */
	////////////////////////////////////////////////////////////////////////////////////
    ff_mt_cfg.ELE = 1;		// Event flag latch enable
    ff_mt_cfg.OAE = 0; 		// Freefall flag
    ff_mt_cfg.ZEFE = 1;		// Eje z
    ff_mt_cfg.YEFE = 1;		// Eje y
    ff_mt_cfg.XEFE = 1;		// Eje x

    mma8451_write_reg(FF_MT_CFG_ADDRESS, ff_mt_cfg.data);
	////////////////////////////////////////////////////////////////////////////////////

    /* FF/MT THRESHOLD */
	////////////////////////////////////////////////////////////////////////////////////
    ff_mt_ths.DBCNTM = 1;				// Resetea la cuenta si sale del freefall
//	ff_mt_ths.THRESHOLD = 0B0000011;	// 0x03 --> 3 cuentas (0.063*3 --> 0.2g)
//    ff_mt_ths.THRESHOLD = (uint8_t) (THS_cm(10)/THS_CM_TO_COUNT);	// 25 cm
    ff_mt_ths.THRESHOLD = 3;

	mma8451_write_reg(FF_MT_THS_ADDRESS, ff_mt_ths.data);
	////////////////////////////////////////////////////////////////////////////////////

	/* FF/MT DEBOUNCE COUNTER */
	////////////////////////////////////////////////////////////////////////////////////
	ff_mt_count.D = 0X0A;		// 10 cuentas antes de la interrupción

	mma8451_write_reg(FF_MT_COUNT_ADDRESS, ff_mt_count.data);
	////////////////////////////////////////////////////////////////////////////////////

	/* REGISTRO 1 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg1.ACTIVE = 0; // Aca se pasa al MMA a modo activo.
	ctrl_reg1.F_READ = 0;
	ctrl_reg1.LNOISE = 0;
	ctrl_reg1.DR = 0B011; // configurado un data rate de 100Hz.
	ctrl_reg1.ASLP_RATE = 0B00;

	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);
	ctrl_reg1.data = mma8451_read_reg(CTRL_REG1_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	/*================================================================================*/

	return;
}

/*!
 * @brief Resetea la bandera de flag del freefall
 *
 * @param void
 *
 * @return void
 * */
void mma8451_resetFFflag(void){
	ff_flag = 0;

	return;
}

/*!
 * @brief Devuelve el valor de la flag
 *
 * @param void
 *
 * @return uint8_t
 * 		Valor de la flag.
 * */
uint8_t mma8451_getFFflag(void){
	return ff_flag;
}

extern void mma8451_IntFF(void){
	INT_SOURCE_t intSource;

	intSource.data = mma8451_read_reg(INT_SOURCE_ADDRESS);
	mma8451_read_reg(FF_MT_SRC_ADDRESS);	// Limpia la flag del mma8451

	return;
}

///////////////////////////

/* FUNCIONES DE DATO LISTO */
///////////////////////////

/*!
 * @brief Inicializa los datos listos para el acelerometro
 *
 * @param void
 *
 * @return void
 * */
void mma8451_DRDYinit(void) {
	CTRL_REG1_t ctrl_reg1;
	CTRL_REG4_t ctrl_reg4;
	CTRL_REG5_t ctrl_reg5;

	ctrl_reg1.ACTIVE = 0;
	mma8451_write_reg(CTRL_REG1_ADDRESS, ctrl_reg1.data);

	/* REGISTRO 4 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg4.INT_EN_DRDY   = 0;	//Espera interrupcion por data ready
	ctrl_reg4.INT_EN_FF_MT  = 0;
	ctrl_reg4.INT_EN_PULSE  = 0;
	ctrl_reg4.INT_EN_LNDPRT = 0;
	ctrl_reg4.INT_EN_TRANS  = 0;
	ctrl_reg4.INT_EN_FIFO   = 0;
	ctrl_reg4.INT_EN_ASLP   = 0;

	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);
	ctrl_reg4.data = mma8451_read_reg(CTRL_REG4_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	/* REGISTRO 5 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg5.INT_CFG_DRDY   = 1;	//Enruta la interrupcion por INT1
	ctrl_reg5.INT_CFG_FF_MT  = 0;
	ctrl_reg5.INT_CFG_PULSE  = 0;
	ctrl_reg5.INT_CFG_LNDPRT = 0;
	ctrl_reg5.INT_CFG_TRANS  = 0;
	ctrl_reg5.INT_CFG_FIFO   = 0;
	ctrl_reg5.INT_CFG_ASLP   = 0;

	mma8451_write_reg(CTRL_REG5_ADDRESS, ctrl_reg5.data);
	ctrl_reg5.data = mma8451_read_reg(CTRL_REG5_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	return;
}

/*!
 * @brief Obtiene el estado de la bandera de interrupcion por dato listo
 *
 * @param void
 *
 * @return uint8_t
 * 		drdy_flag --> Estado de interrupcion
 * */
uint8_t mma8451_getDRYDflag(void){
	return dryd_flag;
}

/*!
 * @brief Resetea la bandera del flag de dato listo
 *
 * @param void
 *
 * @return void
 * */
void mma8451_resetDRYDflag(void){
	dryd_flag = 0;

	return;
}

/*!
 * @brief Habilita la interrupcion por dato listo desde el mma8451
 *
 * @param void
 *
 * @return void
 */
void mma8451_enableDRDYInt(void){
	CTRL_REG4_t ctrl_reg4;

	mma8451_desactivar();

	/* REGISTRO 4 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg4.INT_EN_DRDY = 1;	//Espera interrupcion por data ready
	ctrl_reg4.INT_EN_FF_MT = 1;
	ctrl_reg4.INT_EN_PULSE = 0;
	ctrl_reg4.INT_EN_LNDPRT = 0;
	ctrl_reg4.INT_EN_TRANS = 0;
	ctrl_reg4.INT_EN_FIFO = 0;
	ctrl_reg4.INT_EN_ASLP = 0;

	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);
	ctrl_reg4.data = mma8451_read_reg(CTRL_REG4_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	mma8451_activar();

	return;
}

/*!
 * @brief Deshabilita la interrupcion por dato listo desde el mma8451.
 *
 * @param void
 *
 * @return void
 * */
void mma8451_disableDRDYInt(void){
	CTRL_REG4_t ctrl_reg4;

	mma8451_desactivar();

	/* REGISTRO 4 */
	////////////////////////////////////////////////////////////////////////////////////
	ctrl_reg4.INT_EN_DRDY = 0;	//Espera interrupcion por data ready
	ctrl_reg4.INT_EN_FF_MT = 1;
	ctrl_reg4.INT_EN_PULSE = 0;
	ctrl_reg4.INT_EN_LNDPRT = 0;
	ctrl_reg4.INT_EN_TRANS = 0;
	ctrl_reg4.INT_EN_FIFO = 0;
	ctrl_reg4.INT_EN_ASLP = 0;

	mma8451_write_reg(CTRL_REG4_ADDRESS, ctrl_reg4.data);
	ctrl_reg4.data = mma8451_read_reg(CTRL_REG4_ADDRESS);
	////////////////////////////////////////////////////////////////////////////////////

	mma8451_activar();

	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptOrDMADisabled);	// Deshabilita la interrupcion

	return;
}

/*!
 * @brief Se encarga de procesar los datos enviados de los ejes desde el acelerometro
 * mediante la comunicacion i2c.
 *
 * @param void
 *
 * @return void
 * */
extern void mma8451_IntDRYD(void){
	int16_t readG;

#define BUF_LENGTH 7

	static uint8_t bufTemp[BUF_LENGTH];

	mma8451_read_mult_reg(0, bufTemp, BUF_LENGTH);

	readG = (int16_t) bufTemp[1] << 8 | bufTemp[2];
	readX = readG >> 2;

	readG = (int16_t) bufTemp[3] << 8 | bufTemp[4];
	readY = readG >> 2;

	readG = (int16_t) bufTemp[5] << 8 | bufTemp[6];
	readZ = readG >> 2;

	return;
}

extern void mma8451_HabilitarInt1(void){
	/* HABILITAMOS LA INTERRUPCION 1 */
	PORT_SetPinInterruptConfig(INT1_PORT, INT1_PIN, kPORT_InterruptLogicZero);	// Interrupcion 1

	return;
}

extern void mma8451_HabilitarInt2(void){
	/* HABILITAMOS LA INTERRUPCION 2 */
	PORT_SetPinInterruptConfig(INT2_PORT, INT2_PIN, kPORT_InterruptLogicZero);	// Interrupcion 2

	return;
}

/*==================[end of file]============================================*/
