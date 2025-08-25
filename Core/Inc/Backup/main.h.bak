/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define WEACT_BOARD		1  		// Set to 1 for WeAct board
//#define ESC_BOARD		1		// Set to 1 for ESC board

#define PWM_FREQ            20000      // 20 kHz
#define COUNTER_MAX_FREQ    170000000  // 170 MHz
#define T_VAL   			((uint16_t)(((COUNTER_MAX_FREQ / PWM_FREQ) / 2) & ~1U))	// & ~1U makes sure it is an even number
#define Vbus                10.0         // DC bus voltage (V)
#define SQRT3             	(float)1.7320508075688772 // Square root of 3


#define ENCODER_CPR 		1024
#define COUNTS_PER_REV 		(ENCODER_CPR)


extern UART_HandleTypeDef huart2;	// UART handle

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Red_LED_Pin GPIO_PIN_6
#define Red_LED_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
