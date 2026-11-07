/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t buf[256];
int16_t ch[16];
uint8_t start_byte;
uint8_t flag_byte;
uint8_t stop_byte;
typedef struct{
  int16_t rocker_l_;
  int16_t rocker_l1;
  int16_t rocker_r_;
  int16_t rocker_r1;
  int16_t SWA;
  int16_t SWB;
  int16_t SWC;
  int16_t SWD;
  int16_t VRA;
  int16_t VRB;
  uint8_t error_protect;
  uint8_t byte_lose;
} RC_ctrl_t;
static RC_ctrl_t rcdata;
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, buf, 25);
  __HAL_DMA_DISABLE_IT(&huart3, DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{ 
  buftoch(buf, ch);
  if (huart == &huart3)
  {
    start_byte = buf[0];
    flag_byte = buf[23];
    stop_byte = buf[24];
    buftoch(buf, ch);
    rcdata.rocker_r_ = ch[0];
    rcdata.rocker_r1 = ch[1];
    rcdata.rocker_l_ = ch[2];
    rcdata.rocker_l1 = ch[3];
    rcdata.SWA = ch[4];
    rcdata.SWB = ch[5];
    rcdata.SWC = ch[6];
    rcdata.SWD = ch[7];
    rcdata.VRA = ch[8];
    rcdata.VRB = ch[9];
    rcdata.error_protect = flag_byte >> 3 & 0x01;
    rcdata.byte_lose = flag_byte >> 2 & 0x01;
  }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, buf, 25);
  __HAL_DMA_DISABLE_IT(&huart3, DMA_IT_HT);
}
void buftoch(uint8_t * bbuf[], int16_t *cch[]){
  *cch[0] = (((int16_t)*bbuf[1] >> 0 | (int16_t)*bbuf[2] << 8) & 0x07FF) - 1024;
  *cch[1] = (((int16_t)*bbuf[2] >> 3 | (int16_t)*bbuf[3] << 5) & 0x07FF) - 1024;
  *cch[2] = (((int16_t)*bbuf[3] >> 6 | (int16_t)*bbuf[4] << 2 | (int16_t)*bbuf[5] << 10) & 0x07FF) - 1024;
  *cch[3] = (((int16_t)*bbuf[5] >> 1 | (int16_t)*bbuf[6] << 7) & 0x07FF) - 1024;
  *cch[4] = (((int16_t)*bbuf[6] >> 4 | (int16_t)*bbuf[7] << 4) & 0x07FF) - 1024;
  *cch[5] = (((int16_t)*bbuf[7] >> 7 | (int16_t)*bbuf[8] << 1 | (int16_t)*bbuf[9] << 9) & 0x07FF) - 1024;
  *cch[6] = (((int16_t)*bbuf[9] >> 2 | (int16_t)*bbuf[10] << 6) & 0x07FF) - 1024;
  *cch[7] = (((int16_t)*bbuf[10] >> 5 | (int16_t)*bbuf[11] << 3) & 0x07FF) - 1024;
  *cch[8] = (((int16_t)*bbuf[12] >> 0 | (int16_t)*bbuf[13] << 8) & 0x07FF) - 1024;
  *cch[9] = (((int16_t)*bbuf[13] >> 3 | (int16_t)*bbuf[14] << 5) & 0x07FF) - 1024;
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, buf, 25);
    __HAL_DMA_DISABLE_IT(&huart3, DMA_IT_HT);
  }
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
