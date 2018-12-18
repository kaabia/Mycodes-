
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "BMP280.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void Read_Press(void);
static void Read_Temp(void);
static int32_t bmp280_compensate_T_int32(int32_t adc_T) ;


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint16_t DeviceAdress= 0xEC;   // SLAVE ADRESS
uint8_t I2cData[2]; 
uint8_t Temp[3]; 
uint8_t Press[3];  
uint32_t TEMPERATURE ;
uint32_t PRESSION  ;
BMP280_S32_t t_fine;
uint32_t T_deg ;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock  40 MHz */
    SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  /* USER CODE BEGIN 2 */
  
  // verify connection to device ... led 5 is on when device is ready 
  if (HAL_I2C_IsDeviceReady(&hi2c1,DeviceAdress,2,100) == HAL_OK){BSP_LED_On(LED5); }
  
 // from ctrl_meas register : Select normal power mode [01]  pressure and temp oversampling *1 => 00100111 = 0x27
 I2cData[0] =  0x27 ; 
 HAL_I2C_Mem_Write(&hi2c1, DeviceAdress ,ctrl_meas,I2C_MEMADD_SIZE_8BIT, &I2cData[0] , I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 
 
 // READ dig_T1, dig_T2, AND  dig_T3
 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x88,I2C_MEMADD_SIZE_8BIT, &I2cData[0], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x89,I2C_MEMADD_SIZE_8BIT, &I2cData[1], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 dig_T1 = ~( I2cData[0] |I2cData[1]<<8)+0x01;
 
  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x8A,I2C_MEMADD_SIZE_8BIT, &I2cData[0], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x8B,I2C_MEMADD_SIZE_8BIT, &I2cData[1], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 dig_T2 = ~( I2cData[0] |I2cData[1]<<8)+0x01;
 
  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x8C,I2C_MEMADD_SIZE_8BIT, &I2cData[0], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,0x8D,I2C_MEMADD_SIZE_8BIT, &I2cData[1], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 dig_T3 = ~( I2cData[0] |I2cData[1]<<8) +0x01;
 

 
  
  
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
    Read_Press();
    Read_Temp();
    HAL_Delay(200);
    T_deg = bmp280_compensate_T_int32(TEMPERATURE);
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00301347;  //0x00909BEB //0x00301347
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

}
/* USER CODE BEGIN 4 */
void Read_Press(){
    
//*************************** Pressure ************************** //

 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,press_msb,I2C_MEMADD_SIZE_8BIT, &Press[0], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 
 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,press_lsb,I2C_MEMADD_SIZE_8BIT, &Press[1], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
  
 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,press_xlsb,I2C_MEMADD_SIZE_8BIT, &Press[2], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 
 PRESSION =  (Press[2] |  Press[1]<<4  | Press[0]<<12 ) ; 

}

void Read_Temp(){
 
 //*************************** TEMPERATURE ************************** //

  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,temp_msb,I2C_MEMADD_SIZE_8BIT,&Temp[0], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,temp_lsb,I2C_MEMADD_SIZE_8BIT, &Temp[1], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
  HAL_I2C_Mem_Read(&hi2c1, DeviceAdress,temp_xlsb,I2C_MEMADD_SIZE_8BIT, &Temp[2], I2C_MEMADD_SIZE_8BIT, 100);
 while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)  {}
 TEMPERATURE =  (Temp[0] |  Temp[1]<<4  | Temp[2]<<12 ) ; 
 
} 
/* USER CODE BEGIN 4 */
int32_t bmp280_compensate_T_int32(int32_t adc_T)
{
int32_t var1, var2, T;
var1 = (((adc_T>>3)- ((int32_t)dig_T1<<1)) * ((int32_t)dig_T2))>>11;
var2 =( ((((adc_T>>4) - ((int32_t) dig_T1)) * ((adc_T>>4)- ((int32_t)dig_T1) ))>>12) * ((int32_t)dig_T3) ) >>14;
t_fine = var1 + var2;
T = (t_fine * 5 + 128) >> 8;
return T;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
