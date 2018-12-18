/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_RTC/BSP_RTC.h $
* $Author: ghada.dhibi $
* $Revision: 577 $
* $Date: 2016-10-03 09:08:16 +0100 (lun., 03 oct. 2016) $
******************************************************************************
*/
#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "type.h"
   
typedef enum
{
  RTC_OK,
  RTC_ERROR
}BSP_RTC_Status;


BSP_RTC_Status BSP_RTC_Init(void);

BSP_RTC_Status BSP_RTC_SetTimeStamp(RTC_Date date);
BSP_RTC_Status BSP_RTC_GetTimeStamp(RTC_Date *date);

BSP_RTC_Status BSP_RTC_SetPosix(uint64_t date);
BSP_RTC_Status BSP_RTC_GetPosix(uint64_t *DatePosix);

BSP_RTC_Status BSP_RTC_SetPosixus(uint64_t value);
BSP_RTC_Status BSP_RTC_GetPosixus(uint64_t *DatePosix);

BSP_RTC_Status BSP_RTC_GetPosixMs(uint64_t *DatePosix);

BSP_RTC_Status BSP_RTC_GetCocoa(uint64_t *DateCocoa);
BSP_RTC_Status BSP_RTC_GetCocoaMs(uint64_t *DateCocoa);
BSP_RTC_Status BSP_RTC_GetCocoaus(uint64_t *DateCocoa);

BSP_RTC_Status BSP_RTC_SetCurrentTime( void);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_RTC_H */

/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/