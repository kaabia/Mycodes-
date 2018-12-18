/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_ADS1293.h"
#include "BCP.h"


/**
* @brief  Sinus Injection managemet 
* @retval 
*/


#if defined( ADS_ENABLE_SINUS_INJECT ) || defined( ADS_ENABLE_MASS_TEST_BENCH_SINUS_INJECT )  

#define ADS_SINUS_INJECT_SAMPLES_COUNT 250 
#define ADS_SINUS_INJECT_CONFIG_COUNT 24

static bool xEnableSinusInjection = false;

void BSP_ADS1293_Detect_Test_Mode(void);


const xADSSinusInjectConfig_t xADSSinusInjectConfig[ADS_SINUS_INJECT_CONFIG_COUNT] = 
{
  {0x07, 0x02, GPIOC, GPIO_PIN_1, },
  {0x07, 0x00, GPIOC, GPIO_PIN_1, },
  {0x07, 0x04, GPIOC, GPIO_PIN_1, },
  {0x07, 0x00, GPIOC, GPIO_PIN_1, },
  {0x07, 0x08, GPIOC, GPIO_PIN_1, },
  {0x07, 0x00, GPIOC, GPIO_PIN_1, },
  
  {0x07, 0x01, GPIOC, GPIO_PIN_2, },
  {0x07, 0x00, GPIOC, GPIO_PIN_2, },
  {0x07, 0x02, GPIOC, GPIO_PIN_2, },
  {0x07, 0x00, GPIOC, GPIO_PIN_2, },
  {0x07, 0x04, GPIOC, GPIO_PIN_2, },
  {0x07, 0x00, GPIOC, GPIO_PIN_2, },
  
  {0x07, 0x01, GPIOC, GPIO_PIN_3, },
  {0x07, 0x00, GPIOC, GPIO_PIN_3, },
  {0x07, 0x02, GPIOC, GPIO_PIN_3, },
  {0x07, 0x00, GPIOC, GPIO_PIN_3, },
  {0x07, 0x04, GPIOC, GPIO_PIN_3, },
  {0x07, 0x00, GPIOC, GPIO_PIN_3, },
  
  {0x07, 0x01, GPIOC, GPIO_PIN_4, },
  {0x07, 0x00, GPIOC, GPIO_PIN_4, },
  {0x07, 0x02, GPIOC, GPIO_PIN_4, },
  {0x07, 0x00, GPIOC, GPIO_PIN_4, },
  {0x07, 0x04, GPIOC, GPIO_PIN_4, },
  {0x07, 0x00, GPIOC, GPIO_PIN_4, },
  
};

void BSP_ADS1293_Detect_Test_Mode(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = CONF_ONE_WIRE_PIN;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CONF_ONE_WIRE_PORT, &GPIO_InitStruct);
  
  HAL_Delay(1);
  
  if(HAL_GPIO_ReadPin(CONF_ONE_WIRE_PORT, CONF_ONE_WIRE_PIN))
  {
    xEnableSinusInjection = false;
  }
  else
  {
    xEnableSinusInjection = true;
  }
  
}

/**
* @brief  
* @param  
* @retval None
*/

void ADS1293_ManageSinusInjection(void)
{
  
  #if defined( ADS_ENABLE_SINUS_INJECT ) 
  
  static uint32_t u32ADSCounter = 0;
  static uint8_t  u8ADSConfigID = 0;

  
  if( xEnableSinusInjection == true )
  {
    if( u32ADSCounter++ >= ADS_SINUS_INJECT_SAMPLES_COUNT ) 
    {
      BSP_ADS1293_SPIWrite(xADSSinusInjectConfig[u8ADSConfigID].registerAdress, (REG8*)&xADSSinusInjectConfig[u8ADSConfigID].registerValue, xADSSinusInjectConfig[u8ADSConfigID].GPIOx, xADSSinusInjectConfig[u8ADSConfigID].GPIO_Pin, 1);
      
      u8ADSConfigID++;
      if( u8ADSConfigID >= ADS_SINUS_INJECT_CONFIG_COUNT )
      {
        u8ADSConfigID = 0;
      }
      u32ADSCounter = 0;
    }
  }
  
#elif defined( ADS_ENABLE_MASS_TEST_BENCH_SINUS_INJECT )      

    static uint32_t u32ADSCounter = 0;
    REG8 u8Value;
    if ( u32ADSCounter == 0 )
    {
        u8Value = 0x02;
        BSP_ADS1293_SPIWrite(0x07, &u8Value, GPIOC, GPIO_PIN_1, 1);      
        
    }
    /*
    else if( u32ADSCounter >= ADS_SINUS_INJECT_SAMPLES_COUNT ) 
    {
              u8Value = 0x00;
      BSP_ADS1293_SPIWrite(0x07, &u8Value, GPIOC, GPIO_PIN_1, 1);
    }
    */    
    else
    {
    }
    
    if( u32ADSCounter++ >=  ADS_SINUS_INJECT_SAMPLES_COUNT ) 
    {
      u32ADSCounter = 0;
    }
    
#endif

}


#endif
