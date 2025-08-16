/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
// Implement SVPWM according to the paper "SVPWM for PMSM" by ASW
// System initialization (clock setup, etc.) is usually called
// automatically by the startup code before main() via SystemInit().
// SystemInit() is defined in system_stm32g4xx.c (part of CMSIS device files)
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "SVPWM.h"
#include <math.h>
#include "GPIO_Init.h"
#include "TIM1_Init.h"
#include "debug.h"
#include "util.h"
#include "stdlib.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "encoder.h"
#include "adc.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//char textBuf[100];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void printText(char *text) {
	while (CDC_Transmit_FS((uint8_t*) text, strlen(text)) == USBD_BUSY)
		;
}

int _write(int file, char *ptr, int len) {
    // Wait until the USB CDC is ready
    while (CDC_Transmit_FS((uint8_t*)ptr, len) == USBD_BUSY) {;}
    return len;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_USB_Device_Init();

#ifdef ESC_BOARD
	adc_init();
#endif

  /* USER CODE BEGIN 2 */

	GPIO_Init(); // Initialize GPIOs
	TIM1_Init(); // Initialize Timer 1
	Encoder_Init(); // Initialize Encoder

	SvpwmData data; // Declare the SVPWM data structure
	data.VBus = 10; // Set the DC bus voltage in the SVPWM data structure

	int i = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	HAL_Delay(10000); // Wait for 1 second to ensure everything is initialized

	printText("Starting SVPWM...\r\n");
	run_svpwm_range(&data, 0.0f, 360.0f, 1.0f); // Run SVPWM for a full rotation from 0 to 360 degrees
	run_svpwm_range(&data, 0.5f, 1.0f, 0.1f);
	run_svpwm_range(&data, 59.5f, 60.5f, 0.1f);

	while (1) {
		GPIOC->ODR ^= (1 << 6); // Toggle PC6

		// debug:
		TIM1->CCR1 = 1000; // Set duty cycle for channel A (PA8)
		TIM1->CCR2 = 1000; // Set duty cycle for channel B (PA9)
		TIM1->CCR3 = 1000; // Set duty cycle for channel C (PA10)

		printf("Start el. round %d \r\n", i++);
		for (float grad = 0; grad < 360; grad += 0.01f) {
			data.grad = grad;

			// Convert degrees to radians
			const float PI_F = 3.14159265358979323846f;
			float rad = grad * (PI_F / 180.0f);
			// Calculate alpha and beta components
			data.Va = (cosf(rad)) * 0.57735f * data.VBus;
			data.Vb = (sinf(rad)) * 0.57735f * data.VBus;

			svpwm_calculate(&data);
			if (data.sector != (int)(grad / 60 +1)) {
				printf("Error: Sector %d does not match expected sector %d for grad %f\r\n",
						data.sector, (int)(grad / 60) + 1, grad);
				while(1);
			}
		}

//		while(1) {;	// Stop with indicating by LED
//			GPIOC->ODR ^= (1 << 6); // Toggle PC6
//			HAL_Delay(1000); // Delay for 1 second
//		}


    /* USER CODE END WHILE */
	}
    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
