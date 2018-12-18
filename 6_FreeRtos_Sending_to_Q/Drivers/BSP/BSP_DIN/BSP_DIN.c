/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_DIN.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef BBNAUTE_BOARD 
TIM_HandleTypeDef    TimDINHandle;
#endif

/* Private function prototypes -----------------------------------------------*/
#ifdef BBNAUTE_BOARD 
void BSP_DIN_TIM_Init(void);
#endif
void BSP_DIN_GPIO_Init(void);

/* Private function  ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



void BSP_DIN_Init(void)
{
  BSP_DIN_GPIO_Init();
#ifdef BBNAUTE_BOARD 
  BSP_DIN_TIM_Init(); 
#endif
}

#ifdef CARDIONAUTE_BOARD  
void BSP_DIN_DeInit(void)
{
  /* Peripheral clock disable */   
  CONF_1W_MEM_GPIO_CLK_ENABLE();
  
  HAL_GPIO_DeInit(CONF_1W_MEM_GPIO, CONF_1W_MEM_PIN);
  
  /* Disable the NVIC for GPIO */
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  
}

__weak void BSP_DIN_Callback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
  the HAL_USART_MspInit can be implemented in the user file
  */
}

void BSP_DIN_EnableInterrupt (void)
{
  /* Enable and set EXTI line 0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn , 6, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void BSP_DIN_DisableInterrupt (void)
{
  HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
  /* Disable the NVIC for GPIO */
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}

#endif

#ifdef BBNAUTE_BOARD 
void BSP_DIN_Start(void) 
{
  HAL_TIM_Base_Start_IT(&TimDINHandle);
}
#endif

void BSP_DIN_GPIO_Init(void)
{
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
#ifdef CARDIONAUTE_BOARD  
  
  /* Enable GPIOA clock */
  CONF_1W_MEM_GPIO_CLK_ENABLE();
  
  /* Configure PA.0 pin as input floating */
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;
  GPIO_InitStruct.Pin = CONF_1W_MEM_PIN;
  HAL_GPIO_Init(CONF_1W_MEM_GPIO, &GPIO_InitStruct);
  
#endif 
  
#ifdef BBNAUTE_BOARD 
  
  /* GPIO Ports Clock Enable */
  CONF_DIN_PIN_RSTN_GPIO_CLK_ENABLE();
  
  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = CONF_DIN_PIN_RSTN;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CONF_DIN_PIN_RSTN_GPIO, &GPIO_InitStruct);
#endif
}

#ifdef BBNAUTE_BOARD 
void BSP_DIN_TIM_Init(void)
{
  uint32_t uwPrescalerValue = 0;
  uint32_t PCLK1_freq=0;
  
  /* Compute the prescaler value to have TIMDin counter clock equal to 10000 Hz */
  PCLK1_freq = HAL_RCC_GetPCLK1Freq();
  //  printf ("PCLK1_freq is: %d,\n", PCLK1_freq); 
  uwPrescalerValue = (uint32_t)(PCLK1_freq / 10000) - 1;
  //  printf ("uwPrescalerValue: %d,\n", uwPrescalerValue); 
  
  /* Set TIMx instance */
  TimDINHandle.Instance               = TIM_DIN;
  TimDINHandle.Init.Period            = 10000/25 - 1; //Update rate = 25 Hz ==> 40ms timer duration
  TimDINHandle.Init.Prescaler         = uwPrescalerValue;
  TimDINHandle.Init.ClockDivision     = 0;
  TimDINHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimDINHandle.Init.RepetitionCounter = 0;
  
  HAL_TIM_Base_Init(&TimDINHandle);
  
}

void BSP_DIN_PressDelayReset()
{
  //  uint32_t Reset_Timeout ;
  static uint16_t cmpt_debounce = 0;
  static uint16_t cmpt_delay = 0;
  static uint16_t confirm_press = 0;
  //   Reset_Timeout =  mConfGet(DIN,ResetTimeout);
  if (HAL_GPIO_ReadPin(CONF_DIN_PIN_RSTN_GPIO,CONF_DIN_PIN_RSTN)== GPIO_PIN_RESET)
  {
    if (cmpt_debounce<3)
    {
      cmpt_debounce++;
    }
    else
    {
      confirm_press =1;
    }
  }
  else
  {
    if (cmpt_debounce > 0)
    {
      cmpt_debounce --;
    }
    if (cmpt_debounce == 0)
    {
      confirm_press =0;
    }
  }
  
  if (confirm_press == 1)
  {
    cmpt_delay ++;
    
    if (cmpt_delay >= DINResetTimeout)
    {
      BSP_DIN_Reset();
    }
  }
  else
  {
    cmpt_delay =0;
  }
}



void BSP_DIN_Reset ()
{
  HAL_NVIC_SystemReset();
}

#endif

/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/