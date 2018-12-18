/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_SPO2/BSP_MAX30101.h $
* $Author: ghada.dhibi $
* $Revision: 448 $
* $Date: 2016-06-10 15:14:30 +0100 (ven., 10 juin 2016) $
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_MAX30101_H
#define __BSP_MAX30101_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#include "DRV_I2C.h" 
  /* Exported types ----------------------------------------------------------*/  
  typedef enum          MAX30101_StatusEnum           BSP_MAX30101_StatusEnum;
  typedef struct        MAX30101_LEDStruct            BSP_MAX30101_LEDStruct;
  typedef enum          MAX30101_BitStatusEnum        BSP_MAX30101_BitStatusEnum;
  typedef struct        MAX30101_InterruptStruct      BSP_MAX30101_InterruptStruct;
  typedef enum          MAX30101_SampleAverageEnum    BSP_MAX30101_SampleAverageEnum;
  typedef enum          MAX30101_FIFORollOverEnum     BSP_MAX30101_FIFORollOverEnum;
  typedef enum          MAX30101_FIFOAFullEnum        BSP_MAX30101_FIFOAFullEnum;
  typedef enum          MAX30101_ModeEnum             BSP_MAX30101_ModeEnum;
  typedef enum          MAX30101_ADCFullScaleEnum     BSP_MAX30101_ADCFullScaleEnum;
  typedef enum          MAX30101_SampleRateEnum       BSP_MAX30101_SampleRateEnum;
  typedef enum          MAX30101_PulseWidthEnum       BSP_MAX30101_PulseWidthEnum;
  typedef struct        MAX30101_LEDCurrentStruct     BSP_MAX30101_LEDCurrentStruct;
  typedef enum          MAX30101_SlotEnum             BSP_MAX30101_SlotEnum;
  typedef struct        MAX30101_SlotControlStruct    BSP_MAX30101_SlotControlStruct;
  typedef struct        MAX30101_HandleStruct         BSP_MAX30101_HandleStruct;
  typedef struct        MAX30101_UpdateStruct         BSP_MAX30101_UpdateStruct;
  typedef enum          MAX30101_InterruptEnum        BSP_MAX30101_InterruptEnum;
  typedef enum          MAX30101_AddressEnum          BSP_MAX30101_AddressEnum;
  
    enum MAX30101_StatusEnum
  {
    MAX30101_OK       = 0x00,
    MAX30101_ERROR    = 0x01
  };
  
  struct MAX30101_LEDStruct
  {
    uint32_t    Red_LED;
    uint32_t    IR_LED;
    uint32_t    Green_LED;
  };
  
  enum MAX30101_BitStatusEnum
  {
    BIT_RESET       = 0x00,
    BIT_SET         = 0x01
  };
  
  struct MAX30101_InterruptStruct
  {
    BSP_MAX30101_BitStatusEnum     A_FULL; 
    BSP_MAX30101_BitStatusEnum     PPG_DRY;
    BSP_MAX30101_BitStatusEnum     ALC_OVF;
    BSP_MAX30101_BitStatusEnum     PROX_INT;
    BSP_MAX30101_BitStatusEnum     DIE_TEMP_DRY;
  };
  
  enum MAX30101_SampleAverageEnum
  {
    SA_1  = 0x00,       // 1 SAMPLES AVERAGED PER FIFO SAMPLE
    SA_2  = 0x20,       // 2 SAMPLES AVERAGED PER FIFO SAMPLE
    SA_4  = 0x40,       // 4 SAMPLES AVERAGED PER FIFO SAMPLE
    SA_8  = 0x60,       // 8 SAMPLES AVERAGED PER FIFO SAMPLE
    SA_16 = 0x80,       // 16 SAMPLES AVERAGED PER FIFO SAMPLE
    SA_32 = 0xA0        // 32 SAMPLES AVERAGED PER FIFO SAMPLE
  }; //FIFO Config Register Value 
  
  enum MAX30101_FIFORollOverEnum
  {
    FIFO_ROLLOVER_DIS   = 0x00,       // FIFO_ROLLOVER_DISABLE
    FIFO_ROLLOVER_EN    = 0x10        // FIFO_ROLLOVER_ENABLE
  }; //FIFO Config Register Value 
  
  enum MAX30101_FIFOAFullEnum
  {
    A_FULL_0 = 0x00, // 0 SAMPLES IN THE FIFO
    A_FULL_1 ,// 1 SAMPLES IN THE FIFO
    A_FULL_2 ,// 2 SAMPLES IN THE FIFO
    A_FULL_3 ,// 3 SAMPLES IN THE FIFO
    A_FULL_4 ,// 4 SAMPLES IN THE FIFO
    A_FULL_5 ,// 5 SAMPLES IN THE FIFO
    A_FULL_6 ,// 6 SAMPLES IN THE FIFO
    A_FULL_7 ,// 7 SAMPLES IN THE FIFO
    A_FULL_8 ,// 8 SAMPLES IN THE FIFO
    A_FULL_9 ,// 9 SAMPLES IN THE FIFO
    A_FULL_10,// 10 SAMPLES IN THE FIFO
    A_FULL_11,// 11 SAMPLES IN THE FIFO
    A_FULL_12,// 12 SAMPLES IN THE FIFO
    A_FULL_13,// 13 SAMPLES IN THE FIFO
    A_FULL_14,// 14 SAMPLES IN THE FIFO 
    A_FULL_15 // 15 SAMPLES IN THE FIFO
  }; // FIFO Config Register Value
  
  enum MAX30101_ModeEnum
  {
    UNUSED_0x00 = 0x00,
    UNUSED_0x01, 
    HR_Mode,    //= 0x02,
    SPO2_Mode,  //= 0x03,
    UNUSED_0x04,
    UNUSED_0x05,
    UNUSED_0x06,
    MultiMode  //= 0x07
  }; // MODE_CONFIG Register Value 
  
  enum MAX30101_ADCFullScaleEnum
  {
    FS_2048  = 0x00,    //FULL SCALE 2048 nA
    FS_4096  = 0x20,    //FULL SCALE 4096 nA
    FS_8192  = 0x40,    //FULL SCALE 8192 nA
    FS_16384 = 0x60     //FULL SCALE 16384 nA
  }; //MAX30101 Config Register Value 
  
  enum MAX30101_SampleRateEnum
  {
    SR_50   = 0x00,     // 50 SAMPLES PER SECOND
    SR_100  = 0x04,     // 100 SAMPLES PER SECOND
    SR_200  = 0x08,     // 200 SAMPLES PER SECOND
    SR_400  = 0x0C,     // 400 SAMPLES PER SECOND
    SR_800  = 0x10,     // 800 SAMPLES PER SECOND
    SR_1000 = 0x14,     // 1000 SAMPLES PER SECOND
    SR_1600 = 0x18,     // 1600 SAMPLES PER SECOND
    SR_3200 = 0x1C      // 3200 SAMPLES PER SECOND
  }; //MAX30101 Config Register Value 
  
  enum MAX30101_PulseWidthEnum
  {
    PW_69  = 0x00, // 15-bit ADC resolution
    PW_118 = 0x01, // 16-bit ADC resolution
    PW_215 = 0x02, // 17-bit ADC resolution
    PW_411 = 0x03  // 18-bit ADC resolution
  }; //MAX30101 Config Register Value 
  
  struct MAX30101_LEDCurrentStruct
  { // LED Config
    uint8_t             RED_LED_CurrentControl;
    uint8_t             IR_LED_CurrentControl;
    uint8_t             GREEN_LED_CurrentControl;
    uint8_t             LED_PROX_CurrentControl;
  };
  
  enum MAX30101_SlotEnum
  {
    DISABLED = 0x00,   // Time slot is disabled
    RED_LED_PA,        // RED LED is active using LED1_PA
    IR_LED_PA,         // IR LED is active using LED2_PA
    GREEN_LED_PA,      // Green LED is active using LED3_PA
    OFF,		// Time slot is enabled but no active LED
    RED_LED_PILOT, 	   // RED LED is active using PILOT_PA
    IR_LED_PILOT,      // IR LED is active using PILOT_PA
    GREEN_LED_PILOT    // GREEN LED is active using PILOT_PA
  };  // LED Config
  
  struct MAX30101_SlotControlStruct
  { // SLOT Config  
    BSP_MAX30101_SlotEnum   SLOT1_Control;
    BSP_MAX30101_SlotEnum   SLOT2_Control;
    BSP_MAX30101_SlotEnum   SLOT3_Control;
    BSP_MAX30101_SlotEnum   SLOT4_Control;
  };    
  
  struct MAX30101_HandleStruct
  {
    //FIFO Config
    BSP_MAX30101_SampleAverageEnum       SMP_AVE;
    BSP_MAX30101_FIFORollOverEnum       FIFO_ROLLOVER_EN;
    BSP_MAX30101_FIFOAFullEnum         FIFO_A_FULL;  
    //Mode Config 
    BSP_MAX30101_ModeEnum                MODE;
    //MAX30101 Config 
    BSP_MAX30101_ADCFullScaleEnum        ADC_Range_Control;
    BSP_MAX30101_SampleRateEnum          Sample_Rate_Control;
    BSP_MAX30101_PulseWidthEnum          Pulse_Width_Control;
    // Proximity Threshold
    uint8_t                          PROX_INT_THRESH_Control;
    // LED Config
    BSP_MAX30101_LEDCurrentStruct        LEDx_Current_Control;
    BSP_MAX30101_SlotControlStruct       SLOTx_Control;
  };
  
  struct MAX30101_UpdateStruct
  {
    //MAX30101 update  
    BSP_MAX30101_ADCFullScaleEnum        ADC_Range_Control; 
    BSP_MAX30101_PulseWidthEnum          Pulse_Width_Control;
    // LED update
    BSP_MAX30101_LEDCurrentStruct        LEDx_Current_Control;
  };
  
  enum MAX30101_InterruptEnum
  {
    PWR_RDY_BitControl          = 0x01,  // PWR Ready ENABLE
    DIE_TEMP_DRY_BitControl     = 0x02,   // Internal Temperature Ready Flag ENABLE
    PROX_INT_BitControl         = 0x10,  // Proximity Threshold Triggered ENABLE      
    ALC_OVF_BitControl          = 0x20,  // Ambient Light Cancellation Overflow ENABLE
    PPG_DRY_BitControl          = 0x40,  // New FIFO Data Ready ENABLE
    A_FULL_BitControl           = 0x80  // FIFO Almost Full Flag ENABLE
  };
  
  enum MAX30101_AddressEnum
  {
    //Interrupt registers
    INT_STATUS1         = 0x00,
    INT_STATUS2         ,//= 0x01,
    INT_ENABLE1         ,//= 0x02,
    INT_ENABLE2         ,//= 0x03,
    //FIFO registers
    FIFO_WRITE_PTR      ,//= 0x04,           
    OVER_FLOW_CNT       ,//= 0x05,   
    FIFO_READ_PTR       ,//= 0x06,
    FIFO_DATA_REG       ,//= 0x07,  
    //Configuration registers
    FIFO_CONFIG         ,//= 0x08,   
    MODE_CONFIG         ,//= 0x09,
    SPO2_CONFIG         ,//= 0x0A,
    RESERVED_0x0B,
    LED1_CONFIG         ,//= 0x0C,   
    LED2_CONFIG         ,//= 0x0D,  
    LED3_CONFIG         ,//= 0x0E,  
    RESERVED_0x0F,
    LED_PROX_CONFIG     ,//= 0x10,  
    LED_MULTI12_CONFIG  ,//= 0x11,   
    LED_MULTI34_CONFIG  ,//= 0x12, 
    RESERVED_0x13,
    RESERVED_0x14,
    RESERVED_0x15,
    RESERVED_0x16,
    RESERVED_0x17,
    RESERVED_0x18,
    RESERVED_0x19,
    RESERVED_0x1A,
    RESERVED_0x1B,
    RESERVED_0x1C,
    RESERVED_0x1D,
    RESERVED_0x1E,
    //Temperature registers 
    TEMP_INTEGER        ,//= 0x1F,
    TEMP_FRACTION       ,//= 0x20,
    TEMP_CONFIG         ,//= 0x21,
    RESERVED_0x22,
    RESERVED_0x23,
    RESERVED_0x24,
    RESERVED_0x25,
    RESERVED_0x26,
    RESERVED_0x27,
    RESERVED_0x28,
    RESERVED_0x29,
    RESERVED_0x2A,
    RESERVED_0x2B,
    RESERVED_0x2C,
    RESERVED_0x2D,
    RESERVED_0x2E,
    RESERVED_0x2F,
    //Proximity registers 
    INT_PROXIMITY       ,//= 0x30, 
    //Part ID registers
    REVISION_ID         = 0xFE,
    PART_ID             //= 0xFF
  } ;
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported variables --------------------------------------------------------*/
  /* Exported function prototypes ----------------------------------------------*/
  BSP_MAX30101_StatusEnum BSP_MAX30101_Init(void);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_WakeUp(void);
  BSP_MAX30101_StatusEnum BSP_MAX30101_Sleep(void);
  BSP_MAX30101_StatusEnum BSP_MAX30101_Reset(void);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_SetConfiguration(BSP_MAX30101_HandleStruct *Handle);
  BSP_MAX30101_StatusEnum BSP_MAX30101_UpdateLEDCurrent(BSP_MAX30101_LEDCurrentStruct *LEDx_Current);
  BSP_MAX30101_StatusEnum BSP_MAX30101_UpdateConfiguration(BSP_MAX30101_UpdateStruct *Update);
  BSP_MAX30101_StatusEnum BSP_MAX30101_SetInterrupt(BSP_MAX30101_InterruptStruct *Interrupt);
  BSP_MAX30101_StatusEnum BSP_MAX30101_SetRegValue(BSP_MAX30101_AddressEnum Registre, uint8_t value);
  BSP_MAX30101_StatusEnum BSP_MAX30101_MultiLedModeControl(BSP_MAX30101_SlotControlStruct *SLOTx_Control);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_GetRegValue(BSP_MAX30101_AddressEnum Registre, 
                                           uint8_t *value);
  BSP_MAX30101_StatusEnum BSP_MAX30101_GetBuffValue(BSP_MAX30101_AddressEnum Registre, 
                                            uint16_t size, uint8_t *value);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_GetInterruptStatus(BSP_MAX30101_InterruptEnum Interrupt, 
                                                  BSP_MAX30101_BitStatusEnum *status);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_ReadTemp(uint8_t *Temp_Integer, 
                                        uint8_t *Temp_Fraction);
  
  BSP_MAX30101_StatusEnum BSP_MAX30101_ReadLEDValue(uint8_t *nSamples, BSP_MAX30101_LEDStruct * LED); 
  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_MAX30101_H */
/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/