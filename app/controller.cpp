#include "controller.h"
#include "usb_device.h"

extern "C" ADC_HandleTypeDef hadc1;
extern "C" ADC_HandleTypeDef hadc3;
extern "C" TIM_HandleTypeDef htim2;
extern "C" TIM_HandleTypeDef htim3;
extern "C" uint16_t rtext[_MAX_SS];


controller::controller() {

	for(uint8_t x=0; x<sizeof(RX_message); x++){
		RX_message[x]=0;
	}
	for(uint8_t x=0; x<sizeof(TX_message); x++){
		TX_message[x]=0;
	}

	loadParam();
	appliParam();

	recept = 0;

}

controller::~controller() {

}

void loadParam(){
	sdCard.loadParam(0,0, 10);
	data.tm1Precale=rtext[0];
	data.tm2Precale=rtext[1];

	data.AV0=rtext[2];
	data.Amp0=rtext[3];
	data.Amp1=rtext[4];

	data.AV2=rtext[5];
	data.Amp2=rtext[6];
	data.Amp3=rtext[7];

	//load channel for timer1
	uint16_t num1=0;
	sdCard.loadParam(0,10, sizeof(data.adc1CH));
	for(uint16_t x=0; x<sizeof(data.adc1CH); x++){
		data.adc1CH[x]=rtext[x];
		if(rtext[x]!=0){
			num1++;
		}
	}
	data.numRankADC1=num1;

	//load channel for timer1
	uint16_t num2=0;
	sdCard.loadParam(0,10 + data.adc1CH.size, sizeof(data.adc3CH));
	for(uint16_t x=0; x<sizeof(data.adc3CH); x++){
		data.adc3CH[x]=rtext[x];
		if(rtext[x]!=0){
			num2++;
		}
	}
	data.numRankADC3=num2;
}

void appliParam(){
	htim2.Init.Prescaler = data.tm1Prescale;
	htim3.Init.Prescaler = data.tm2Pprescale;

	//CH0
	if(data.AV0){
		HAL_GPIO_WritePin(GPIOB, AV_0_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOB, AV_0_Pin, GPIO_PIN_SET);
	}

	GPIO_InitStruct.Pin = Amp_0_Pin;
	switch(data.Amp0){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_WritePin(GPIOB, Amp_0_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_WritePin(GPIOB, Amp_0_Pin, GPIO_PIN_SET);
		break;
	}
	HAL_GPIO_Init(Amp_0_GPIO_Port, &GPIO_InitStruct);

	//CH1
	GPIO_InitStruct.Pin = Amp_1_Pin;
	switch(data.Amp1){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
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
	if(data.AV2){
		HAL_GPIO_WritePin(GPIOE, AV_2_Pin, GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(GPIOE, AV_2_Pin, GPIO_PIN_SET);
	}

	GPIO_InitStruct.Pin = Amp_2_Pin;
	switch(data.Amp2){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		AL_GPIO_Init(Amp_2_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_2_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		AL_GPIO_Init(Amp_2_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_2_Pin, GPIO_PIN_SET);
		break;
	}

	//CH3
	GPIO_InitStruct.Pin = Amp_3_Pin;
	switch(data.Amp3){
	default:
	case 0:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		break;
	case 1:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		AL_GPIO_Init(Amp_3_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_3_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		AL_GPIO_Init(Amp_3_GPIO_Port, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, Amp_3_Pin, GPIO_PIN_SET);
		break;
	}


	ADC_ChannelConfTypeDef sConfig = {0};
	//nivBatt
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	uint16_t num =0;
	//ADC1
	for(uint16_t x=0; x<data.numRank; x++){

		while(data.adc1CH[num] == 0){
			num++;
		}
		switch (num){
		case 0:
			sConfig.Channel = ADC_CHANNEL_3;
			break;
		case 1:
			sConfig.Channel = ADC_CHANNEL_4;
			break;
		default:
			break;
		}

		switch (x){
		case 0:
			sConfig.Rank = ADC_REGULAR_RANK_2;
			break;
		case 1:
			sConfig.Rank = ADC_REGULAR_RANK_3;
			break;
		default:
			break;
		}
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}
	//ADC3
	sConfig = {0};
	for(uint16_t x=0; x<data.numRankADC3; x++){

		while(data.adc3CH[num] = 0){
			num++;
		}
		switch (num){
		case 0:
			sConfig.Channel = ADC_CHANNEL_0;
			break;
		case 1:
			sConfig.Channel = ADC_CHANNEL_1;
			break;
		default:
			break;
		}

		switch (x){
		case 0:
			sConfig.Rank = ADC_REGULAR_RANK_1;
			break;
		case 1:
			sConfig.Rank = ADC_REGULAR_RANK_2;
			break;
		default:
			break;
		}
		if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

void saveParam(){
	sdCard.saveParam(0,data.tm1Precale);
	sdCard.saveParam(1,data.tm2Precale);
	sdCard.saveParam(2,data.AV0);
	sdCard.saveParam(3,data.Amp0);
	sdCard.saveParam(4,data.Amp1);
	sdCard.saveParam(5,data.AV2);
	sdCard.saveParam(6,data.Amp2);
	sdCard.saveParam(7,data.Amp3);

	for(uint16_t x=0; x<sizeof(data.adc1CH); x++){
		sdCard.saveParam(10+x,data.adc1CH[x]);
	}
	for(uint16_t x=0; x<sizeof(data.adc3CH); x++){
		sdCard.saveParam(10+x+sizeof(data.adc1CH),data.adc3CH[x]);
	}
}

void saveData(uint8_t adc, uint8_t half){
	uint16_t num = 0;
	if(adc==1){
		for(uint16_t x=0; x<data.numRankADC1; x++){

			while(data.adc1CH[num] == 0){
				num++;
			}
			if(half==1){
				sdCard.saveData(num,adc1Buffer[x]);
			}else{
				sdCard.saveData(num,adc1Buffer[data.numRankADC1+x]);
			}
		}
	}
	else if(adc == 3){
		for(uint16_t x=0; x<data.numRankADC3; x++){

			while(data.adc3CH[num] == 0){
				num++;
			}
			if(half==1){
				sdCard.saveData(num,adc3Buffer[x]);
			}else{
				sdCard.saveData(num,adc3Buffer[data.numRankADC3+x]);
			}
		}
	}
}

uint16_t maxValue(uint8_t ch,uint8_t vers,uint16_t pos, uint16_t size){

	sdCard.loadData(ch,vers,pos,size);
	uint16_t val = sdCard.rtext[0];
	for (uint32_t x = 1;  x< sdCard.bytesread; x++) {
		if (sdCard.rtext[x] > val)
			val= sdCard.rtext[x];
	}
	return val;
}

uint16_t minValue(uint8_t ch,uint8_t vers,uint16_t pos, uint16_t size){

	sdCard.loadData(ch,vers,pos,size);
	uint16 val = sdCard.rtext[0];
	for (uint32_t x = 1;  x< data.bytesread; x++) {
		if (sdCard.rtext[x] < val)
			val= sdCard.rtext[x];
	}
	return val;
}

uint16_t midValue(uint8_t ch,uint8_t vers,uint16_t pos, uint16_t size){
	uint64_t valmid = 0;
	uint16_t num =0;
	uint16_t sizeNum = 1;
	uint64_t valFinal=0;
	sdCard.loadData(ch,vers,pos,size);
	for (uint32_t x = 0;  x< sdCard.bytesread; x++) {
		if ((sdCard.rtext[x] +valmid) >= 0xFFFF){
			valFinal = valFinal+(valmid/num);
			sizeNum++;
			num=1;
			valmid = sdCard.rtext[x];
		}
		else{
			valmid = sdCard.rtext[x] + valmid;
			num++;
		}
	}
	valFinal = valFinal+(valmid/num);
	sizeNum++;

	return (uint16_t)valFinal/sizeNum;
}

void nivBatt (){
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED5_Pin|LED6_Pin|LED7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED8_Pin|CHGspd_Pin, GPIO_PIN_RESET);

	if(data.levelBatt>3){
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	}
	if(data.levelBatt>3.3){
		HAL_GPIO_WritePin(GPIOB, LED5_Pin, GPIO_PIN_SET);
	}
	if(data.levelBatt>3.4){
		HAL_GPIO_WritePin(GPIOB, LED6_Pin, GPIO_PIN_SET);
	}
	if(data.levelBatt>3.5){
		HAL_GPIO_WritePin(GPIOB, LED7_Pin, GPIO_PIN_SET);
	}
	if(data.levelBatt>3.6){
		HAL_GPIO_WritePin(GPIOD, LED8_Pin, GPIO_PIN_SET);
	}
}

void selectTM(uint8_t adc, uint8_t channel, uint8_t use){
	if(adc == 1){
		data.adc1CH[channel]=use;
		sdCard.saveParam(10+channel,use);
	}else if(adc == 3){
		data.adc3CH[channel]=use;
		sdCard.saveParam(10+sizeof(data.adc1CH)+channel,use);
	}
}

void paramCH(uint8_t ch, uint8_t va, uint8_t amp){
	switch (ch) {
	case 1:
		data.AV0 = va;
		data.Amp0 = amp;
		sdCard.saveParam(2,va);
		sdCard.saveParam(3,data.amp);


		break;
	case 2:

		data.Amp1 = amp;
		sdCard.saveParam(4,amp);

		break;
	case 3:
		data.AV2 = va;
		sdCard.saveParam(5,va);
		data.Amp2 = amp;
		sdCard.saveParam(6,amp);

		break;
	case 4:

		data.Amp3 = amp;
		sdCard.saveParam(7,amp);
		break;
	default:
		break;
	}
}

void paramTM(uint8_t tm, uint16_t prescale){
	switch (tm) {
	case 1:
		data.tm1Precale =prescale;
		htim2.Init.Prescaler = prescale;
		sdCard.saveParam(0,Precale);

		break;
	case 2:
		data.tm2Precale =prescale;
		htim3.Init.Prescaler = prescale;
		sdCard.saveParam(1,Precale);
		break;
	default:
		break;
	}

}

void startM(){
	// Start timers triggering ADCs
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);   // For ADC1
	HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_4);   // For ADC3
	sdCard.updateName();
}

void stopM(){
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim3);
}

//---------[Data Reception]---------
void dataRecept(){
	while(1){
		while(recept == 0){

		}
		for(uint8_t x=0; x<5; x++){
			TX_message[x]=0;
		}

		switch (RX_message[0]) {
		case 0x01:
			data.paramTM(RX_message[1],RX_message[2]);

			TX_message[0]=0x10;
			TX_message[1]=RX_message[1];

			break;
		case 0x02:
			selectTM(RX_message[1], RX_message[2], RX_message[3]);

			TX_message[0]=0x10;
			TX_message[1]=RX_message[1];

			break;
		case 0x03:
			paramCH(RX_message[1], RX_message[2],RX_message[3]);
			TX_message[0]=0x10;
			TX_message[1]=RX_message[1];

			break;
		case 0x04:
			TX_message[0]=0x14;
			TX_message[1]=RX_message[1];
			TX_message[2]=RX_message[2];
			TX_message[3]=midValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);

			break;
		case 0x05:
			TX_message[0]=0x15;
			TX_message[1]=RX_message[1];
			TX_message[2]=RX_message[2];
			TX_message[3]=minValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);

			break;
		case 0x06:
			TX_message[0]=0x16;
			TX_message[1]=RX_message[1];
			TX_message[2]=RX_message[2];
			TX_message[3]=maxValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);

			break;
		case 0x07:
			TX_message[0]=0x17;
			TX_message[1]=RX_message[1];
			TX_message[2]=RX_message[2];
			TX_message[3]=midValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);
			TX_message[4]=minValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);
			TX_message[5]=maxValue(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);

			break;
		case 0x08:

			deleteData(RX_message[1],RX_message[2]);
			TX_message[0]=0x10;
			TX_message[1]=RX_message[1];

			break;
		case 0x09:

			TX_message[0] = 0x19;
			TX_message[1] = RX_message[1];
			TX_message[2] = RX_message[2];
			TX_message[3] = sdCard.sizeData(RX_message[1],RX_message[2]);

			break;
		case 0x0A:
			TX_message[0]=0x1A;
			TX_message[1] = RX_message[1];
			TX_message[2]=data.sizeVersion(RX_message[1]);
			break;

		case 0x0F:
			saveParam();

			TX_message[0]=0x10;
			TX_message[1]=RX_message[1];

			break;
		default:
			TX_message[0]=0x11;
			break;
		}
		CDC_Transmit_FS(TX_message, 5);
		recept = 0;
	}
}

