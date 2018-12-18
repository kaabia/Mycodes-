/**
  ******************************************************************************
  * $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_PWR/BSP_PWR.h $
  * $Author: ghada.dhibi $
  * $Revision: 448 $
  * $Date: 2016-06-10 15:14:30 +0100 (ven., 10 juin 2016) $
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_PWR_H
#define __BSP_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DRV_ADC.h"
/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
//#define P4VF            1
//#define VBAT            2
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported function  --------------------------------------------------------*/
#ifdef BBNAUTE_BOARD 
   void BSP_PWR_P3v1P1v8Init(void);
   void BSP_PWR_P3v1P1v8Enable(void);
   float BSP_PWR_GetP4VFValue(void);
   float BSP_PWR_GetVBATFValue(void);
#elif defined CARDIONAUTE_BOARD
   float BSP_PWR_GetVBATValue(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /*__BSP_PWR_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
