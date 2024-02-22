/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define DtoACS1_Pin GPIO_PIN_1
#define DtoACS1_GPIO_Port GPIOD
#define CS1_Pin GPIO_PIN_4
#define CS1_GPIO_Port GPIOE
#define CS2_Pin GPIO_PIN_3
#define CS2_GPIO_Port GPIOE
#define FRAM_CS_Pin GPIO_PIN_13
#define FRAM_CS_GPIO_Port GPIOG
#define DtoACS2_Pin GPIO_PIN_14
#define DtoACS2_GPIO_Port GPIOH
#define CE1_Pin GPIO_PIN_13
#define CE1_GPIO_Port GPIOC
#define FRAM_WP_Pin GPIO_PIN_5
#define FRAM_WP_GPIO_Port GPIOB
#define CS3_Pin GPIO_PIN_13
#define CS3_GPIO_Port GPIOH
#define CE3_Pin GPIO_PIN_11
#define CE3_GPIO_Port GPIOA
#define LCD_BL_CTRL_Pin GPIO_PIN_15
#define LCD_BL_CTRL_GPIO_Port GPIOG
#define V1_switch_Pin GPIO_PIN_6
#define V1_switch_GPIO_Port GPIOE
#define DtoACS3_Pin GPIO_PIN_8
#define DtoACS3_GPIO_Port GPIOG
#define TOUCH_INT_Pin GPIO_PIN_2
#define TOUCH_INT_GPIO_Port GPIOG
#define TOUCH_INT_EXTI_IRQn EXTI2_IRQn
#define V2_switch_Pin GPIO_PIN_9
#define V2_switch_GPIO_Port GPIOF
#define LCD_DISP_Pin GPIO_PIN_10
#define LCD_DISP_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOC
#define CE2_Pin GPIO_PIN_0
#define CE2_GPIO_Port GPIOA
#define EN_Drain_Pin GPIO_PIN_4
#define EN_Drain_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
