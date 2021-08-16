/*
 * cardSDRW.h
 *
 *  Created on: 26 mai 2021
 *      Author: antoine
 */

#ifndef PROG_CARDSDRW_H_
#define PROG_CARDSDRW_H_

#include <stdint.h>
#include <stdio.h>
#include "fatfs.h"
#include <vector>
#include "appData.h"

class cardSDRW {
public:
	cardSDRW(appData* pointData);
	virtual ~cardSDRW();

	//data
	void openData();
	void saveData(uint8_t file, uint16_t size, uint16_t* data);
	void loadData(uint8_t file,uint32_t pos, uint16_t size);
	uint32_t sizeData(uint8_t adc, uint8_t vers, uint8_t div);
	FRESULT lifeData(uint8_t adc, uint8_t vers,uint8_t div);
	uint16_t freeSize();
	void sync(uint8_t file);
	void deleteData(uint8_t nbVersADCA, uint8_t nbVersADCB);
	void closeData();

	void openDataFile(uint8_t adc, uint8_t vers, uint8_t div);
	void closeDataFile();
	void loadDataFile(uint32_t pos, uint16_t size);

	//param
	void openParam();
	void saveParam(uint16_t pos, uint8_t* data, uint16_t size);
	void loadParam(uint16_t pos, uint16_t size);
	uint16_t sizeParam();
	void closeParam();

	UINT byteswritten, bytesread; /* File write/read counts */
	uint8_t rtext[_MAX_SS];/* File read buffer */

	//param
	FIL* paramFile;

	//data_Value (0-1) tempFile (2)
	FIL* SDFile[3];

	appData* data;
};

#endif /* PROG_CARDSDRW_H_ */
