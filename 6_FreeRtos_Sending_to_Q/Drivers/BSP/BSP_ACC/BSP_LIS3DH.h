/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_ACC/BSP_LIS3DH.h $
* $Author: ghada.dhibi $
* $Revision: 459 $
* $Date: 2016-06-15 11:21:58 +0100 (mer., 15 juin 2016) $
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LIS3DH_H
#define __BSP_LIS3DH_H

#ifdef __cplusplus
extern "C" {
#endif   
  
  
#include <stdint.h>
#include "DRV_I2C.h"
#include "BSP_LIS3DH_Conf.h"
  
  typedef enum          LIS3DH_StatusEnum       BSP_LIS3DH_StatusEnum;
  typedef struct        LIS3DH_Value            BSP_LIS3DH_Value;
  
  enum LIS3DH_StatusEnum
  {
    LIS3DH_OK       = 0x00,
    LIS3DH_ERROR    = 0x01
  };
  
  typedef enum
  {
    LIS3DH_BUFFER1_FULL = 0x01,
    LIS3DH_BUFFER2_FULL = 0x00,
    
  }LIS3DH_BUFFER_STATUS;
  
  __packed struct LIS3DH_Value
  {
    int16_t X;
    int16_t Y;
    int16_t Z;
  };
  
  /* Public APIs */
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_Init(void);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_Config(void);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_SetDataRate(uint8_t dataRate);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_SetReg(uint8_t AddrRegister, uint8_t DataBuffer);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_GetReg(uint8_t AddrRegister, uint8_t *Datavalue);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_GetBuffer(uint8_t AddrRegister , uint8_t size, uint8_t *value);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_Detect(void);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadAccData(BSP_LIS3DH_Value* AccData, float LIS3DH_Sense);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_EnableFIFO(void);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadFIFO(BSP_LIS3DH_Value ACCE_DATA [], uint16_t* sample, float LIS3DH_Sense);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_EnableStream(void);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadStream(BSP_LIS3DH_Value ACCE_DATA [], float LIS3DH_Sense);
  BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadTempData(int8_t* TempData);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_LIS3DH_H */

/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/