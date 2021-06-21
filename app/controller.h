/*
 * controller.h
 *
 */


#ifndef PROG_CONTROLLER_H_
#define PROG_CONTROLLER_H_

#include <stdint.h>
#include "cardSDRW.h"
#include "appData.h"

#define Amp_0_GPIO PB6_GPIO
#define Amp_0 PB6
#define AV_0_GPIO PB8_GPIO
#define AV_0 PB8
#define Amp_1_GPIO PE0_GPIO
#define Amp_1 PE0
#define AV_2_GPIO PE9_GPIO
#define AV_2 PE9
#define Amp_2_GPIO PE11_GPIO
#define Amp_2 PE11
#define Amp_3_GPIO PE7_GPIO
#define Amp_3 PE7



class controller {
public:
	controller();
	virtual ~controller();
	
// Data
	void saveData(uint8_t adc, uint8_t half);

	uint8_t recept;
private:

	// Param 
	void loadParam();
	void appliParam();
	void saveParam();
	
	// calcul value
	uint16_t maxValue(uint8_t ch, uint8_t vers, uint16_t pos, uint16_t size);
	uint16_t minValue(uint8_t ch, uint8_t vers,uint16_t pos, uint16_t size);
	uint16_t midValue(uint8_t ch, uint8_t vers,uint16_t pos, uint16_t size);
	//
	void nivBatt ();
	void selectTM(uint8_t adc, uint8_t ch,uint8_t use);
	void paramCH(uint8_t ch, uint8_t va, uint8_t amp);
	void paramTM(uint8_t tm, uint32_t prescale);

	//reception
	void dataRecept();

	void startM();
	void stopM();

	cardSDRW sdCard;
	appData data;
	uint16_t RX_message[4];
	uint16_t TX_message [5];
	uint16_t DataValue[];

};

#endif /* PROG_CONTROLLER_H_ */
