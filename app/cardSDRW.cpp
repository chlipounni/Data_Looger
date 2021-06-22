/*
 * cardSDRW.cpp
 *
 *  Created on: 26 mai 2021
 *      Author: antoine
 */

#include "cardSDRW.h"

cardSDRW::cardSDRW() {
	if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK){
		Error_Handler();
	}

	for (uint32_t var = 0; var < sizeof(rtext); ++var) {
		rtext[var]=0;
	}
}

cardSDRW::~cardSDRW() {
	// TODO Auto-generated destructor stub
}

void cardSDRW::saveParam(uint16_t pos, uint16_t data){
	openParam();
	//Write to the text file
	f_lseek (&SDFile, pos);
	f_write(&SDFile, &data, 1, (UINT*)&byteswritten);
	f_close(&SDFile);
}

void cardSDRW::saveData(uint8_t ch, uint16_t data){
	openCH(ch);
	//Write to the text file
	f_lseek (&SDFile, f_size(&SDFile));
	f_write(&SDFile, &data, 1, (UINT*)&byteswritten);
	f_close(&SDFile);
}

void cardSDRW::loadParam(uint32_t pos, uint32_t size){

	for (uint16_t var = 0; var < size; ++var) {
		rtext[var]=0;
	}

	openParam();
	//pos en byte !!!
	f_lseek (&SDFile, pos*2);
	f_read(&SDFile, rtext, size, (UINT*) &bytesread);
	f_close(&SDFile);
}

void cardSDRW::loadData(uint8_t ch,uint8_t ver,uint32_t pos, uint32_t size){

	for (uint16_t var = 0; var < size; ++var) {
		rtext[var]=0;
	}

	openCH(ch, ver);
	//pos en byte !!!
	f_lseek (&SDFile, pos*2);
	f_read(&SDFile, rtext, size, (UINT*) &bytesread);
	f_close(&SDFile);
}

void cardSDRW::deleteData(uint8_t ch,uint8_t vers){
	char name [10];
	sprintf(name,"ch%d_%d.txt",ch ,vers);
	f_unlink (name);
}

uint16_t cardSDRW::sizeData(uint8_t ch,uint8_t vers){
	uint16_t result = 0 ;
	openCH(ch,vers);
	result = f_truncate (&SDFile);
	f_close(&SDFile);
	return result;
}

uint16_t cardSDRW::sizeVersion(uint8_t ch){
	uint16_t result = 0 ;
	char name [10];
	do{
		sprintf(name,"ch%d_%d.txt",ch ,result+1);
		result = f_open(& SDFile, name, FA_OPEN_EXISTING);
		if(result == FR_OK){
			result++;
			f_close(&SDFile);
		}
	}while(result == FR_OK);

	return result;
}

void cardSDRW::openParam(){

	if(f_open(& SDFile, "param.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void cardSDRW::openCH(uint8_t ch){
	if(ch<1 || ch>19){
		Error_Handler();
	}

	if(f_open(& SDFile, nameFile[ch], FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void cardSDRW::openCH(uint8_t ch,uint8_t vers){
	if(ch<1 || ch>4){
		Error_Handler();
	}
	char name[20];
	sprintf(name,"ch%d_%d.txt",ch ,vers);
	if(f_open(& SDFile, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void cardSDRW::updateName(){
	char name [20];
	uint16_t num;
	FRESULT result;

	for(uint8_t x = 0; x<4 ;x++){
		sprintf(name,"");
		num = 0;
		do{
			num++;
			sprintf(name,"ch%d_%d.txt",x ,num);
			result = f_open(& SDFile, name, FA_OPEN_EXISTING);
			if(result == FR_OK){f_close(&SDFile);}

		}while(result == FR_OK);
		sprintf(nameFile[x],name);
	}	
}

uint16_t cardSDRW::levelSD(){
	   char str[12];
	    f_getlabel("", str, 0);


	    FATFS *fs;
	       DWORD fre_clust, fre_sect, tot_sect;


	       /* Get volume information and free clusters of drive 1 */
	       res = f_getfree("", &fre_clust, &fs);

	       /* Get total sectors and free sectors */
	       tot_sect = (fs->n_fatent - 2) * fs->csize;
	       fre_sect = fre_clust * fs->csize;

	       /* Print the free space (assuming 512 bytes/sector) */
	       //printf("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);

	       return (fre_sect *100)/tot_sect;
}

