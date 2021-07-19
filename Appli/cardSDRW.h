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

#define nbDiv 3

class cardSDRW {
public:
	cardSDRW();
	virtual ~cardSDRW();

	//data
	void openData();
	void saveData(uint8_t file, uint16_t size, uint16_t* data);
	void loadData(uint8_t file,uint32_t pos, uint16_t size);
	uint32_t sizeData(uint8_t adc, uint8_t vers, uint8_t div);
	void sync(uint8_t file);
	void deleteData();
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

	FRESULT res; /* FatFs function common result code */
	UINT byteswritten, bytesread; /* File write/read counts */
	uint8_t rtext[_MAX_SS];/* File read buffer */

	//param
	FIL* paramFile;

	//data_Value (0-5) tempFile (6)
	FIL* SDFile[7];

	appData* data;
};

#endif /* PROG_CARDSDRW_H_ */
