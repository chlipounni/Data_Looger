#include "controller.h"

controller::controller() {
	//sdCard = new cardSDRW();
	//data = new appData();

	for(uint8_t x=0; x<sizeof(RX_message); x++){
		RX_message[x]=0;
	}
	for(uint8_t x=0; x<sizeof(TX_message); x++){
		TX_message[x]=0;
	}

	loadData();
	apliData();

	for(uint8_t x=0; x<data.numRank; x++){
		dataValue[x]=0;
	}
	recept = 0;

}

controller::~controller() {

}

void loadParam(){
	sdCard.loadParam(0,0, 10);
	data.tm1Precale=rtext[0];
	data.tm2Precale=rtext[1];
	data.ch1VA=rtext[2];
	data.ch1Amp=rtext[3];
	data.ch2VA=rtext[4];
	data.ch2Amp=rtext[5];
	data.ch3VA=rtext[6];
	data.ch3Amp=rtext[7];
	data.ch4VA=rtext[8];
	data.ch4Amp=rtext[9];

	//load channel for timer
	uint16_t num=0;
	sdCard.loadParam(0,13, sizeof(data.adc1CH));
	for(uint16_t x=0; x<data.adc1CH.size; x++){
		data.adc1CH[x]=rtext[x];
		if(rtext[x]!=0){
			num++;
		}
	}
	data.numRank=num;
}

void appliParam(){
	htim2.Init.Prescaler = data.tm1Prescale;
	htim3.Init.Prescaler = data.tm2Pprescale;

	//CH1
	HAL_GPIO_WritePin(enCH1_GPIO, enCH1, GPIO_PIN_SET);
	if(data.ch1va){
		HAL_GPIO_WritePin(CH1VA_GPIO, CH1VA, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(CH1VA_GPIO, CH1VA, GPIO_PIN_SET);
	}
	switch(data.ch1Amp){
	default:
	case 0:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_SET);
		break;
	}
	HAL_GPIO_WritePin(enCH1_GPIO, enCH1, GPIO_PIN_RESET);

	//CH2
	HAL_GPIO_WritePin(enCH2_GPIO, enCH2, GPIO_PIN_SET);
	if(data.ch2va){
		HAL_GPIO_WritePin(CH2VA_GPIO, CH2VA, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(CH2VA_GPIO, CH2VA, GPIO_PIN_SET);
	}
	switch(data.ch2Amp){
	default:
	case 0:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_SET);
		break;
	}
	HAL_GPIO_WritePin(enCH2_GPIO, enCH2, GPIO_PIN_RESET);

	//CH3
	HAL_GPIO_WritePin(enCH3_GPIO, enCH3, GPIO_PIN_SET);
	if(data.ch3va){
		HAL_GPIO_WritePin(CH3VA_GPIO, CH3VA, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(CH3VA_GPIO, CH3VA, GPIO_PIN_SET);
	}
	switch(data.ch3Amp){
	default:
	case 0:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_SET);
		break;
	}
	HAL_GPIO_WritePin(enCH3_GPIO, enCH3, GPIO_PIN_RESET);

	//CH4
	HAL_GPIO_WritePin(enCH4_GPIO, enCH4, GPIO_PIN_SET);
	if(data.ch4va){
		HAL_GPIO_WritePin(CH4VA_GPIO, CH4VA, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(CH4VA_GPIO, CH4VA, GPIO_PIN_SET);
	}
	switch(data.ch4Amp){
	default:
	case 0:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(CHA0_GPIO, CHA0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CHA1_GPIO, CHA1, GPIO_PIN_SET);
		break;
	}
	HAL_GPIO_WritePin(enCH4_GPIO, enCH4, GPIO_PIN_RESET);

	hadc1.Init.NbrOfConversion = data.numRank;
	/** Configure Regular Channel
	 */
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;

	//nivBatt
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = ADC_REGULAR_RANK_15;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	uint16_t num =0;
	for(uint16_t x=0; x<data.numRank; x++){

		while(adc1CH[num] = 0){
			num++;
		}
		switch (num){
		case 0:
			sConfig.Channel = ADC_CHANNEL_3;
			break;
		case 1:
			sConfig.Channel = ADC_CHANNEL_4;
			break;
		case 2:
			sConfig.Channel = ADC_CHANNEL_5;
			break;
		case 3:
			sConfig.Channel = ADC_CHANNEL_7;
			break;
		case 4:
			sConfig.Channel = ADC_CHANNEL_8;
			break;
		case 5:
			sConfig.Channel = ADC_CHANNEL_9;
			break;
		case 6:
			sConfig.Channel = ADC_CHANNEL_10;
			break;
		case 7:
			sConfig.Channel = ADC_CHANNEL_11;
			break;
		case 8:
			sConfig.Channel = ADC_CHANNEL_14;
			break;
		case 9:
			sConfig.Channel = ADC_CHANNEL_16;
			break;
		case 10:
			sConfig.Channel = ADC_CHANNEL_17;
			break;
		case 11:
			sConfig.Channel = ADC_CHANNEL_18;
			break;
		case 13:
			sConfig.Channel = ADC_CHANNEL_19;
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
		case 2:
			sConfig.Rank = ADC_REGULAR_RANK_4;
			break;
		case 3:
			sConfig.Rank = ADC_REGULAR_RANK_5;
			break;
		case 4:
			sConfig.Rank = ADC_REGULAR_RANK_6;
			break;
		case 5:
			sConfig.Rank = ADC_REGULAR_RANK_7;
			break;
		case 6:
			sConfig.Rank = ADC_REGULAR_RANK_8;
			break;
		case 7:
			sConfig.Rank = ADC_REGULAR_RANK_9;
			break;
		case 8:
			sConfig.Rank = ADC_REGULAR_RANK_10;
			break;
		case 9:
			sConfig.Rank = ADC_REGULAR_RANK_11;
			break;
		case 10:
			sConfig.Rank = ADC_REGULAR_RANK_12;
			break;
		case 11:
			sConfig.Rank = ADC_REGULAR_RANK_13;
			break;
		case 12:
			sConfig.Rank = ADC_REGULAR_RANK_14;
			break;
		case 13:
			sConfig.Rank = ADC_REGULAR_RANK_15;
			break;
		default:
			break;
		}
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

void saveParam(){
	sdCard.saveParam(0,data.tm1Precale);
	sdCard.saveParam(1,data.tm2Precale);
	sdCard.saveParam(2,data.ch1VA);
	sdCard.saveParam(3,data.ch1Amp);
	sdCard.saveParam(4,data.ch2VA);
	sdCard.saveParam(5,data.ch2Amp);
	sdCard.saveParam(6,data.ch3VA);
	sdCard.saveParam(7,data.ch3Amp);
	sdCard.saveParam(8,data.ch4VA);
	sdCard.saveParam(9,data.ch4Amp);

	for(uint16_t x=0; x<sizeof(data.adc1CH); x++){
		sdCard.saveParam(10+x,data.adc1CH[x]);
	}
}

void saveData(){
	uint16_t num =0;
	for(uint16_t x=0; x<data.numRank; x++){

		while(adc1CH[num] = 0){
			num++;
		}
		data.saveData(num,dataValue[x]);
	}
	dataV=0;
}

void dataMain(){
	while(1){
		if(recept)
			dataRecept();
		if(dataV)
			saveData();
	}
}

uint16_t maxValue(uint8_t ch,uint16_t pos, uint16_t size){

	sdCard.loadData(ch,pos,size);
	uint16 val = sdCard.rtext[0];
	for (uint32_t x = 1;  x< data.bytesread; x++) {
		if (sdCard.rtext[x] > val)
			val= sdCard.rtext[x];
	}
	return val;
}

uint16_t minValue(uint8_t ch,uint16_t pos, uint16_t size){

	sdCard.loadData(ch,pos,size);
	uint16 val = sdCard.rtext[0];
	for (uint32_t x = 1;  x< data.bytesread; x++) {
		if (sdCard.rtext[x] < val)
			val= sdCard.rtext[x];
	}
	return val;
}

uint16_t midValue(uint8_t ch,uint16_t pos, uint16_t size){
	uint64 valmid = 0;
	uint16_t num =0;
	uint16_t size =1;
	uint64_t valFinal=0;
	sdCard.loadData(ch,pos,size);
	for (uint32_t x = 0;  x< data.bytesread; x++) {
		if ((sdCard.rtext[x] +valMid) >= 0xFFFF){
			valFinal = valFinal+(valMild/num);
			size++;
			num=1;
			valMid = sdCard.rtext[x];
		}
		else{
			valMid = sdCard.rtext[x] + valmid;
			num++;
		}
	}
	valFinal = valFinal+(valMild/num);
	size++;

	return (uint16_t)valFinal/size;
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

void selectTM(uint8_t tm, uint8_t channel){
	adc1CH[channel-1]=tm;
	sdCard.saveParam(9+channel,tm);
}

void paramCH(uint8_t ch, uint8_t va, uint8_t amp){
	switch (ch) {
	case 1:
		ch1VA = va;
		ch1Amp = amp;
		sdCard.saveParam(2,va);
		sdCard.saveParam(3,data.amp);


		break;
	case 2:
		ch2VA = va;
		ch2Amp = amp;
		sdCard.saveParam(4,va);
		sdCard.saveParam(5,amp);

		break;
	case 3:
		ch3VA = va;
		ch3Amp = amp;
		sdCard.saveParam(6,va);
		sdCard.saveParam(7,amp);

		break;
	case 4:
		ch4VA = va;
		ch4Amp = amp;
		sdCard.saveParam(8,va);
		sdCard.saveParam(9,amp);
		break;
	default:
		break;
	}
}

void paramTM(uint8_t tm, uint16_t prescale){
	switch (tm) {
	case 1:
		tm1Precale =prescale;
		htim2.Init.Prescaler = prescale;
		sdCard.saveParam(0,Precale);

		break;
	case 2:
		tm2Precale =prescale;
		htim3.Init.Prescaler = prescale;
		sdCard.saveParam(1,Precale);
		break;
	default:
		break;
	}

}

void startM(){
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	sdCard.updateName();
}

void stopM(){
	HAL_TIM_BASE_Stop_IT(&htim2);
	HAL_TIM_BASE_Stop_IT(&htim3);
}

//---------[Data Reception]---------
void dataRecept(){
	for(uint8_t x=0; x<5; x++){
		TX_message[x]=0;
	}

	switch (RX[0]) {
	case 0x01:
		data.paramTM(RX_message[1],RX_message[2]);

		TX_message[0]=0x10;
		TX_message[1]=RX_message[1];

		break;
	case 0x02:
		data.selectTM(RX_message[2], RX_message[1]);

		TX_message[0]=0x10;
		TX_message[1]=RX_message[1];

		break;
	case 0x03:
		data.paramCH(RX_message[1], RX_message[2],RX_message[3],RX_message[4]);
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

		data.deleteData(RX_message[1]);
		TX_message[0]=0x10;
		TX_message[1]=RX_message[1];

		break;
	case 0x09:

		data.deleteData(RX_message[1]);
		TX_message[0] = 0x19;
		TX_message[1] = RX_message[1];
		TX_message[2] = RX_message[2];
		TX_message[3] = data.sizeData(RX_message[1],RX_message[2]);

		break;
	case 0x0A:
		saveData();

		TX_message[0]=0x1A;
		TX_message[1] = RX_message[1];
		TX_message[2] = RX_message[2];
		TX_message[3]=data.sizeVersion(RX_message[1]);
		break;

	case 0x0F:
		saveData();

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
