/*
 * appData.h
 *
 *  Created on: 26 mai 2021
 *      Author: antoine
 */

#ifndef APPDATA_H_
#define APPDATA_H_

class appData {
public:
	appData();
	virtual ~appData();

//dataSave
	uint8_t adc1CH[13];

	uint16_t tm1Precale;
	uint16_t tm2Precale;

	uint8_t ch1VA;
	uint8_t ch1Amp;
	uint8_t ch2VA;
	uint8_t ch2Amp;
	uint8_t ch3VA;
	uint8_t ch3Amp;
	uint8_t ch4VA;
	uint8_t ch4Amp;

//data note save
	uint16_t levelBatt;
	uint8_t numRank;
};

#endif /* APPDATA_H_ */
