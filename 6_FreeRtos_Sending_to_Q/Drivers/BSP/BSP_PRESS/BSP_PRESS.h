/**
  ******************************************************************************
  * $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_PRESS/BSP_PRESS.h $
  * $Author: ghada.dhibi $
  * $Revision: 280 $
  * $Date: 2016-04-27 18:15:29 +0100 (mer., 27 avr. 2016) $
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PRESS_H
#define __BSP_PRESS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DRV_ADC.h"
/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported function  --------------------------------------------------------*/
float BSP_PRESS_GetValue(void);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_PRESS_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
