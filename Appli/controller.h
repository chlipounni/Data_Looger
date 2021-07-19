/*
 * controller.h
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdint.h>
#include "cardSDRW.h"
#include "appData.h"

#define ADC_BUFFER_SIZE 4000
#define div1 100
#define boucle 10

class Controller{
public:
	Controller(Controller &other) = delete;
	void operator=(const Controller &) = delete;
	static Controller* GetInstance();
	void init();

	//reception
	void dataRecept();

	// Data
	void saveData(uint8_t adc,uint8_t full);
	void startData();
	void stopData();

	//niv
	void loadNiv();
	void nivSDcard();
	void nivBatt();


	uint8_t RX_buffer [10];
	uint8_t TX_buffer [10];

	uint16_t adc1Buffer[ADC_BUFFER_SIZE] __attribute__ ((aligned (8)));
	uint16_t adc3Buffer[ADC_BUFFER_SIZE] __attribute__ ((aligned (8)));

protected:
	static Controller* controller_;
	Controller();

private:

	// Param
	void loadParam();
	void appliParam();
	void saveAllParam();
	void saveParam(uint16_t pos, uint32_t* data);

	cardSDRW* sdCard;
	appData* data;

	uint16_t value[60];
	uint32_t midAdd [2] ={0,0};
	uint8_t offset;

	void divMidMinMax(uint8_t adc,uint8_t divVer,uint8_t div, uint16_t* val);
	void messageOK();
	void messageNOK();

};

#endif /* CONTROLLER_H_ */
