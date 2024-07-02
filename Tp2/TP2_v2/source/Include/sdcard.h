/*
 * sdcard.h
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_SDCARD_H_
#define INCLUDE_SDCARD_H_

/*< Archivos >*/
#include "fsl_sdspi.h"
#include "fatfs/fatfs_include/ff.h"

/*< Definiciones >*/
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

//#define CMD0	        (0x40+0)	                       /* GO_IDLE_STATE          */
//#define CMD1	        (0x40+1)	                       /* SEND_OP_COND           */
//#define CMD8	        (0x40+8)	                       /* SEND_IF_COND           */
//#define CMD9	        (0x40+9)	                       /* SEND_CSD               */
//#define CMD10	        (0x40+10)	                       /* SEND_CID               */
//#define CMD12	        (0x40+12)	                       /* STOP_TRANSMISSION      */
//#define CMD13	        (0xC0+13)	                       /* SD_STATUS              */
//#define CMD16	        (0x40+16)	                       /* SET_BLOCKLEN           */
//#define CMD17	        (0x40+17)	                       /* READ_SINGLE_BLOCK      */
//#define CMD18	        (0x40+18)	                       /* READ_MULTIPLE_BLOCK    */
//#define CMD23	        (0xC0+23)	                       /* SET_WR_BLK_ERASE_COUNT */
//#define CMD24	        (0x40+24)	                       /* WRITE_BLOCK            */
//#define CMD25	        (0x40+25)	                       /* WRITE_MULTIPLE_BLOCK   */
//#define CMD32	        (0x40+32)	                       /* ERASE_BLOCK_START_ADDR */
//#define CMD33	        (0x40+33)	                       /* ERASE_BLOCK_END_ADDR   */
//#define CMD38	        (0x40+38)	                       /* ERASE_SELECTED_BLOCKS  */
//#define CMD41	        (0xC0+41)	                       /* SEND_OP_COND           */
//#define CMD55	        (0x40+55)	                       /* APP_CMD                */
//#define CMD58	        (0x40+58)	                       /* READ_OCR               */

#define CMD0	        (0)	                       /* GO_IDLE_STATE          */
#define CMD1	        (1)	                       /* SEND_OP_COND           */
#define CMD8	        (8)	                       /* SEND_IF_COND           */
#define CMD9	        (9)	                       /* SEND_CSD               */
#define CMD10	        (10)	                       /* SEND_CID               */
#define CMD12	        (12)	                       /* STOP_TRANSMISSION      */
#define CMD13	        (13)	                       /* SD_STATUS              */
#define CMD16	        (16)	                       /* SET_BLOCKLEN           */
#define CMD17	        (17)	                       /* READ_SINGLE_BLOCK      */
#define CMD18	        (18)	                       /* READ_MULTIPLE_BLOCK    */
#define CMD23	        (23)	                       /* SET_WR_BLK_ERASE_COUNT */
#define CMD24	        (24)	                       /* WRITE_BLOCK            */
#define CMD25	        (25)	                       /* WRITE_MULTIPLE_BLOCK   */
#define CMD32	        (32)	                       /* ERASE_BLOCK_START_ADDR */
#define CMD33	        (33)	                       /* ERASE_BLOCK_END_ADDR   */
#define CMD38	        (38)	                       /* ERASE_SELECTED_BLOCKS  */
#define CMD41	        (41)	                       /* SEND_OP_COND           */
#define CMD55	        (55)	                       /* APP_CMD                */
#define CMD58	        (58)	                       /* READ_OCR               */

#define MMC_CS_SET()      (FGPIOD_PSOR |= 1<<4)
#define MMC_CS_CLR()      (FGPIOD_PCOR |= 1<<4)

/*< Variables >*/
typedef struct{
	FIL file_;
	char *buffer;
	char *nameFile;
}File_t;

/*< Funciones >*/
extern void sd_init(void);
extern void sd_write(File_t file);

#endif /* INCLUDE_SDCARD_H_ */
