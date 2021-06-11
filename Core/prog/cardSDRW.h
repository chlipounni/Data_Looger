/*
 * cardSDRW.h
 *
 *  Created on: 26 mai 2021
 *      Author: antoine
 */

#ifndef PROG_CARDSDRW_H_
#define PROG_CARDSDRW_H_

class cardSDRW {
public:
	cardSDRW();
	virtual ~cardSDRW();

	void saveParam(uint16_T pos,uint16_t data);
	void saveData(uint8_t ch,uint16_t data);
	void loadParam(uint8_t ch,uint32_T pos, uintr32_t size);
	void loadData(uint8_t ch,uint32_T pos, uintr32_t size);
	uint16_t sizeData(uint8_t ch,uint8_t vers);
	uint16_t sizeVersion(uint8_t ch);
	void openParam(uint8_t ch);
	void openCH(uint8_t ch);
	void openCH(uint8_t ch,uint8_t vers);
	void updateName();
	void deleteData(uint8_t ch,uint8_t vers);

	uint16_t levelSD();

	FRESULT res; /* FatFs function common result code */
	uint32_t byteswritten, bytesread; /* File write/read counts */
	uint16_t rtext[_MAX_SS];/* File read buffer */
	char*[] nameFile [13];
};

#endif /* PROG_CARDSDRW_H_ */
