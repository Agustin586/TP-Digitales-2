/*
 * sdcard.c
 *
 *  Created on: 29 jun. 2024
 *      Author: aguat
 */

/*< Archivos >*/
#include "Include/sdcard.h"
#include "Include/spiRtos.h"
#include "fatfs/fatfs_include/ff.h"
#include "fsl_debug_console.h"

sdspi_card_t g_card;

static void MountFileSystem(void);

extern void sd_init(void) {
//	if (SDSPI_Init(&g_card)) {
//		PRINTF(
//				"Fallo en la inicialización de la tarjeta SD a través de SPI.\r\n");
//		return;
//	}
//
//	MountFileSystem();
//
//	PRINTF("Tarjeta SD inicializada correctamente a través de SPI.\r\n");
}

FATFS fileSystem;

//const TCHAR driverNumberBuffer[3U] = { '0', ':', '/' };

static void MountFileSystem(void) {
//	if (f_mount(&fileSystem, "", 0)) {
//		PRINTF("Fallo en el montaje del sistema de archivos.\r\n");
//		return;
//	}
//
//	PRINTF("Sistema de archivos montado correctamente.\r\n");
}

extern void sd_write(File_t file) {
	UINT bytesWritten;

//	if (f_open(&file.file_, _T(file.nameFile), FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
//		f_write(&file.file_, file.buffer, sizeof(file.buffer), &bytesWritten);
//		f_close(&file.file_);
//		PRINTF("Escritura completa\r\n");
//	}

	return;
}

void WriteReadSDCard(void) {
//	FIL file;
//	UINT bytesWritten;
//	UINT bytesRead;
//	char writeBuffer[] = "Hola, mundo!";
//	char readBuffer[32];
//
//	// Escritura en la tarjeta SD
//	if (f_open(&file, _T("test.txt"), FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
//		f_write(&file, writeBuffer, sizeof(writeBuffer), &bytesWritten);
//		f_close(&file);
//		PRINTF("Escritura completa: %s\r\n", writeBuffer);
//	}

//	// Lectura desde la tarjeta SD
//	if (f_open(&file, _T("test.txt"), FA_READ) == FR_OK) {
//		f_read(&file, readBuffer, sizeof(readBuffer), &bytesRead);
//		f_close(&file);
//		PRINTF("Lectura completa: %s\r\n", readBuffer);
//	}
}

