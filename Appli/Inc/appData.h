/*
 * appData.h
 *
 *  Created on: 26 mai 2021
 *      Author: antoine
 */

#ifndef APPDATA_H_
#define APPDATA_H_

#include <stdint.h>

class appData {
public:
	appData();
	virtual ~appData();

	//dataSave
	uint8_t numAdcCH [2] = {2,2};
	uint8_t adcAnbSeries = 0;
	uint8_t adcBnbSeries = 0;

	uint32_t adcAPeriod = 4800;
	uint8_t adcADiv = 0;
	uint16_t adcBPeriod = 4800;
	uint8_t adcBDiv = 0;

	uint8_t AV0 = 1;
	uint8_t Amp0 = 0;
	uint8_t Amp1 = 0;

	uint8_t AV2 = 1;
	uint8_t Amp2 = 0;
	uint8_t Amp3 = 0;

	uint16_t nullVal = 0;
};

#endif /* APPDATA_H_ */
