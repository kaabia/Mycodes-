/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_TEMP/BSP_TEMP.h $
* $Author: ghada.dhibi $
* $Revision: 448 $
* $Date: 2016-06-10 15:14:30 +0100 (ven., 10 juin 2016) $
******************************************************************************
*/
#ifndef __BSP_TEMP_H
#define __BSP_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
  
#include "DRV_I2C.h"  
  
  /* Private define ------------------------------------------------------------*/
  
#define TEMP_ID_W                       0x90   //ID Device write mode 1001 0000 (ADD0 to GND)
#define TEMP_ID_R                       0x91   //ID Device read mode  1001 0001 (ADD0 to GND)
#define TEMP_ID_GENRAL_CALL             0x00   //General Call address 0
  
#define TEMP_RESET                      0x06
#define MASK_SHUTDOWN                   0x01
#define MASK_ONESHOT                    0x80
  
  
    
  typedef enum TEMP_AddressEnum         BSP_TEMP_AddressEnum;
  typedef enum TEMP_EmModeEnum          BSP_TEMP_EmModeEnum;
  typedef enum TEMP_ConvRateEnum        BSP_TEMP_ConvRateEnum;
  typedef enum TEMP_SdModeEnum          BSP_TEMP_SdModeEnum;
  typedef enum TEMP_TmModeEnum          BSP_TEMP_TmModeEnum;
  typedef enum TEMP_PolModeEnum         BSP_TEMP_PolModeEnum;
  typedef enum TEMP_FaultQueueEnum      BSP_TEMP_FaultQueueEnum;
  typedef enum TEMP_OsModeEnum          BSP_TEMP_OsModeEnum;
  typedef enum TEMP_StatusEnum          BSP_TEMP_StatusEnum;
  typedef struct TEMP_ConfigurationStruct BSP_TEMP_ConfigurationStruct;	
  
  /* Exported types ----------------------------------------------------------*/ 
  
  enum TEMP_AddressEnum
  {
    TEMP_REG = 0x00,   //Temperature Register
    MODE_CONF,       //Mode Configuration Register
    TEMP_LOW_REG,      //LOW Temperature Register
    TEMP_HIGH_REG      //HIGH Temperature Register
  };
  
  enum TEMP_EmModeEnum
  {
    NORMAL_MODE=0x00,
    EXTENDED_MODE=0x10
  };
  
  enum TEMP_ConvRateEnum
  {
    RATE1 = 0x00,   //Conversion rate = 0.25Hz
    RATE2,          //Conversion rate = 1Hz
    RATE3,          //Conversion rate = 4Hz (default)
    RATE4           //Conversion rate = 8Hz
  };
  
  enum TEMP_SdModeEnum
  {
    WAKE_MODE=0x00,
    SHUTDOWN_MODE=0x0100
  };
  
  enum TEMP_TmModeEnum
  {
    COMPARATOR_MODE=0x00,
    INTERRUPT_MODE=0x0200
  };
  
  enum TEMP_PolModeEnum
  {
    INACTIVE_POLARITY=0x00,
    ACTIVE_POLARITY=0x0400
  };
  
  enum TEMP_FaultQueueEnum
  {
    CF1 = 0x00, //Consecutive faults = 1 (to generate an alert)
    CF2,        //Consecutive faults = 2 (to generate an alert)
    CF3,        //Consecutive faults = 4 (to generate an alert)
    CF4         //Consecutive faults = 6 (to generate an alert)
  };
  
  enum TEMP_OsModeEnum
  {
    INACTIVE_OS=0x00,
    ACTIVE_OS=0x8000
  };
  
  enum TEMP_StatusEnum
  {
    TEMP_OK       = 0x00,
    TEMP_ERROR    = 0x01
  };
  
  struct TEMP_ConfigurationStruct
  {
    BSP_TEMP_EmModeEnum     Extended_mode;
    BSP_TEMP_ConvRateEnum   Conversion_rate;
    BSP_TEMP_SdModeEnum     Shutdown;
    BSP_TEMP_TmModeEnum     Thermostat;
    BSP_TEMP_PolModeEnum    Polarity;
    BSP_TEMP_FaultQueueEnum Consecutive_faults;
    BSP_TEMP_OsModeEnum     One_shot;
  };	
  
  /* Exported function prototypes ----------------------------------------------*/
  BSP_TEMP_StatusEnum BSP_TEMP_Init(void);
  
  BSP_TEMP_StatusEnum BSP_TEMP_Shutdown();
  BSP_TEMP_StatusEnum BSP_TEMP_WakeUp();
  BSP_TEMP_StatusEnum BSP_TEMP_OneShot();
  BSP_TEMP_StatusEnum BSP_TEMP_ResetGeneralCall();
  
  BSP_TEMP_StatusEnum BSP_TEMP_SetRegBuffer(BSP_TEMP_AddressEnum Register, uint16_t DataBuffer);
  BSP_TEMP_StatusEnum BSP_TEMP_SetRegMSB(BSP_TEMP_AddressEnum Register, uint8_t MSB);
  
  BSP_TEMP_StatusEnum BSP_TEMP_GetRegBuffer(BSP_TEMP_AddressEnum Register, uint16_t *value);
  BSP_TEMP_StatusEnum BSP_TEMP_GetRegMSB(BSP_TEMP_AddressEnum Register, uint8_t* MSBvalue); 
  
  BSP_TEMP_StatusEnum BSP_TEMP_GetModuleConfig(uint16_t* configValue);
  BSP_TEMP_StatusEnum BSP_TEMP_GetMSBConfig(uint8_t* configValueMSB);
  
  float BSP_TEMP_ReadTemperatureC();
  float BSP_TEMP_ReadTemperatureF();
  
  BSP_TEMP_StatusEnum BSP_TEMP_SetConfiguration(BSP_TEMP_ConfigurationStruct* config);
  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_TEMP_H */

/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/