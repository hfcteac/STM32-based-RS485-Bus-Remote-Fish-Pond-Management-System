/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <oled.h>
#include <bsp_ds18b20.h>

/**
 * ��������: �ض���c�⺯��printf��DEBUG_USARTx
 * �������?: ��
 * �� �� ֵ: ��
 * ˵    ������
 */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}
/**
 * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
 * �������?: ��
 * �� �� ֵ: ��
 * ˵    ������
 */
int fgetc(FILE *f)
{
	uint8_t ch = 0;
	HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
	return ch;
}

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include <string.h>

#define RXBUFFERSIZE 256	 //�������ֽ���
char RxBuffer[RXBUFFERSIZE]; //��������
uint8_t aRxBuffer;			 //�����жϻ���
uint8_t Uart1_Rx_Cnt = 0;	 //���ջ������?
double adcDATA;
float temp;
unsigned int menu = 0, k = 0, l = 0, m = 1, n = 0, pwmnum = 200, tempseth = 45, tempsetl = 22;
// menu����˵�ѡ��k��ú��ʹ�ܣ�l�ǻ�ʹ�ܣ�m����ʪ���޸ģ�n�Ƿ�������ת��pwmnum���ٶ�ѡ��

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t ADC_Value[100];
 uint8_t i;
 uint32_t ad1,ad2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  //MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
//	OLED_Init();
//	OLED_Clear();
DQ_GPIO_Init();
Ds18b20_Init();
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		HAL_Delay(200);
		for(i = 0,ad1 =0,ad2=0; i < 100;){
				ad1 += ADC_Value[i++];
				ad2 += ADC_Value[i++];
		}
		ad1 /= 50;
		ad2 /= 50;
		LOOP:temp=Ds18b20_Get_Temp();
		if(temp>50)
			goto LOOP;
		printf("%04.0f", (ad1*3.3f/4096)*1000);
		printf("%04.0f ", (ad2*3.3f/4096)*1000);
		printf("%04.0f\r\n",temp*100 );
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
			 the HAL_UART_TxCpltCallback could be implemented in the user file
	 */

	if (Uart1_Rx_Cnt >= 255) //锟斤拷锟斤拷卸锟�?
	{
		Uart1_Rx_Cnt = 0;
		memset(RxBuffer, 0x00, sizeof(RxBuffer));
		HAL_UART_Transmit(&huart1, (uint8_t *)"数据溢出", Uart1_Rx_Cnt, 0xFFFF);
	}
	else
	{
		RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer; //锟斤拷锟斤拷锟斤拷锟斤拷转锟斤拷

		if ((RxBuffer[Uart1_Rx_Cnt - 1] == 0x0A) && (RxBuffer[Uart1_Rx_Cnt - 2] == 0x0D)) //锟叫断斤拷锟斤拷�?
		{
			HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer, Uart1_Rx_Cnt, 0xFFFF); //锟斤拷锟秸碉拷锟斤拷锟斤拷息锟斤拷锟酵筹拷去
			while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)			//锟斤拷锟経ART锟斤拷锟酵斤拷锟斤拷
				Uart1_Rx_Cnt = 0;
			memset(RxBuffer, 0x00, sizeof(RxBuffer)); //锟斤拷锟斤拷锟斤拷锟�?
		}
	}

	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); //锟劫匡拷锟斤拷锟斤拷锟斤拷锟叫讹�?
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
