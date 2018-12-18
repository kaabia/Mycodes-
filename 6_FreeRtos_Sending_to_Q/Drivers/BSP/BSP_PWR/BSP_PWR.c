/**
  ******************************************************************************
  * $HeadURL$
  * $Author$
  * $Revision$
  * $Date$
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "BSP_PWR.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
#ifdef BBNAUTE_BOARD
/**
* @brief  Hardware ressources configuration for the power peripheric
* @param  None.
* @retval None.
*/

void BSP_PWR_P3v1P1v8Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  CONF_PWR_PIN_EN_P3V1_P1V8_GPIO_CLK_ENABLE();
  /*Configure GPIO pin : A3_Pin */
  GPIO_InitStruct.Pin = CONF_PWR_PIN_EN_P3V1_P1V8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(CONF_PWR_PIN_EN_P3V1_P1V8_GPIO, &GPIO_InitStruct);
}


/**
* @brief  Enable the power peripheric
* @param  None.
* @retval None.
*/
void BSP_PWR_P3v1P1v8Enable(void)
{
  HAL_GPIO_WritePin(CONF_PWR_PIN_EN_P3V1_P1V8_GPIO,CONF_PWR_PIN_EN_P3V1_P1V8,GPIO_PIN_SET);
}

/**
* @brief  Get the P4VF value
* @param  None.
* @retval P4VF value
*/
float BSP_PWR_GetP4VFValue(void)
{
  uint32_t adcval[2];
  float retval;
  DRV_ADCPWR_GetVal(adcval);
  retval = 1.465*adcval[0];
  
  return retval;
}

/**
* @brief  Get the VBAT value
* @param  None.
* @retval VBAT value
*/
float BSP_PWR_GetVBATFValue(void)
{
  uint32_t adcval[2];
  float retval;  
  DRV_ADCPWR_GetVal(adcval);
  retval = 0.733*adcval[1];
  
  return retval;
}

#elif defined CARDIONAUTE_BOARD

/**
* @brief  Get the VBAT value
* @param  None.
* @retval VBAT value
*/
float BSP_PWR_GetVBATValue(void)
{
  uint32_t adcval[2];
  float retval;
  adcval[0] = DRV_ADCPWR_GetVal();
  retval = 1.669*adcval[0]; // FBA : Coef 1.663
  
  return retval;
}
#endif
/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/
