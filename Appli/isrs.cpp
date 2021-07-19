#include "main.h"
#include "usbd_cdc_if.h"
#include "Appli/controller.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern uint16_t adc1Buffer[ADC_BUFFER_SIZE] __attribute__ ((aligned (8)));
extern uint16_t adc3Buffer[ADC_BUFFER_SIZE] __attribute__ ((aligned (8)));

extern Controller* theController;

uint8_t x = 0;

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc)
{
	if (hadc == &hadc1)
	{
		theController->saveData(1,0);
	}
	else if (hadc == &hadc3)
	{
		theController->saveData(3,0);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	if (hadc == &hadc1)
	{

		theController->saveData(1,1);
	}
	else if (hadc == &hadc3)
	{
		theController->saveData(3,1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	GPIO_PinState etat;
	if(GPIO_Pin == BP_Pin)
	{
		etat = HAL_GPIO_ReadPin(BP_GPIO_Port, BP_Pin);
		if(etat == GPIO_PIN_RESET)
		{
			theController->loadNiv();
			//view niv SDcard
			theController->loadNiv();
			theController->nivSDcard();

		}else if(etat == GPIO_PIN_SET){
			//view niv batt
			theController->nivBatt();
		}

	}else if(GPIO_Pin == SW1_Pin){
		etat = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
		if(etat == GPIO_PIN_RESET)
		{
			theController->stopData();
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

		}
		else if(etat == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			theController->startData();

		}
	}
}
