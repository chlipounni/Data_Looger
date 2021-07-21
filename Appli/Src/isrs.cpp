#include "main.h"
#include "usbd_cdc_if.h"
<<<<<<< HEAD:Appli/Src/isrs.cpp
#include "Appli/Inc/controller.h"
=======
#include "app/Inc/controller.h"
>>>>>>> 68f92784134064e32d7ff9a30d487dc831eed5f2:app/Src/isrs.cpp

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
	__disable_irq();
	if (hadc == &hadc1)
	{
		theController->saveData(1,0);
	}
	else if (hadc == &hadc3)
	{
		theController->saveData(3,0);
	}
	__enable_irq();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	__disable_irq();
	if (hadc == &hadc1)
	{

		theController->saveData(1,1);
	}
	else if (hadc == &hadc3)
	{
		theController->saveData(3,1);
	}
	__enable_irq();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
<<<<<<< HEAD:Appli/Src/isrs.cpp

=======
>>>>>>> 68f92784134064e32d7ff9a30d487dc831eed5f2:app/Src/isrs.cpp
	static uint8_t toggle = 0;
	GPIO_PinState etat;
	if(GPIO_Pin == BP_Pin)
	{
		__disable_irq();
		etat = HAL_GPIO_ReadPin(BP_GPIO_Port, BP_Pin);
		if(etat == GPIO_PIN_SET)
		{
			if(toggle){
				toggle = 0;
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
				//view niv SDcard
				theController->nivSDcard();
			}else{
				toggle = 1;
				//view niv batt
				theController->nivBatt();
			}
<<<<<<< HEAD:Appli/Src/isrs.cpp
			__enable_irq();
=======
>>>>>>> 68f92784134064e32d7ff9a30d487dc831eed5f2:app/Src/isrs.cpp
		}else if(etat == GPIO_PIN_RESET){
			//led off
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);

			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, LED5_Pin|LED6_Pin|LED7_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, LED8_Pin, GPIO_PIN_SET);
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
