/**
  ******************************************************************************
  * $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_WIFI/BSP_WIFI.h $
  * $Author: ghada.dhibi $
  * $Revision: 396 $
  * $Date: 2016-05-27 10:38:51 +0100 (ven., 27 mai 2016) $
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_WIFI_H
#define __BSP_WIFI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "m2m_wifi.h"
#include "nmasic.h"
#include "m2m_hif.h"
#include "m2m_ota.h"
#include "nm_common.h"
#include "m2m_wifi.h"
#include "socket.h"
#include <string.h>
/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define BSP_WIFI_Init()         nm_bsp_init()
#define BSP_WIFI_Reset()        nm_bsp_reset()
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported function  --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__BSP_WIFI_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
