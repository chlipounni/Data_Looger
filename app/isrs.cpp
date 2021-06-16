#include "main.h"

extern "C" ADC_HandleTypeDef hadc1;
extern "C" ADC_HandleTypeDef hadc3;

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{
    static uint32_t dmaIrqCounterAdc1 = 0;
    static uint32_t dmaIrqCounterAdc2 = 0;

    if (hadc == &hadc1)
    {
        dmaIrqCounterAdc1++;
    }
    else if (hadc == &hadc3)
    {
        dmaIrqCounterAdc2++;
    }
}
