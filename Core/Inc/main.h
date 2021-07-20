/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AIN0_Pin GPIO_PIN_2
#define AIN0_GPIO_Port GPIOC
#define AIN1_Pin GPIO_PIN_3
#define AIN1_GPIO_Port GPIOC
#define AIN2_Pin GPIO_PIN_6
#define AIN2_GPIO_Port GPIOA
#define AIN3_Pin GPIO_PIN_4
#define AIN3_GPIO_Port GPIOC
#define AINBATT_Pin GPIO_PIN_1
#define AINBATT_GPIO_Port GPIOB
#define Amp_3_Pin GPIO_PIN_7
#define Amp_3_GPIO_Port GPIOE
#define AV_2_Pin GPIO_PIN_9
#define AV_2_GPIO_Port GPIOE
#define Amp_2_Pin GPIO_PIN_11
#define Amp_2_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_12
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_13
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_14
#define LED6_GPIO_Port GPIOB
#define LED7_Pin GPIO_PIN_15
#define LED7_GPIO_Port GPIOB
#define LED8_Pin GPIO_PIN_8
#define LED8_GPIO_Port GPIOD
#define CHGspd_Pin GPIO_PIN_9
#define CHGspd_GPIO_Port GPIOD
#define BP_Pin GPIO_PIN_10
#define BP_GPIO_Port GPIOD
#define BP_EXTI_IRQn EXTI15_10_IRQn
#define LED3_Pin GPIO_PIN_11
#define LED3_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOC
#define SD_DETECT_Pin GPIO_PIN_7
#define SD_DETECT_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_8
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI9_5_IRQn
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOA
#define USB_DN_Pin GPIO_PIN_11
#define USB_DN_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define DEBUG_VCP_RX_Pin GPIO_PIN_0
#define DEBUG_VCP_RX_GPIO_Port GPIOD
#define DEBUG_VCP_TX_Pin GPIO_PIN_1
#define DEBUG_VCP_TX_GPIO_Port GPIOD
#define Amp_0_Pin GPIO_PIN_4
#define Amp_0_GPIO_Port GPIOB
#define AV_0_Pin GPIO_PIN_6
#define AV_0_GPIO_Port GPIOB
#define Amp_1_Pin GPIO_PIN_0
#define Amp_1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
