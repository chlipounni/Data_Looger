/*
 * controller.h
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>
#include "cardSDRW.h"
#include "appData.h"
#include "usb_device.h"

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
	uint16_t RX_message [4];
	uint16_t TX_message [5];
	uint16_t DataValue	[];

};

#endif /* CONTROLLER_H */
