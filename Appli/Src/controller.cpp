#include "../Inc/controller.h"
#include "usbd_cdc_if.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

Controller* Controller::controller_ = nullptr;

Controller::Controller()
{
	data = new appData();
	sdCard = new cardSDRW(data);
}

Controller* Controller::GetInstance()
{
	if(controller_ == nullptr) {
		controller_ = new Controller();
	}
	return controller_;
}

void Controller::init()
{
	loadParam();
	appliParam();
}

void Controller::loadParam()
{
	if(sdCard->sizeParam() == 18){
		sdCard->openParam();
		sdCard->loadParam( 0, 18);
		data->tm1Prescale = 0;
		for(uint8_t i = 0 ; i<4; i++){
			data->tm1Prescale += (uint32_t)sdCard->rtext[0+i]<<(8*i);
		}
		data->adc1Vers = sdCard->rtext[4];
		data->tm2Prescale = 0;
		for(uint8_t i = 0 ; i<2; i++){
			data->tm2Prescale += (uint16_t)sdCard->rtext[5+i]<<(8*i);
		}
		data->adc3Vers = sdCard->rtext[7];
		//ADC1
		data->numAdcCH[0] = sdCard->rtext[8];
		data->adc1Vers = sdCard->rtext[9];
		data->AV0 = sdCard->rtext[10];
		data->Amp0 = sdCard->rtext[11];
		data->Amp1 = sdCard->rtext[12];
		//ADC3
		data->numAdcCH[1] = sdCard->rtext[13];
		data->adc3Vers = sdCard->rtext[14];
		data->AV2 = sdCard->rtext[15];
		data->Amp2 = sdCard->rtext[16];
		data->Amp3 = sdCard->rtext[17];

		sdCard->closeParam();
	}else{
		saveAllParam();
	}
}

void Controller::appliParam()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	htim2.Init.Period = data->tm1Prescale;
	switch (data->tm1Div){
	case 0:
		htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		break;
	case 1:
		htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
		break;
	case 2:
		htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
		break;
	}
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	htim3.Init.Period = data->tm2Prescale;
	switch (data->tm2Div){
	case 0:
		htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		break;
	case 1:
		htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
		break;
	case 2:
		htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
		break;
	}
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}

	//CH0
	if(data->AV0){
		HAL_GPIO_WritePin(GPIOB, AV_0_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOB, AV_0_Pin, GPIO_PIN_SET);
	}

	GPIO_InitStruct.Pin = Amp_0_Pin;
	switch(data->Amp0){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(Amp_0_GPIO_Port, &GPIO_InitStruct);
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_0_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB, Amp_0_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_0_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB, Amp_0_Pin, GPIO_PIN_SET);
		break;
	}

	//CH1
	GPIO_InitStruct.Pin = Amp_1_Pin;
	switch(data->Amp1){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(Amp_1_GPIO_Port, &GPIO_InitStruct);
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_1_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_1_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_1_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_1_Pin, GPIO_PIN_SET);
		break;
	}

	//CH2
	if(data->AV2){
		HAL_GPIO_WritePin(GPIOE, AV_2_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOE, AV_2_Pin, GPIO_PIN_SET);
	}

	GPIO_InitStruct.Pin = Amp_2_Pin;
	switch(data->Amp2){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(Amp_2_GPIO_Port, &GPIO_InitStruct);
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_2_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_2_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_2_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_2_Pin, GPIO_PIN_SET);
		break;
	}

	//CH3
	GPIO_InitStruct.Pin = Amp_3_Pin;
	switch(data->Amp3){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(Amp_3_GPIO_Port, &GPIO_InitStruct);
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_3_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_3_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(Amp_3_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_3_Pin, GPIO_PIN_SET);
		break;
	}

	//adc1
	hadc1.Init.NbrOfConversion = data->numAdcCH[0];
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
		Error_Handler();
	//adc3
	hadc3.Init.NbrOfConversion = data->numAdcCH[1];
	if (HAL_ADC_Init(&hadc3) != HAL_OK)
		Error_Handler();
}

void Controller::saveAllParam(){
	sdCard->openParam();
	sdCard->saveParam(0, (uint8_t*) &data->tm1Prescale,4);
	sdCard->saveParam(4, &data->tm1Div,1);
	sdCard->saveParam(5, (uint8_t*) &data->tm2Prescale,2);
	sdCard->saveParam(7, &data->tm2Div,1);
	sdCard->saveParam(8, &data->numAdcCH[0],1);
	sdCard->saveParam(9, &data->adc1Vers,1);
	sdCard->saveParam(10, &data->AV0,1);
	sdCard->saveParam(11, &data->Amp0,1);
	sdCard->saveParam(12, &data->Amp1,1);
	sdCard->saveParam(13, &data->numAdcCH[1],1);
	sdCard->saveParam(14, &data->adc3Vers,1);
	sdCard->saveParam(15, &data->AV2,1);
	sdCard->saveParam(16, &data->Amp2,1);
	sdCard->saveParam(17, &data->Amp3,1);
	sdCard->closeParam();
}

void Controller::saveParam(uint16_t pos, uint8_t* data){
	sdCard->openParam();
	if(pos==0){
		sdCard->saveParam(pos, data, 4);
	}else if(pos==5){
		sdCard->saveParam(pos, data, 2);
	}else{
		sdCard->saveParam(pos, data, 1);
	}

	sdCard->closeParam();
}

void Controller::saveData(uint8_t adc,uint8_t full) {
	if(adc == 1) {
		if(full == 0) {
			sdCard->saveData(0, ADC_BUFFER_SIZE, &adc1Buffer[0]);
		} else {
			sdCard->saveData(0, ADC_BUFFER_SIZE, &adc1Buffer[ADC_BUFFER_SIZE/2]);
		}

	}else if(adc == 3) {
		if(full == 0) {
			sdCard->saveData(3, ADC_BUFFER_SIZE, &adc3Buffer[0]);
		} else {
			sdCard->saveData(3, ADC_BUFFER_SIZE, &adc3Buffer[ADC_BUFFER_SIZE/2]);
		}
	}
}

void Controller::startData()
{
	sdCard->openData();
	if(data->numAdcCH [0])
		HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);   // For ADC1
	if(data->numAdcCH [1])
		HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_4);   // For ADC3
}

void Controller::stopData()
{
	if(data->numAdcCH [0]){
		HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_2);   // For ADC1

	}
	if(data->numAdcCH [1]){
		HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_4);   // For ADC3
	}

	if((sdCard->lifeData(0, data->adc1Vers, 0)==FR_OK) || (sdCard->lifeData(1, data->adc3Vers, 0)==FR_OK)){
		sdCard->closeData();

		sdCard->openParam();
		if(data->numAdcCH [0])
			sdCard->saveParam(9, &data->adc1Vers, 1);
		if(data->numAdcCH [1])
			sdCard->saveParam(14, &data->adc3Vers, 1);
		sdCard->closeParam();
	}
}

void Controller::dataRecept()
{
	uint32_t val [3] = {0,0,0};
	uint8_t nbCHFile = 0 ;
	uint32_t posFile = 0;
	uint32_t sizeFile = 0;

	memset (TX_buffer, '\0', sizeof(TX_buffer));

	switch (RX_buffer[0]) {

	case 0x01://tm time
		if(RX_buffer[1] == 0){
			data->tm1Prescale = 0;
			for(uint8_t i = 0 ; i<4; i++){
				data->tm1Prescale += (uint32_t)RX_buffer[2+i]<<(8*i);
			}
			data->tm1Div =  RX_buffer[6];

			sdCard->openParam();
			sdCard->saveParam(0, (uint8_t*) &data->tm1Prescale,4);
			sdCard->saveParam(4, &data->tm1Div,1);
			sdCard->closeParam();

			messageOK();
		} else if(RX_buffer[1] == 1){
			data->tm2Prescale = RX_buffer[2] + (RX_buffer[3]<<8);
			data->tm2Div =  RX_buffer[6];

			sdCard->openParam();
			sdCard->saveParam(5, (uint8_t*) &data->tm2Prescale,2);
			sdCard->saveParam(7, &data->tm2Div,1);
			sdCard->closeParam();

			messageOK();
		}else{
			messageNOK();
		}
		break;

	case 0x02://nb CH use
		if(RX_buffer[1] == 0){
			data->numAdcCH[0] = RX_buffer[2];

			sdCard->openParam();
			sdCard->saveParam(8, &data->numAdcCH[0],1);
			sdCard->closeParam();

			messageOK();
		} else if(RX_buffer[1] == 1){
			data->numAdcCH[1] = RX_buffer[2];

			sdCard->openParam();
			sdCard->saveParam(13, &data->numAdcCH[1],1);
			sdCard->closeParam();

			messageOK();
		}else{
			messageNOK();
		}
		break;

	case 0x03://param ch
		if(RX_buffer[1] == 0){
			if(RX_buffer[2] == 0){
				data->AV0=RX_buffer[3];
				data->Amp0=RX_buffer[4];

				sdCard->openParam();
				sdCard->saveParam(10, &data->AV0,1);
				sdCard->saveParam(11, &data->Amp0,1);
				sdCard->closeParam();

				messageOK();
			}else if(RX_buffer[2]==1){
				data->Amp1=RX_buffer[4];
				sdCard->openParam();
				sdCard->saveParam(12, &data->Amp1,1);
				sdCard->closeParam();
				messageOK();
			}else{
				messageNOK();
			}
		} else if(RX_buffer[1] == 1){
			if(RX_buffer[2] == 0){
				data->AV2=RX_buffer[3];
				data->Amp2=RX_buffer[4];

				sdCard->openParam();
				sdCard->saveParam(15, &data->AV2,1);
				sdCard->saveParam(16, &data->Amp2,1);
				sdCard->closeParam();

				messageOK();
			}else if(RX_buffer[2]==1){
				data->Amp3=RX_buffer[4];

				sdCard->openParam();
				sdCard->saveParam(17, &data->Amp3,1);
				sdCard->closeParam();

				messageOK();
			}else{
				messageNOK();
			}
		}else{
			messageNOK();
		}
		break;

	case 0x04://mid
		sdCard->openDataFile(RX_buffer[1], RX_buffer[3], RX_buffer[4]);
		for(uint8_t i = 0 ; i<4; i++)
			posFile += (uint32_t)RX_buffer[8-i]<<(8*i);
		for(uint8_t i = 0 ; i<2; i++)
			sizeFile += (uint32_t)RX_buffer[10-i]<<(8*i);
		sdCard->loadDataFile(0, 1);
		nbCHFile = sdCard->rtext[0];

		TX_buffer[0] = 0x14;
		TX_buffer[1] = RX_buffer[2];

		if(nbCHFile == 1 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+3000*i, 6000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[4+6*x];
					TX_buffer[4] = sdCard->rtext[5+6*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+3*(sizeFile%1000), 6*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[4+6*x];
				TX_buffer[4] = sdCard->rtext[5+6*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[4+12*x];
					TX_buffer[4] = sdCard->rtext[5+12*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[4+12*x];
				TX_buffer[4] = sdCard->rtext[5+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 2){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[10+12*i];
					TX_buffer[4] = sdCard->rtext[11+12*i];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[10+12*x];
				TX_buffer[4] = sdCard->rtext[11+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else
		{
			messageNOK();
		}

		break;

	case 0x05://min
		sdCard->openDataFile(RX_buffer[1], RX_buffer[3], RX_buffer[4]);
		for(uint8_t i = 0 ; i<4; i++)
			posFile += (uint32_t)RX_buffer[8-i]<<(8*i);
		for(uint8_t i = 0 ; i<2; i++)
			sizeFile += (uint32_t)RX_buffer[10-i]<<(8*i);
		sdCard->loadDataFile(0, 1);
		nbCHFile = sdCard->rtext[0];

		TX_buffer[0] = 0x15;
		TX_buffer[1] = RX_buffer[2];

		if(nbCHFile == 1 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+3000*i, 6000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[0+6*x];
					TX_buffer[4] = sdCard->rtext[1+6*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+3*(sizeFile%1000), 6*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[0+6*x];
				TX_buffer[4] = sdCard->rtext[1+6*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[0+12*x];
					TX_buffer[4] = sdCard->rtext[1+12*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[0+12*x];
				TX_buffer[4] = sdCard->rtext[1+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 2){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[6+12*i];
					TX_buffer[4] = sdCard->rtext[7+12*i];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[6+12*x];
				TX_buffer[4] = sdCard->rtext[7+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else
		{
			messageNOK();
		}

	case 0x06://max
		sdCard->openDataFile(RX_buffer[1], RX_buffer[3], RX_buffer[4]);
		for(uint8_t i = 0 ; i<4; i++)
			posFile += (uint32_t)RX_buffer[8-i]<<(8*i);
		for(uint8_t i = 0 ; i<2; i++)
			sizeFile += (uint32_t)RX_buffer[10-i]<<(8*i);
		sdCard->loadDataFile(0, 1);
		nbCHFile = sdCard->rtext[0];

		TX_buffer[0] = 0x16;
		TX_buffer[1] = RX_buffer[2];

		if(nbCHFile == 1 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+3000*i, 6000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[2+6*x];
					TX_buffer[4] = sdCard->rtext[3+6*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+3*(sizeFile%1000), 6*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[2+6*x];
				TX_buffer[4] = sdCard->rtext[3+6*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[2+12*x];
					TX_buffer[4] = sdCard->rtext[3+12*x];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[2+12*x];
				TX_buffer[4] = sdCard->rtext[3+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 2){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[8+12*i];
					TX_buffer[4] = sdCard->rtext[9+12*i];
					CDC_Transmit_FS(TX_buffer, 5);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[8+12*x];
				TX_buffer[4] = sdCard->rtext[9+12*x];
				CDC_Transmit_FS(TX_buffer, 5);
			}
		}else
		{
			messageNOK();
		}

	case 0x07://all
		sdCard->openDataFile(RX_buffer[1], RX_buffer[3], RX_buffer[4]);
		for(uint8_t i = 0 ; i<4; i++)
			posFile += (uint32_t)RX_buffer[8-i]<<(8*i);
		for(uint8_t i = 0 ; i<2; i++)
			sizeFile += (uint32_t)RX_buffer[10-i]<<(8*i);
		sdCard->loadDataFile(0, 1);
		nbCHFile = sdCard->rtext[0];

		TX_buffer[0] = 0x15;
		TX_buffer[1] = RX_buffer[2];

		if(nbCHFile == 1 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+3000*i, 6000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;

					TX_buffer[3] = sdCard->rtext[0+6*x];
					TX_buffer[4] = sdCard->rtext[1+6*x];
					TX_buffer[5] = sdCard->rtext[2+6*x];
					TX_buffer[6] = sdCard->rtext[3+6*x];
					TX_buffer[7] = sdCard->rtext[4+6*x];
					TX_buffer[8] = sdCard->rtext[5+6*x];
					CDC_Transmit_FS(TX_buffer, 9);
				}
			}
			sdCard->loadDataFile(posFile+3*(sizeFile%1000), 6*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[0+6*x];
				TX_buffer[4] = sdCard->rtext[1+6*x];
				TX_buffer[5] = sdCard->rtext[2+6*x];
				TX_buffer[6] = sdCard->rtext[3+6*x];
				TX_buffer[7] = sdCard->rtext[4+6*x];
				TX_buffer[8] = sdCard->rtext[5+6*x];
				CDC_Transmit_FS(TX_buffer, 9);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 1){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[0+12*x];
					TX_buffer[4] = sdCard->rtext[1+12*x];
					TX_buffer[5] = sdCard->rtext[2+12*x];
					TX_buffer[6] = sdCard->rtext[3+12*x];
					TX_buffer[7] = sdCard->rtext[4+12*x];
					TX_buffer[8] = sdCard->rtext[5+12*x];
					CDC_Transmit_FS(TX_buffer, 9);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[0+12*x];
				TX_buffer[4] = sdCard->rtext[1+12*x];
				TX_buffer[5] = sdCard->rtext[2+12*x];
				TX_buffer[6] = sdCard->rtext[3+12*x];
				TX_buffer[7] = sdCard->rtext[4+12*x];
				TX_buffer[8] = sdCard->rtext[5+12*x];
				CDC_Transmit_FS(TX_buffer, 9);
			}
		}else if(nbCHFile == 2 && RX_buffer[2] == 2){
			for (uint16_t i = 0; i < sizeFile/1000; ++i) {
				sdCard->loadDataFile(posFile+6000*i, 12000);
				for (uint16_t x = 0; x < 1000; ++x) {
					TX_buffer[2] = x;
					TX_buffer[3] = sdCard->rtext[6+12*x];
					TX_buffer[4] = sdCard->rtext[7+12*x];
					TX_buffer[5] = sdCard->rtext[8+12*x];
					TX_buffer[6] = sdCard->rtext[9+12*x];
					TX_buffer[7] = sdCard->rtext[10+12*x];
					TX_buffer[8] = sdCard->rtext[11+12*x];
					CDC_Transmit_FS(TX_buffer, 9);
				}
			}
			sdCard->loadDataFile(posFile+6*(sizeFile%1000), 12*(sizeFile%1000));
			for (uint16_t x = 0; x < sizeFile%1000; ++x) {
				TX_buffer[2] = x;
				TX_buffer[3] = sdCard->rtext[6+12*x];
				TX_buffer[4] = sdCard->rtext[7+12*x];
				TX_buffer[5] = sdCard->rtext[8+12*x];
				TX_buffer[6] = sdCard->rtext[9+12*x];
				TX_buffer[7] = sdCard->rtext[10+12*x];
				TX_buffer[8] = sdCard->rtext[11+12*x];
				CDC_Transmit_FS(TX_buffer, 9);
			}
		}else
		{
			messageNOK();
		}
		break;

	case 0x08://delete
		sdCard->deleteData();
		data->adc1Vers = 0;
		data->adc3Vers = 0;

		sdCard->openParam();
		sdCard->saveParam(9, &data->adc1Vers,1);
		sdCard->saveParam(14, &data->adc3Vers,1);
		sdCard->closeParam();

		messageOK();
		break;
	case 0x09://ret ndData

		TX_buffer[0] = 0x19;
		TX_buffer[1] = RX_buffer[1];
		TX_buffer[2] = RX_buffer[2];
		TX_buffer[3] = RX_buffer[3];
		val[0] = sdCard->sizeData(RX_buffer[1], RX_buffer[2], RX_buffer[3]);
		for(uint8_t i = 0 ; i<4; i++){
			TX_buffer[7-i] = val[0]>>(8*i);
		}
		CDC_Transmit_FS(TX_buffer, 8);
		break;

	case 0x0A://ret nbVers

		TX_buffer[0] = 0x1A;
		TX_buffer[1] = RX_buffer[1];
		if(RX_buffer[1] == 0){
			for(uint8_t i = 0 ; i<2; i++){
				TX_buffer[3-i] = data->adc1Vers>>(8*i);
			}
		}else if(RX_buffer[1] == 0){
			for(uint8_t i = 0 ; i<2; i++){
				TX_buffer[3-i] = data->adc3Vers>>(8*i);
			}
		}
		CDC_Transmit_FS(TX_buffer, 4);
		break;
	case 0x0B://ret prescale TM

		TX_buffer[0] = 0x1B;
		TX_buffer[1] = RX_buffer[1];
		if(RX_buffer[1] == 0){
			for(uint8_t i = 0 ; i<4; i++){
				TX_buffer[5-i] = data->tm1Prescale>>(8*i);
			}
			TX_buffer[6] = data->tm1Div;
		}else if(RX_buffer[1] == 1){
			for(uint8_t i = 0 ; i<4; i++){
				TX_buffer[5-i] = data->tm2Prescale>>(8*i);
			}
			TX_buffer[6] = data->tm2Div;
		}
		CDC_Transmit_FS(TX_buffer, 7);
		break;
	case 0x0C://ret param CH
		TX_buffer[0] = 0x1C;
		TX_buffer[1] = RX_buffer[1];
		TX_buffer[2] = RX_buffer[2];
		if(RX_buffer[1] == 0){
			if(RX_buffer[2] == 0){
				TX_buffer[3] = data->AV0;
				TX_buffer[4] = data->Amp0;
			}else if(RX_buffer[2]==1){
				TX_buffer[4] = data->Amp1;
			}
		} else if(RX_buffer[1] == 1){
			if(RX_buffer[2] == 0){
				TX_buffer[3] = data->AV2;
				TX_buffer[4] = data->Amp2;
			}else if(RX_buffer[2]==1){
				TX_buffer[4] = data->Amp3;
			}
		}
		CDC_Transmit_FS(TX_buffer, 5);
		break;

	case 0x0D://ret nb CH
		TX_buffer[0] = 0x1D;
		TX_buffer[1] = RX_buffer[1];
		TX_buffer[2] = data->numAdcCH[RX_buffer[1]];

		CDC_Transmit_FS(TX_buffer, 3);
		break;

	default:
		messageNOK();
		break;
	}
}

void Controller::messageOK(){
	TX_buffer[0] = 0x10;
	TX_buffer[1] = RX_buffer[1];
	CDC_Transmit_FS(TX_buffer, 2);
}

void Controller::messageNOK(){
	TX_buffer[0] = 0x11;
	TX_buffer[1] = RX_buffer[1];
	CDC_Transmit_FS(TX_buffer, 2);
}

void Controller::nivSDcard(){
	nivLED(100-sdCard->freeSize());
}

void Controller::nivBatt(){
	// Get ADC value
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	nivLED(HAL_ADC_GetValue(&hadc2)/655);
}

void Controller::nivLED(uint8_t value){
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	if(value>20)
		HAL_GPIO_WritePin(GPIOB, LED5_Pin, GPIO_PIN_RESET);
	if(value>40)
		HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_RESET);
	if(value>60)
		HAL_GPIO_WritePin(GPIOB, LED7_Pin, GPIO_PIN_RESET);
	if(value>80)
		HAL_GPIO_WritePin(GPIOD, LED8_Pin, GPIO_PIN_RESET);
}
