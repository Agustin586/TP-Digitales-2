/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, 2018, Gustavo Muro
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

/* !!!!!!!!!!!! FALTA (01/06/2024) !!!!!!!!!!!!!
 *
 *  - Buscar mejor mecanismo para activar/desactivar el radar.
 *
 *  - Detectar el angulo del servo
 *
 *  - En que parte conviene inicializar el modulo de UART con ringbuffer
 *
 */


/*==================[inclusions]=============================================*/
#include "stdio.h"
#include <UART0.h>
#include "string.h"
#include "procTrama.h"
#include "SD2_board.h"
#include "UART0.h"
#include "mefRecTrama.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

static uint8_t auxBuf[11]={0,0,0,0,0,
						   0,0,0,0,0,0};

static uint32_t distancia = 0, angulo = 0;

static uint8_t salto_linea[2] = {'\n','\r'};

static char numtochar(uint8_t num){
	if(num<10) return (num+48);
	else return '#';
}

void procTrama(char *buf, int length)
{
    // Mensaje: Accion sobre el Led Rojo
    if(buf[2] == '0' && buf[3] == '1'){
	    switch (buf[4]){
        	case 'E':
            	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_ON);
            	break;
        	case 'A':
            	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_OFF);
            	break;
        	case 'T':
            	board_setLed(BOARD_LED_ID_ROJO, BOARD_LED_MSG_TOGGLE);
            	break;
    	    }
    	    
    	    // Retransmitir el mismo mensaje recibido.

	    	uart0_envByte(':');
	    	uart0_envDatos(buf, 5);
	    	uart0_envDatos(salto_linea, 2);
     }
     
     // Mensaje: Leer estado de SW1
     else if(buf[2] == '1' && buf[3] == '1'){
	    if(board_getSw(BOARD_SW_ID_1)){
	    
	    	// Transmitir el mensaje :XX11P’LF’ (las XX deben ser iguales a las recibidas).
	    	strcpy(auxBuf, ":XX11P");
	    	auxBuf[1] = buf[0];
	    	auxBuf[2] = buf[1];

	    	uart0_envDatos(auxBuf, 6);
	    	uart0_envDatos(salto_linea, 2);
	    }
	    
	    else{
	    	// Transmitir el mensaje :XX11N’LF’ (las XX deben ser iguales a las recibidas).
	    	strcpy(auxBuf, ":XX11N");
	        auxBuf[1] = buf[0];
	    	auxBuf[2] = buf[1];

	    	uart0_envDatos(auxBuf, 6);
	    	uart0_envDatos(salto_linea, 2);
	    }
     }
	
     // Mensaje: Accion sobre el radar
     else if(buf[2] == '0' && buf[3] == '2'){
	    switch (buf[4]){
        	case 'E':
            	
            	// Encender radar.
        		uart0_envByte('E');
				uart0_envDatos(salto_linea, 2);

            	break;
        	case 'A':
        	
            	// Apagar radar.
        		uart0_envByte('A');
        		uart0_envDatos(salto_linea, 2);
            	
            	break;

    	    }
    	    
    	    // Retransmitir el mismo mensaje recibido.
	    	uart0_envByte(':');
	        uart0_envDatos(buf, 5);
	        uart0_envDatos(salto_linea, 2);
     }
     
    //Mensaje: Transmitir ultimos valores de angulo en grados (GGG) y distancia en mm (DDD).
     else if(buf[2] == '2' && buf[3] == '1'){

    	// distancia = (uint32_t)HCSR04_getDistance();

    	// AGREGAR INFO ANGULO /////////////////////////////////////////////////////////////
	   
    	// angulo = (uint32_t)MG90S_getAngle();

    	////////////////////////////////////////////////////////////////////////////////////

	    // Transmitir los bytes :XX21GGGDDD’LF’ (las XX deben ser iguales a las recibidas).
    	/*
    	 strcpy(auxBuf,":XX21");
         auxBuf[1] = buf[0];
    	 auxBuf[2] = buf[1];
    	 auxBuf[5] = numtochar(  angulo/100 );
    	 auxBuf[6] = numtochar(  angulo/10 - auxBuf[5]*10 );
    	 auxBuf[7] = numtochar(  angulo - (angulo/10)*10  );
    	 auxBuf[8] = numtochar(  distancia/100 );
    	 auxBuf[9] = numtochar(  distancia/10 - auxBuf[8]*10 );
    	 auxBuf[10] = numtochar(  distancia - (distancia/10)*10  );
    	 auxBuf[11] = 0x0D;

    	 uart0_envDatos(auxBuf, 11);
    	 uart0_envDatos(salto_linea, 2);
    	*/

    	 uart0_envByte('#');
    	 uart0_envDatos(salto_linea, 2);
     }

}

/*==================[end of file]============================================*/


