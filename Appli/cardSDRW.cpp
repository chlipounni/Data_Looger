/*
 * cardSDRW.cpp
 */

#include "cardSDRW.h"

cardSDRW::cardSDRW(appData* pointData)
{
	data = pointData;

	if(f_mount(&SDFatFS, "", 0) != FR_OK)
		Error_Handler();
	for (uint32_t i = 0; i < _MAX_SS-1; ++i)
		rtext[i]=0;
	for (uint8_t i = 0; i < 7; ++i)
		SDFile[i] = new FIL;
	paramFile = new FIL;
}

cardSDRW::~cardSDRW()
{
}

void cardSDRW::openData()
{
	char text [20];
	if(data->numAdcCH[0]){
		//adc1
		sprintf(text,"0_%d_div0.smp",data->adc1Vers);
		if( f_open(SDFile[0], text, FA_CREATE_ALWAYS | FA_WRITE | FA_READ ) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[0],(uint8_t*) data->numAdcCH[0], 1, &byteswritten) != FR_OK)
			Error_Handler();

		sprintf(text,"0_%d_div1.smp",data->adc1Vers);
		if( f_open(SDFile[1], text, FA_CREATE_ALWAYS | FA_WRITE | FA_READ ) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[1],(uint8_t*) data->numAdcCH[0], 1, &byteswritten) != FR_OK)
			Error_Handler();

		sprintf(text,"0_%d_div2.smp",data->adc1Vers);
		if( f_open(SDFile[2], text, FA_CREATE_ALWAYS | FA_WRITE ) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[2],(uint8_t*) data->numAdcCH[0], 1, &byteswritten) != FR_OK)
			Error_Handler();
	}

	if(data->numAdcCH[1]){
		//adc3
		sprintf(text,"1_%d_div0.smp",data->adc3Vers);
		if( f_open(SDFile[3], text, FA_CREATE_ALWAYS |  FA_WRITE |  FA_READ) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[3],(uint8_t*) data->numAdcCH[1], 1, &byteswritten) != FR_OK)
			Error_Handler();

		sprintf(text,"1_%d_div1.smp",data->adc3Vers);
		if( f_open(SDFile[4], text, FA_CREATE_ALWAYS |  FA_WRITE | FA_READ) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[4],(uint8_t*) data->numAdcCH[1], 1, &byteswritten) != FR_OK)
			Error_Handler();

		sprintf(text,"1_%d_div2.smp",data->adc3Vers);
		if( f_open(SDFile[5], text, FA_CREATE_ALWAYS |  FA_WRITE) != FR_OK)
			Error_Handler();
		if(f_write(SDFile[5],(uint8_t*) data->numAdcCH[1], 1, &byteswritten) != FR_OK)
			Error_Handler();
	}
}

void cardSDRW::sync(uint8_t file){
	if( f_sync(SDFile[file]) != FR_OK)
		Error_Handler();
}

void cardSDRW::deleteData(){
	char text [20];
	//adc1
	for(uint8_t i = 0; i<=data->adc1Vers; i++){
		sprintf(text,"0_%d_div0.smp",i);
		if( f_unlink (text) != FR_OK)
			Error_Handler();
		sprintf(text,"0_%d_div1.smp",i);
		if(  f_unlink (text)  != FR_OK)
			Error_Handler();
		sprintf(text,"0_%d_div2.smp",i);
		if(  f_unlink (text)  != FR_OK)
			Error_Handler();
	}
	//adc3
	for(uint8_t i = 0; i<=data->adc3Vers; i++){
		sprintf(text,"1_%d_div0.smp",i);
		if( f_unlink (text) != FR_OK)
			Error_Handler();
		sprintf(text,"1_%d_div1.smp",i);
		if(  f_unlink (text)  != FR_OK)
			Error_Handler();
		sprintf(text,"1_%d_div2.smp",i);
		if(  f_unlink (text)  != FR_OK)
			Error_Handler();
	}
}

void cardSDRW::saveData(uint8_t file,uint16_t size, uint16_t* data)
{
	if(f_write(SDFile[file],(void*) data, size, &byteswritten) != FR_OK)
		Error_Handler();
}

void cardSDRW::loadData(uint8_t file,uint32_t pos, uint16_t size)
{
	if(f_lseek (SDFile[file], pos) != FR_OK)
		Error_Handler();
	if(f_read(SDFile[file], rtext, size, (UINT*) &bytesread) != FR_OK)
		Error_Handler();
}

void cardSDRW::closeData()
{
	FILINFO fno;
	char text [20];

	if(data->numAdcCH[0]){
		sprintf(text,"0_%d_div0.smp",data->adc1Vers);
		if(f_stat (text, &fno) == FR_OK){
			data->adc1Vers++;
			//adc1
			if( f_close(SDFile[0]) != FR_OK)
				Error_Handler();
			if( f_close(SDFile[1]) != FR_OK)
				Error_Handler();
			if( f_close(SDFile[2]) != FR_OK)
				Error_Handler();
		}
	}

	if(data->numAdcCH[1]){
		//adc3
		sprintf(text,"1_%d_div0.smp",data->adc3Vers);
		if(f_stat (text, &fno) == FR_OK){
			data->adc3Vers++;
			if( f_close(SDFile[3]) != FR_OK)
				Error_Handler();
			if( f_close(SDFile[4]) != FR_OK)
				Error_Handler();
			if(f_close(SDFile[5]) != FR_OK)
				Error_Handler();
		}
	}
}

// solo file Data
void cardSDRW::openDataFile(uint8_t adc, uint8_t vers, uint8_t div){
	char text [20];
	sprintf(text,"%d_%d_div%d.smp",adc,vers,div);
	if( f_open(SDFile[6], text, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != FR_OK)
		Error_Handler();
}

void cardSDRW::loadDataFile(uint32_t pos, uint16_t size){
	if(f_lseek (SDFile[6], pos) != FR_OK)
		Error_Handler();
	if(f_read(SDFile[6], rtext, size*2, (UINT*) &bytesread) != FR_OK)
		Error_Handler();
}

uint32_t cardSDRW::sizeData(uint8_t adc, uint8_t vers,uint8_t div)
{
	//0-> adc1; 1-> adc3
	FILINFO fno;
	char text [20];
	sprintf(text,"%d_%d_div%d.smp",adc,vers,div);
	if( f_stat(text, &fno) != FR_OK)
		return 0;
	return fno.fsize;
}

FRESULT cardSDRW::lifeData(uint8_t adc, uint8_t vers,uint8_t div)
{
	//0-> adc1; 1-> adc3
	FILINFO fno;
	char text [20];
	sprintf(text,"%d_%d_div%d.smp",adc,vers,div);
	return  f_stat(text, &fno);
}

void cardSDRW::closeDataFile(){
	if( f_close(SDFile[6]) != FR_OK)
		Error_Handler();
}

void cardSDRW::openParam()
{
	if( f_open(paramFile, "param.par", FA_OPEN_ALWAYS | FA_WRITE | FA_READ) != FR_OK)
		Error_Handler();
}

void cardSDRW::closeParam()
{
	if( f_close(paramFile) != FR_OK)
		Error_Handler();
}

uint16_t cardSDRW::sizeParam()
{
	FILINFO fno;
	if( f_stat("param.par", &fno) != FR_OK)
		return 0;
	return fno.fsize;
}

void cardSDRW::loadParam(uint16_t pos, uint16_t size)
{
	//pos en byte !!!
	f_lseek (paramFile, pos);
	if(f_read(paramFile, rtext, size, (UINT*) &bytesread) != FR_OK)
		Error_Handler();
}

void cardSDRW::saveParam(uint16_t pos, uint8_t* data, uint16_t size)
{
	//pos en byte !!!
	f_lseek (paramFile, pos);
	if(f_write(paramFile,(void*) data, size, &byteswritten) != FR_OK)
		Error_Handler();
}
