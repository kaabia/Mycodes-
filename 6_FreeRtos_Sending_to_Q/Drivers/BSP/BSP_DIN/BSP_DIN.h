/**
  ******************************************************************************
  * $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_DIN/BSP_DIN.h $
  * $Author: ghada.dhibi $
  * $Revision: 280 $
  * $Date: 2016-04-27 18:15:29 +0100 (mer., 27 avr. 2016) $
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DIN_H
#define __BSP_DIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
   #include "stm32l4xx_hal.h"
/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
#ifdef BBNAUTE_BOARD 
   extern TIM_HandleTypeDef    TimDINHandle;
#endif
/* Exported function prototypes ----------------------------------------------*/
/* Exported function  --------------------------------------------------------*/

void BSP_DIN_Init(void);
#ifdef CARDIONAUTE_BOARD  
void BSP_DIN_DeInit(void);
void BSP_DIN_Callback(void);
void BSP_DIN_EnableInterrupt (void);
void BSP_DIN_DisableInterrupt (void);
#endif
#ifdef BBNAUTE_BOARD 
void BSP_DIN_Reset(void);
void BSP_DIN_PressDelayReset(void);
void BSP_DIN_Start(void) ;
#endif
#ifdef __cplusplus
}
#endif

#endif /*__BSP_DIN_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
