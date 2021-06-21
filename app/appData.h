#ifndef APPDATA_H_
#define APPDATA_H_

#include <stdint.h>

#define adc1Size 2
#define adc2Size 2

class appData {
public:
	appData();
	virtual ~appData();

	//dataSave
	uint8_t adc1CH[adc1Size];
	uint8_t adc3CH[adc2Size];

	uint16_t tm1Precale;
	uint16_t tm2Precale;

	uint8_t Amp0;
	uint8_t Amp1;
	uint8_t Amp2;
	uint8_t Amp3;
	uint8_t AV0;
	uint8_t AV2;

	//data note save
	uint16_t levelBatt;
	uint8_t numRankADC1;
	uint8_t numRankADC3;
};

#endif /* APPDATA_H_ */

