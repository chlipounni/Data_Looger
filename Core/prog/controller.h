/*
 * controller.h
 *
 */

#include "cardSDRW.h"
#include "appData.h"

#ifndef PROG_CONTROLLER_H_
#define PROG_CONTROLLER_H_

#define enCH1_GPIO
#define enCH1
#define CH1VA_GPIO
#define CH1VA
#define enCH2_GPIO
#define enCH2
#define CH2VA_GPIO
#define CH2VA
#define enCH3_GPIO
#define enCH3
#define CH3VA_GPIO
#define CH3VA
#define enCH4_GPIO
#define enCH4
#define CH4VA_GPIO
#define CH4VA
#define CHA0_GPIO
#define CHA1_GPIO
#define CHA0
#define CHA1

class controller {
public:
	controller();
	virtual ~controller();
	
	void dataMain();

private:

	// Param 
	void loadParam();
	void appliParam();
	void saveParam();
	// Data
	void saveData();
	
	// calcul value
	uint16_t maxValue(uint8_t ch, uint8_t vers, uint16_t pos, uint16_t size);
	uint16_t minValue(uint8_t ch, uint8_t vers,uint16_t pos, uint16_t size);
	uint16_t midValue(uint8_t ch, uint8_t vers,uint16_t pos, uint16_t size);
	//
	void nivBatt ();
	void selectTM(uint8_t tm, uint8_t ch);
	void paramCH(uint8_t ch, uint8_t va, uint8_t amp);
	void paramTM(uint8_t tm, uint32_t prescale);

	//reception
	void dataRecept();
	uint8_t recept;
	uint8_t dataV;

	void startM();
	void stopM();

	cardSDRW sdCard;
	appData data;
	uint16_t RX_message[4];
	uint16_t TX_message [5];
	uint16_t DataValue[];

};

#endif /* PROG_CONTROLLER_H_ */
