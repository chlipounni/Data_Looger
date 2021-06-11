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

	for (var = 0; var < sizeof(rtext); ++var) {
		rtext[var]=0;
	}
}

cardSDRW::~cardSDRW() {
	// TODO Auto-generated destructor stub
}

void saveParam(uint16_T pos, uint16_t data){
	openParam();
	//Write to the text file
	f_lseek (&SDFile, pos);
	f_write(&SDFile, data, 1, (void *)&byteswritten);
	f_close(&SDFile);
}

void saveData(uint8_T ch, uint16_t data){
	openCH(ch);
	//Write to the text file
	f_lseek (&SDFile, f_size(&SDFile));
	f_write(&SDFile, data, 1, (void *)&byteswritten);
	f_close(&SDFile);
}

void loadParam(uint8_t ch,uint32_T pos, uintr32_t size){

	for (var = 0; var < size; ++var) {
		rtext[var]=0;
	}

	openParam(ch);
	//pos en byte !!!
	f_lseek (&SDFile, pos*2);
	f_read(&SDFile, rtext, size, bytesread);
	f_close(&SDFile);
}

void loadData(uint8_t ch,uint32_T pos, uintr32_t size){

	for (var = 0; var < size; ++var) {
		rtext[var]=0;
	}

	openCH(ch);
	//pos en byte !!!
	f_lseek (&SDFile, pos*2);
	f_read(&SDFile, rtext, size, bytesread);
	f_close(&SDFile);
}

void deleteData(uint8_t ch,uint8_t vers){
	char* name [10];
	sprintf(name,"ch%d_%d.txt",ch ,vers);
	f_unlink (name);
}

uint16_t sizeData(uint8_t ch,uint8_t vers){
	uint16_t result = 0 ;
	openCH(ch,vers);
	result f_truncate (&SDFile);
	f_close(&SDFile);
	return result;
}

uint16_t sizeVersion(uint8_t ch){
	uint16_t result = 0 ;
	char* name [10];
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

void openParam(){

	if(f_open(& SDFile, "param.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void openCH(uint8_t ch){
	if(ch<1 || ch>13){
		Error_Handler();
	}

	if(f_open(& SDFile, nameFile[ch], FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void openCH(uint8_t ch,uint8_t vers){
	if(ch<1 || ch>13){
		Error_Handler();
	}
	char* name [10] = "";
	sprintf(name,"ch%d_%d.txt",ch ,vers);
	if(f_open(& SDFile, name, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != (FR_OK)){
		Error_Handler();
	}
}

void updateName(){
	char* name [10];
	uint16_t num;
	FRESULT result;

	for(uint8_t x = 0; x<13 ;x++){
		name = "";
		num = 0;
		result;
		do{
			num++;
			sprintf(name,"ch%d_%d.txt",x ,num);
			result = f_open(& SDFile, name, FA_OPEN_EXISTING);
			if(result == FR_OK){f_close(&SDFile);}

		}while(result == FR_OK);
		nameFile[x]= name;
	}	
}

uint16_t levelSD(){
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
	       printf("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);
}

