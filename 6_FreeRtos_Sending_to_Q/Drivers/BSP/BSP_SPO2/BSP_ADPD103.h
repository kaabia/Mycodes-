/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_SPO2/BSP_ADPD103.h $
* $Author: ghada.dhibi $
* $Revision: 448 $
* $Date: 2016-06-10 15:14:30 +0100 (ven., 10 juin 2016) $
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_ADPD103_H
#define __BSP_ADPD103_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#include "DRV_I2C.h" 
//#include "configADPDTask.h"
  
  //#define SLOTA_AFE_WIDTH ((uint8_t)APP_SLOTx_AFE_WIDTH);
  //#define SLOTB_AFE_WIDTH ((uint8_t)APP_SLOTx_AFE_WIDTH);
  /* Exported types ----------------------------------------------------------*/  
  
  typedef enum ADPD103_AddressRegister          BSP_ADPD103_AddressRegister;
  typedef enum ADPD103_DefaultValueRegister     BSP_ADPD103_DefaultValueRegister;
  typedef enum ADPD103_SLOTxPDSel               BSP_ADPD103_SLOTxPDSel;
  typedef enum ADPD103_SLOTxLEDSel              BSP_ADPD103_SLOTxLEDSel;
  typedef enum ADPD103_LEDCoarse                BSP_ADPD103_LEDCoarse;
  typedef enum ADPD103_LEDScale                 BSP_ADPD103_LEDScale;
  typedef enum ADPD103_AFEMode                  BSP_ADPD103_AFEMode;
  typedef enum ADPD103_AFE_PowerDown            BSP_ADPD103_AFE_PowerDown;
  typedef enum ADPD103_ADCTiming                BSP_ADPD103_ADCTiming;
  typedef enum ADPD103_OperatingMode            BSP_ADPD103_OperatingMode;
  typedef enum ADPD103_SLOTNumAverage           BSP_ADPD103_SLOTNumAverage;
  typedef enum ADPD103_UsedValue                BSP_ADPD103_UsedValue;
  typedef struct ADPD103_HandleStruct           BSP_ADPD103_HandleStruct;
  typedef enum ADPD103_Status                   BSP_ADPD103_Status;
  typedef enum ADPD103_LEDSlew                  BSP_ADPD103_LEDSlew;
  typedef struct ADPD103_LEDCH                  BSP_ADPD103_LEDCH;
  typedef struct ADPD103_LEDsCH                 BSP_ADPD103_LEDsCH;
  
  
  /* Address registers */
  enum ADPD103_AddressRegister
  {
    Status              = 0x00,
    INT_MASK            = 0x01,
    INT_IO_CTL          = 0x02,
    FIFO_THRESH         = 0x06,
    DEVID               = 0x08,
    I2CS_ID             = 0x09,
    CLK_RATIO           = 0x0A,
    SLAVE_ADDRESS_KEY   = 0x0D,
    SW_RESETR           = 0x0F,
    ModeR               = 0x10,
    SLOT_EN             = 0x11,
    FSAMPLE             = 0x12,
    PD_LED_SELECT       = 0x14,
    NUM_AVG             = 0x15,
    SLOTA_CH1_OFFSET    = 0x18,
    SLOTA_CH2_OFFSET    = 0x19,
    SLOTA_CH3_OFFSET    = 0x1A,
    SLOTA_CH4_OFFSET    = 0x1B,
    SLOTB_CH1_OFFSET    = 0x1E,
    SLOTB_CH2_OFFSET    = 0x1F,
    SLOTB_CH3_OFFSET    = 0x20,
    SLOTB_CH4_OFFSET    = 0x21,
    ILED3_COARSE        = 0x22,
    ILED1_COARSE        = 0x23,
    ILED2_COARSE        = 0x24,
    ILED_FINE           = 0x25,
    SLOTA_LED_PULSE     = 0x30,
    SLOTA_NUMPULSES     = 0x31,
    LED_DISABLE         = 0x34,
    SLOTB_LED_PULSE     = 0x35,
    SLOTB_NUMPULSES     = 0x36,
    EXT_SYNC_STARTUP    = 0x38,
    SLOTA_AFE_WINDOW    = 0x39,
    SLOTB_AFE_WINDOW    = 0x3B,
    AFE_PWR_CFG1        = 0x3C,
    SLOTA_TIA_CFG       = 0x42,
    SLOTA_AFE_CFG       = 0x43,
    SLOTB_TIA_CFG       = 0x44,
    SLOTB_AFE_CFG       = 0x45,
    SAMPLE_CLK          = 0x4B,
    CLK32M_ADJUST       = 0x4D,
    ADC_CLOCK           = 0x4E,
    EXT_SYNC_SEL        = 0x4F,
    CLK32M_CAL_EN       = 0x50,
    AFE_PWR_CFG2        = 0x54,
    TIA_INDEP_GAIN      = 0x55,
    DIGITAL_INT_EN      = 0x58,
    DIG_INT_CFG         = 0x5A,
    DATA_ACCESS_CTL     = 0x5F,
    FIFO_ACCESS         = 0x60,
    SLOTA_PD1_16BIT     = 0x64,
    SLOTA_PD2_16BIT     = 0x65,
    SLOTA_PD3_16BIT     = 0x66,
    SLOTA_PD4_16BIT     = 0x67,
    SLOTB_PD1_16BIT     = 0x68,
    SLOTB_PD2_16BIT     = 0x69,
    SLOTB_PD3_16BIT     = 0x6A,
    SLOTB_PD4_16BIT     = 0x6B,
    A_PD1_LOW           = 0x70,
    A_PD2_LOW           = 0x71,
    A_PD3_LOW           = 0x72,
    A_PD4_LOW           = 0x73,
    A_PD1_HIGH          = 0x74,
    A_PD2_HIGH          = 0x75,
    A_PD3_HIGH          = 0x76,
    A_PD4_HIGH          = 0x77,
    B_PD1_LOW           = 0x78,
    B_PD2_LOW           = 0x79,
    B_PD3_LOW           = 0x7A,
    B_PD4_LOW           = 0x7B,
    B_PD1_HIGH          = 0x7C,
    B_PD2_HIGH          = 0x7D,
    B_PD3_HIGH          = 0x7E,
    B_PD4_HIGH          = 0x7F
  };
  
  /* The default value of registers */
  enum ADPD103_DefaultValueRegister
  {
    SAMPLE_CLKM          =        0x2612,
    ModeRM               =        0x0000,
    SW_RESETRM           =        0x0000,
    SLOT_LED_PULSEM      =        0x0320,
    DATA_ACCESS_CTLM     =        0x0000,
    PD_LED_SELECTM       =        0x0541,
    AFE_PWR_CFG1M        =        0x3006,
    NUM_AVGM             =        0x0600,
    ILED_COARSEM         =        0x3000,
    SLOT_ENM             =        0x1000
  };
  
  /* Values to be used in configuration */
  enum ADPD103_UsedValue
  {
    SLOTA_AFE_WIDTH     =       6+1, // SLOTA_LED_WIDTH+1, // OK
    SLOTA_AFE_OFFSET    =       25-9, // OK
    SLOTA_AFE_FOFFSET   =       0x1B, // OK
    
    SLOTB_AFE_WIDTH     =       6+1, // SLOTA_LED_WIDTH+1, // OK
    SLOTB_AFE_OFFSET    =       25-9, // OK
    SLOTB_AFE_FOFFSET   =       0x1B, // OK
    
    CLK32K_EN           =       0x01,
    SLOTB_DATA_HOLD     =       0x01,
    SLOTA_DATA_HOLD     =       0x01,
    FIFO_ACCESS_ENA     =       0x1,
    FIFO_THRESHV        =       0x1,
    FIFO_OVRN_PREVENT   =       0x1,
    SLOTB_FIFO_MODE     =       0x4,
    SLOTB_EN            =       0x1,
    SLOTA_FIFO_MODE     =       0x4, // 4 analog channels
    SLOTA_EN            =       0x1,    
    FSAMPLEV            =       0x000A
      
  };
  
  /************************ LED CONTROL Configuration **************************/
  /* Select connection of PD */
  enum ADPD103_SLOTxPDSel
  {
    PDALL       =       0x1,
    PD5_8       =       0x4,
    PD1_4       =       0x5
  };
  
  /* Time slot LED configuration */
  enum ADPD103_SLOTxLEDSel
  {
    SLOT_LED1     =       0x1,
    SLOT_LED2     =       0x2,
    SLOT_LED3     =       0x3
  };
  
  /* LED coarse current setting */
  enum ADPD103_LEDCoarse
  {
    LED_Coarse25mA        =       0x00,
    LED_Coarse40mA        =       0x01,
    LED_Coarse55mA        =       0x02,
    LED_Coarse70mA        =       0x03,
    LED_Coarse85mA        =       0x04,
    LED_Coarse100mA       =       0x05,
    LED_Coarse115mA       =       0x06,
    LED_Coarse130mA       =       0x07,
    LED_Coarse145mA       =       0x08,
    LED_Coarse160mA       =       0x09,
    LED_Coarse175mA       =       0x0A,
    LED_Coarse190mA       =       0x0B,
    LED_Coarse205mA       =       0x0C,
    LED_Coarse220mA       =       0x0D,
    LED_Coarse235mA       =       0x0E,
    LED_Coarse350mA       =       0x0F
  };
  
  /* LED current scale factor */
  enum ADPD103_LEDScale
  {
    LED_SCALE100          =       0x01,
    LED_SCALE40           =       0x00
  };
  
  /* LED driver slew rate control */
  enum ADPD103_LEDSlew
  {
    LED_SLEW0,
    LED_SLEW1,
    LED_SLEW2,
    LED_SLEW3,
    LED_SLEW4,
    LED_SLEW5,
    LED_SLEW6,
    LED_SLEW7
  };
  
  /****************************** AFE Configuration ***************************/
  /* AFE channels power-down select */
  enum ADPD103_AFE_PowerDown
  {
    AFE_POWERDOWN_CH2_4   =       0x38,
    AFE_NO_POWERDOWN      =       0x00
  };
  
  /* Time Slot AFE setting */
  enum ADPD103_AFEMode
  {
    AFE_NormalMode        =       0x1C,
    AFE_DigitalMode       =       0x1D
  };
  
  /**************************** SYSTEM Configuration **************************/
  /* ADC clock speed */
  enum ADPD103_ADCTiming
  {
    ADC_Timing1M          =       0x0040,
    ADC_Timing500K        =       0x0060
  };
  
  /* the operating mode */
  enum ADPD103_OperatingMode
  {
    StandbyMode           =       0x00,
    ProgramMode           =       0x01,
    NormalMode            =       0x02
  };
  
  /****************************** ADC Configuration ***************************/
  /* Sample sum/average for Time Slot */
  enum ADPD103_SLOTNumAverage
  {
    AveragingFactor_1,
    AveragingFactor_2,
    AveragingFactor_4,
    AveragingFactor_8,
    AveragingFactor_16,
    AveragingFactor_32,
    AveragingFactor_64,
    AveragingFactor_128
  };
  
  struct ADPD103_HandleStruct
  {
    BSP_ADPD103_SLOTxPDSel     SLOTB_PD_SEL, SLOTA_PD_SEL;
    BSP_ADPD103_SLOTxLEDSel    SLOTB_LED_SEL, SLOTA_LED_SEL;
    BSP_ADPD103_AFEMode        SLOTA_AFE_MODE, SLOTB_AFE_MODE;
    BSP_ADPD103_AFE_PowerDown  AFE_POWERDOWN;
    BSP_ADPD103_ADCTiming      ADC_Timing;    
    BSP_ADPD103_OperatingMode  Mode;
    BSP_ADPD103_SLOTNumAverage SLOTB_NUM_AVG, SLOTA_NUM_AVG;
    BSP_ADPD103_LEDScale       LED1_SCALE, LED2_SCALE, LED3_SCALE;
    BSP_ADPD103_LEDCoarse      LED1_COARSE, LED2_COARSE, LED3_COARSE;
    BSP_ADPD103_LEDSlew        LED1_SLEW, LED2_SLEW, LED3_SLEW;   
    uint8_t                SLOTA_LED_WIDTH, SLOTA_LED_OFFSET, SLOTA_LED_NUMBER, SLOTA_LED_PERIOD;
    uint8_t                SLOTB_LED_WIDTH, SLOTB_LED_OFFSET, SLOTB_LED_NUMBER, SLOTB_LED_PERIOD;
  };
  
  
  enum ADPD103_Status
  {
    ADPD103_OK       = 0x00,
    ADPD103_ERROR    = 0x01
  };
  
  struct ADPD103_LEDCH
  {
    uint16_t CH1;
    uint16_t CH2;
    uint16_t CH3;
    uint16_t CH4;
  };
  
  struct ADPD103_LEDsCH
  {
    BSP_ADPD103_LEDCH led[2];
  };
  
  
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported variables --------------------------------------------------------*/
  /* Exported function prototypes ----------------------------------------------*/
  
  BSP_ADPD103_Status BSP_ADPD103_Init(void);
  BSP_ADPD103_Status BSP_ADPD103_SetNormalMode();
  BSP_ADPD103_Status BSP_ADPD103_SetProgramMode();
  BSP_ADPD103_Status BSP_ADPD103_SetRegValue(uint8_t Register, uint16_t value);
  BSP_ADPD103_Status BSP_ADPD103_SetRegMSBValue(uint8_t Register, uint8_t value);
  BSP_ADPD103_Status BSP_ADPD103_ReadReg(uint8_t Register, uint16_t* data);
  BSP_ADPD103_Status BSP_ADPD103_ReadMSBReg(uint8_t Register, uint8_t* data);
  BSP_ADPD103_Status BSP_ADPD103_ReadBuffer(uint8_t Register, uint8_t* data, uint8_t number);
  BSP_ADPD103_Status BSP_ADPD103_ReadDataRegister (BSP_ADPD103_LEDCH LED[2]);
  BSP_ADPD103_Status BSP_ADPD103_ReadDataFIFO (BSP_ADPD103_LEDsCH DATA[], uint8_t *number);//(BSP_ADPD103_LEDCH LED[2]);
  BSP_ADPD103_Status BSP_ADPD103_SoftwareReset();
  BSP_ADPD103_Status BSP_ADPD103_SetGeneralConfiguration(BSP_ADPD103_HandleStruct SPO2);
  BSP_ADPD103_Status BSP_ADPD103_ClearFIFO (void);
  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_ADPD103_H */
/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
