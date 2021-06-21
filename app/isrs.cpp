#include "main.h"

extern "C" ADC_HandleTypeDef hadc1;
extern "C" ADC_HandleTypeDef hadc3;

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{
    if (hadc == &hadc1)
    {
        control.saveData(1,1);
    }
    else if (hadc == &hadc3)
    {
        control.saveData(3,1);
    }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	 if (hadc == &hadc1)
	    {
	        control.saveData(1,0);
	    }
	    else if (hadc == &hadc3)
	    {
	        control.saveData(3,0);
	    }
	}
