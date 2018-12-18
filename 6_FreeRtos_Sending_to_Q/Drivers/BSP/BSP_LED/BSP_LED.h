/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_LED/BSP_LED.h $
* $Author: ghada.dhibi $
* $Revision: 443 $
* $Date: 2016-06-09 13:21:52 +0100 (jeu., 09 juin 2016) $
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
  /* Exported constants --------------------------------------------------------*/
//#define BLINK_LED
  
  typedef enum
  {
    LED_START_OK       = 0x00,
    LED_START_ERROR    = 0x01,
    LED_START_BUSY     = 0x02,
    LED_START_TIMEOUT  = 0x03,
    LED_STOP_OK        = 0x04,
    LED_STOP_ERROR     = 0x05,
    LED_STOP_BUSY      = 0x06,
    LED_STOP_TIMEOUT   = 0x07
  } BSP_LED_StatusTypeDef;
  typedef enum
  {
    LED_INIT_OK       = 0x00,
    LED_INIT_ERROR    = 0x01,
    LED_INIT_BUSY     = 0x02,
    LED_INIT_TIMEOUT  = 0x03
  }BSP_LED_INIT_StatusTypeDef;
  
  typedef enum
  {
    LED_GREEN = 0x01,
    LED_ORANGE = 0x02,
    LED_RED  = 0x03  
  }LED_TypeDef;
  
  typedef enum
  {
    LED_PIN_OFF = 0x01,
    LED_PIN_ON = 0x02,
  }LED_PinStateDef;
  
/* Exported function prototypes ----------------------------------------------*/
BSP_LED_INIT_StatusTypeDef BSP_LED_Init(void);
BSP_LED_StatusTypeDef BSP_LED_On(LED_TypeDef Led);
BSP_LED_StatusTypeDef BSP_LED_Off(LED_TypeDef Led);
BSP_LED_StatusTypeDef BSP_LED_Reverse(LED_TypeDef led);
LED_PinStateDef BSP_LED_GetLedState(LED_TypeDef Led);
void BSP_LED_FreqDutyCycleInit(LED_TypeDef Led, uint32_t frequency, uint32_t duty_cycle);
  /* Exported function  --------------------------------------------------------*/
  
  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_LED_H */
  /************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
  