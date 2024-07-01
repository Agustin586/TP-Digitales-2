/*
 * sdcard.h
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

#ifndef INCLUDE_SDCARD_H_
#define INCLUDE_SDCARD_H_

#include "fsl_sdspi.h"
#include "fatfs/fatfs_include/ff.h"

typedef struct{
	FIL file_;
	char *buffer;
	char *nameFile;
}File_t;

extern void sd_init(void);
extern void sd_write(File_t file);

#endif /* INCLUDE_SDCARD_H_ */
