/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$

******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_LED.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BSP_LED_GPIO_Init(void);

#if defined (BBNAUTE_BOARD)
#define BSP_LED_ON GPIO_PIN_RESET
#define BSP_LED_OFF GPIO_PIN_SET
#elif defined (CARDIONAUTE_BOARD)
#define BSP_LED_ON GPIO_PIN_SET
#define BSP_LED_OFF GPIO_PIN_RESET
#endif

/**
* @brief   Initialization of the LED status.
* @param   None.
* @retval  BSP LED status value.
*/
BSP_LED_INIT_StatusTypeDef BSP_LED_Init(void)
{ 
  BSP_LED_GPIO_Init();
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_RED);
  
  return LED_INIT_OK;
}

/**
* @brief  Turn LED ON.
* @param  Led: The specific LED.
* @retval BSP LED status value.
*/
BSP_LED_StatusTypeDef BSP_LED_On(LED_TypeDef Led)
{
  switch (Led)
  {
  case LED_GREEN:
    HAL_GPIO_WritePin(CONF_LED_PIN_GREEN_GPIO,CONF_LED_PIN_GREEN,BSP_LED_ON);
    break;
  case LED_ORANGE :
    HAL_GPIO_WritePin(CONF_LED_PIN_ORANGE_GPIO,CONF_LED_PIN_ORANGE,BSP_LED_ON);
    break;
  case LED_RED :
    HAL_GPIO_WritePin(CONF_LED_PIN_RED_GPIO,CONF_LED_PIN_RED,BSP_LED_ON);
    break;
  }
  
  return LED_START_OK;
}

/**
* @brief  Turn LED OFF.
* @param  Led: The specific LED.
* @retval BSP LED status value.
*/
BSP_LED_StatusTypeDef BSP_LED_Off(LED_TypeDef Led)
{
  switch (Led)
  {
  case LED_GREEN:
    HAL_GPIO_WritePin(CONF_LED_PIN_GREEN_GPIO,CONF_LED_PIN_GREEN,BSP_LED_OFF);
    break;
  case LED_ORANGE:
    HAL_GPIO_WritePin(CONF_LED_PIN_ORANGE_GPIO,CONF_LED_PIN_ORANGE,BSP_LED_OFF);
    break;
  case LED_RED:
    HAL_GPIO_WritePin(CONF_LED_PIN_RED_GPIO,CONF_LED_PIN_RED,BSP_LED_OFF);
    break;
  }
  
  return LED_STOP_OK;
}

/**
* @brief  Reverse the status led.
* @param  Led: The specific LED.
* @retval BSP LED status value.
*/
BSP_LED_StatusTypeDef BSP_LED_Reverse(LED_TypeDef led)
{
  LED_PinStateDef ledPinState;
  BSP_LED_StatusTypeDef retval;
  
  ledPinState = BSP_LED_GetLedState(led);
  
  if(ledPinState == LED_PIN_ON){
    retval = BSP_LED_Off(led);
  }
  else{
    retval = BSP_LED_On(led);
  }
  
  return retval;
}

/**
* @brief  Return Led State (ON/OFF).
* @param  Led: The specific LED.
* @retval BSP LED state value.
*/
LED_PinStateDef BSP_LED_GetLedState(LED_TypeDef Led)
{
  GPIO_PinState state;
  
  switch (Led)
  {
  case LED_GREEN:
    state = HAL_GPIO_ReadPin(CONF_LED_PIN_GREEN_GPIO,CONF_LED_PIN_GREEN);
    break;
  case LED_ORANGE:
    state = HAL_GPIO_ReadPin(CONF_LED_PIN_ORANGE_GPIO,CONF_LED_PIN_ORANGE);
    break;
  case LED_RED:
    state = HAL_GPIO_ReadPin(CONF_LED_PIN_RED_GPIO,CONF_LED_PIN_RED);
    break;
  }
  
  if(state == GPIO_PIN_SET){
    return LED_PIN_OFF;
  }
  else{
    return LED_PIN_ON;
  }
}

/**
* @brief  Configurate the frequency and the 
*         duty-cycle for the toggling LEDs.
* @param  frequency: the toggling frequency.
* @param  duty_cycle: the toggling duty-cycle.
* @retval None.
*/
void BSP_LED_FreqDutyCycleInit(LED_TypeDef Led, uint32_t frequency, uint32_t duty_cycle)
{
  TIM_HandleTypeDef    TimHandle;
  TIM_OC_InitTypeDef sConfig;
  
  TimHandle.Init.Prescaler         = (uint32_t)(SystemCoreClock / 10000) - 1;
  TimHandle.Init.Period            = 10000/frequency - 1; 
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.Pulse        = (TimHandle.Init.Period*duty_cycle/100);
  
  switch (Led)
  {
  case LED_GREEN:
    TimHandle.Instance = TIM1;
    HAL_TIM_PWM_Init(&TimHandle);
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    break;
    
  case LED_ORANGE:
    TimHandle.Instance = TIM5;
    HAL_TIM_PWM_Init(&TimHandle);
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
    break;
    
  case LED_RED:
    TimHandle.Instance = TIM3;
    HAL_TIM_PWM_Init(&TimHandle);
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4);
    break;
  }
}

/**
* @brief  Initialization of GPIO.
* @param  None.
* @retval None.
*/
void BSP_LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  CONF_LED_PIN_GREEN_GPIO_CLK_ENABLE();
  CONF_LED_PIN_ORANGE_GPIO_CLK_ENABLE(); 
  CONF_LED_PIN_RED_GPIO_CLK_ENABLE();
  CONF_LED_RED_TIM_CLK_ENABLE();
  CONF_LED_ORANGE_TIM_CLK_ENABLE();
  CONF_LED_GREEN_TIM_CLK_ENABLE();
  
#ifdef BLINK_LED
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
#else 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
#endif
  
  /* Configure LED GREEN pin*/
  GPIO_InitStruct.Pin = CONF_LED_PIN_GREEN;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(CONF_LED_PIN_GREEN_GPIO, &GPIO_InitStruct);
  
  /* Configure LED ORANGE pin*/
  GPIO_InitStruct.Pin = CONF_LED_PIN_ORANGE;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
  HAL_GPIO_Init(CONF_LED_PIN_ORANGE_GPIO, &GPIO_InitStruct);
  
  /* Configure LED RED pin*/
  GPIO_InitStruct.Pin = CONF_LED_PIN_RED;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(CONF_LED_PIN_RED_GPIO, &GPIO_InitStruct);
}
