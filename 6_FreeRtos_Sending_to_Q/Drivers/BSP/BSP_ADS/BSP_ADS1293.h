/**
  ******************************************************************************
  * $HeadURL:$
  * $Author:$
  * $Revision:$
  * $Date:$
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_ADS1293_H
#define __BSP_ADS1293_H



  
/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "cmsis_os.h"
#include "BSP_ADS1293_REG.h"
#include "DRV_SPI.h"
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADS1293_SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define ADS1293_CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define ADS1293_READ_BIT(REG, BIT)    ((REG) & (BIT))
#define ADS1293_CLEAR_REG(REG)        ((REG) = (0x0))
#define ADS1293_WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define ADS1293_MODIFY_REG(REG, CLEARMASK, SETMASK)  ADS1293_WRITE_REG((REG), (((REG) & (~(CLEARMASK))) | (SETMASK)))
#define ADS1293_POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) // ARM CPUs dependent intrinsic!
#define DISABLE_SYNCBOUT (1)
#define ENABLE_SYNCBOUT (0)
#define START_CONVERSATION (0x01)
#define STOP_CONVERSATION (0x00)
  

/* Exported types ------------------------------------------------------------*/
typedef uint8_t REG8;
typedef uint8_t BIT;
typedef uint8_t DATA8;
typedef uint16_t GPIO_PIN;

typedef enum
{
  
  ADS1293_TS_OPEN,
  ADS1293_TS_CLOSE_V1,
  ADS1293_TS_CLOSE_V2,
  
  ADS1293_TS_PREGNAUTE,
  
  ADS1293_TS_SOMNONAUTE,
  ADS1293_CAP_SOMNONAUTE,
  
  ADS1293_NO_TS,
  
}ADS1293_ChannelMappingType_t;

/**
* @brief  
* @param  
* @retval None
*/
typedef struct 
{
  uint8_t registerAdress;
  uint8_t registerValue;
  GPIO_TypeDef* GPIOx; 
  uint16_t GPIO_Pin;
}xADSSinusInjectConfig_t;


typedef enum
{
   ADS1293_CHANNEL_ENABLED = 0x00,
   ADS1293_CHANNEL_DISABLED = 0x01,
}ADS1293_CHANNEL_STATE;


typedef enum
{
  Z_LEVEL_0 = 0x00,
  Z_LEVEL_1 = 0x00,
  Z_LEVEL_2 = 0x00,
  Z_LEVEL_3 = 0x00,
  Z_LEVEL_4 = 0x00,
}eADS_ImpedanceLevel_t;


typedef struct
{
  eADS_ImpedanceLevel_t Value[12];
}xADS_Impedance_t;



#define ADS1293_Status_TypeDef HAL_StatusTypeDef
#define ADS1293_OK  HAL_OK
#define ADS1293_ERROR  HAL_ERROR

typedef enum ADS1293_OpMode_Enum ADS1293_OpMode_TypeDef;
enum ADS1293_OpMode_Enum
{
  STD_MODE = 0x00, // STANDBY mode deactivated, PWR_DOWN mode deacivated
  STDBY = 0x01, // STANDBY mode activated
  PWRDWN = 0x02, // PWR_DOWN mode acivated 
};

typedef enum ADS1293_Channel_Enum ADS1293_Channel_TypeDef;
enum ADS1293_Channel_Enum
{
  CH1 = 0x01, // CH1
  CH2 = 0x02, // CH2
  CH3 = 0x03, // CH3
  PACE = 0x04, // PACE
};
  
typedef enum ADS1293_In_Enum ADS1293_In_TypeDef;
enum ADS1293_In_Enum
{
  DISC = 0x00, // Disconnected
  IN1 = 0x01, // IN1
  IN2 = 0x02, // IN2
  IN3 = 0x03, // IN3
  IN4 = 0x04, // IN4
  IN5 = 0x05, // IN5
  IN6 = 0x06, // IN6
};

typedef enum ADS1293_InMask_Enum ADS1293_InMask_TypeDef;
enum ADS1293_InMask_Enum
{
  INP1_MASK = 0x01, // IN1
  INP2_MASK = 0x02, // IN2
  INP3_MASK = 0x04, // IN3
  INP4_MASK = 0x08, // IN4
  INP5_MASK = 0x10, // IN5
  INP6_MASK = 0x20, // IN6
};
  
typedef enum ADS1293_DetectCurrent_Enum ADS1293_DetectCurrent_TypeDef;
enum ADS1293_DetectCurrent_Enum
{
  DET_CURR_0nA = 0x00, // 0 nA 
  DET_CURR_8nA = 0x01, // 8 nA
  DET_CURR_16nA = 0x02, // 16 nA
  DET_CURR_24nA = 0x03, // 24 nA
  // .
  // .          // to be defined
  DET_CURR_80nA = 0x0A, // 24 nA
  // .
  DET_CURR_88nA = 0x0B, // 24 nA
  //.
  DET_CURR_152nA = 0x13, // 200 nA
  //.
  DET_CURR_200nA = 0x19, // 200 nA
  //.
  DET_CURR_400nA = 0x32, // 400 nA
  // .
  DET_CURR_600nA = 0x4B, // 600 nA
  // .
  DET_CURR_1024nA = 0x80, // 600 nA
  // .
  DET_CURR_2032nA = 0xFE, // 2.032 uA  
  DET_CURR_2040nA = 0xFF, // 2.040 uA
};
  
typedef enum ADS1293_FreqDivFactor_Enum ADS1293_FreqDivFact_TypeDef;
enum ADS1293_FreqDivFactor_Enum
{
  K1 = 0x00, // K == 1 
  K16 = 0x01, // K == 16
};
 
typedef enum ADS1293_FlexChCn_Enum ADS1293_FlexChCn_TypeDef;
enum ADS1293_FlexChCn_Enum
{
  FLEX_CHx_CN_DISC = 0x00, // Terminal is disconnected (default)
  FLEX_CHx_CN_IN1 = 0x01, // Terminal connected to input IN1
  FLEX_CHx_CN_IN2 = 0x02, // Terminal connected to input IN2
  FLEX_CHx_CN_IN3 = 0x03, // Terminal connected to input IN3
  FLEX_CHx_CN_IN4 = 0x04, // Terminal connected to input IN4
  FLEX_CHx_CN_IN5 = 0x05, // Terminal connected to input IN5
  FLEX_CHx_CN_IN6 = 0x06, // Terminal connected to input IN6
};

typedef enum ADS1293_AclvlLod_Enum ADS1293_AclvlLod_TypeDef; // Programmable comparator trigger level for AC lead-off detection
enum ADS1293_AclvlLod_Enum
{
  LOD_CN_ACLVL_LOD_L1 = 0x00, // Level 1
  LOD_CN_ACLVL_LOD_L2 = 0x01, // Level 2
  LOD_CN_ACLVL_LOD_L3 = 0x02, // Level 3
  LOD_CN_ACLVL_LOD_L4 = 0x03, // Level 4
};

typedef enum ADS1293_CmDetCapDrive_Enum ADS1293_CmDetCapDrive_TypeDef;
enum ADS1293_CmDetCapDrive_Enum
{
  CMDET_CN_CMDET_CAPDRIVE_LOW_CAP = 0x00, // Low cap-drive mode (default)       
  CMDET_CN_CMDET_CAPDRIVE_MEDIUM_LOW_CAP = 0x01, // Medium low cap-drive mode
  CMDET_CN_CMDET_CAPDRIVE_MEDIUM_HIGH_CAP = 0x02, // Medium high cap-drive mode
  CMDET_CN_CMDET_CAPDRIVE_HIGH_CAP = 0x03, // High cap-drive mode
};

typedef enum ADS1293_SelRld_Enum ADS1293_SelRld_TypeDef;
enum ADS1293_SelRld_Enum
{
  RLD_CN_SELRLD_DISC = 0x00,
  RLD_CN_SELRLD_IN1 = 0x01,
  RLD_CN_SELRLD_IN2 = 0x02,
  RLD_CN_SELRLD_IN3 = 0x03,
  RLD_CN_SELRLD_IN4 = 0x04,
  RLD_CN_SELRLD_IN5 = 0x05,
  RLD_CN_SELRLD_IN6 = 0x06,
};

typedef enum ADS1293_r2Rate_Enum ADS1293_r2Rate_TypeDef;
enum ADS1293_r2Rate_Enum
{
  R2_RATE_R2_RATE_4 = 0x01,
  R2_RATE_R2_RATE_5 = 0x02, 
  R2_RATE_R2_RATE_6 = 0x04,
  R2_RATE_R2_RATE_8 = 0x08,
};

typedef enum ADS1293_r3Rate_Enum ADS1293_r3Rate_TypeDef;
enum ADS1293_r3Rate_Enum
{
  R3_RATE_CHx_R3_RATE_CHx_4  = 0x01,
  R3_RATE_CHx_R3_RATE_CHx_6 = 0x02,
  R3_RATE_CHx_R3_RATE_CHx_8 = 0x04,
  R3_RATE_CHx_R3_RATE_CHx_12 = 0x08,
  R3_RATE_CHx_R3_RATE_CHx_16 = 0x10,
  R3_RATE_CHx_R3_RATE_CHx_32 = 0x20,
  R3_RATE_CHx_R3_RATE_CHx_64 = 0x40,
  R3_RATE_CHx_R3_RATE_CHx_128 = 0x80,
};

typedef enum ADS1293_drdybSrc_Enum ADS1293_drdybSrc_TypeDef;
enum ADS1293_drdybSrc_Enum
{
  DRDYB_SRC_DRDYB_SRC_NONE = 0x00, // DRDYB pin not asserted (default)    
  DRDYB_SRC_DRDYB_SRC_CH1_PACE = 0x01, // Driven by Channel 1 pace 
  DRDYB_SRC_DRDYB_SRC_CH2_PACE = 0x02, // Driven by Channel 2 pace
  DRDYB_SRC_DRDYB_SRC_CH3_PACE = 0x04, // Driven by Channel 3 pace
  DRDYB_SRC_DRDYB_SRC_CH1_ECG = 0x08, // Driven by Channel 1 ECG
  DRDYB_SRC_DRDYB_SRC_CH2_ECG = 0x10, // Driven by Channel 2 ECG
  DRDYB_SRC_DRDYB_SRC_CH3_ECG = 0x20, // Driven by Channel 3 ECG
};

typedef enum ADS1293_syncbSrc_Enum ADS1293_syncbSrc_TypeDef; // Select channel to drive the SYNCB pin
enum ADS1293_syncbSrc_Enum
{
  SYNCB_CN_SYNCB_SRC_NONE = 0x00, // No source selected (default)    
  SYNCB_CN_SYNCB_SRC_CH1_PACE = 0x01, // Driven by Channel 1 pace 
  SYNCB_CN_SYNCB_SRC_CH2_PACE = 0x02, // Driven by Channel 2 pace
  SYNCB_CN_SYNCB_SRC_CH3_PACE = 0x04, // Driven by Channel 3 pace
  SYNCB_CN_SYNCB_SRC_CH1_ECG = 0x08, // Driven by Channel 1 ECG
  SYNCB_CN_SYNCB_SRC_CH2_ECG = 0x10, // Driven by Channel 2 ECG
  SYNCB_CN_SYNCB_SRC_CH3_ECG = 0x20, // Driven by Channel 3 ECG
};

typedef enum ADS1293_ADSSel_Enum ADS1293_ADSSel_TypeDef;
enum ADS1293_ADSSel_Enum
{
  AFE_CSn_FRONT = 0x00,
  AFE_CSn_PRECORD1 = 0x01,
  AFE_CSn_PRECORD2 = 0x02,
  AFE_CSn_PRECORD3 = 0x03,
};

typedef enum
{
  LOD_INIT = 0x0001,
  LOD_INPUTSEL = 0x0002,
  LOD_CHANGELEVEL = 0x0003,
  LOD_WAITMS = 0x0004,
  LOD_READERRORLOD = 0x0005,
  LOD_STORERES = 0x0006,
  LOD_DEINIT = 0x0007
}ADS1293_eLeadOffDetect_t;

typedef enum
{
  ADS1 = 0x0001,
  ADS2 = 0x0002,
  ADS3 = 0x0003,
  ADS4 = 0x0004
}ADS1293_eADSSel_t;

typedef enum
{
  ADS1293_Normal   = 0x00,
  ADS1293_TestBenchConfig   = 0x01,
  ADS1293_SinusInjectConfig = 0x02
}ADS1293_SelectConfig;

//typedef enum
//{
//  Z_LEVEL4 = 0x0001,
//  Z_LEVEL3 = 0x0002,
//  Z_LEVEL2 = 0x0003,
//  Z_LEVEL1 = 0x0004,
//  Z_LEVEL0 = 0x0005
//}ADS1293_eZlevel_t;

typedef struct ADS1293_LodCnReg_Struct ADS1293_LodCnReg_TypeDef;
struct ADS1293_LodCnReg_Struct
{
  ADS1293_AclvlLod_TypeDef aclvl_lod; // Programmable comparator trigger level for AC lead-off detection
  BIT selac_lod; // Lead-off detect operation mode
  BIT shdn_lod; // Shut down lead-off detection
  BIT acad_lod; // AC analog/digital lead-off mode select
};

typedef struct ADS1293_CmDetConfig_Struct ADS1293_CmDetConfig_TypeDef; // Common-mode detector configuartion struct
struct ADS1293_CmDetConfig_Struct
{
  //ADS1293_InMask_TypeDef inputs; // input: Common mode detect input pins
  uint8_t inputs; // input: Common mode detect input pins
  BIT cmd_bw; // Common mode detect bandwidth mode
  ADS1293_CmDetCapDrive_TypeDef cmd_capdrive; // Common mode detect capacitive load drive capability
};

typedef struct ADS1293_WilsonRef_Struct ADS1293_WilsonRef_TypeDef; // Wilson Reference configuartion struct
struct ADS1293_WilsonRef_Struct
{
  ADS1293_In_TypeDef selWilson1; // Wilson reference routing for the first buffer amplifier
  ADS1293_In_TypeDef selWilson2; // Wilson reference routing for the second buffer amplifier
  ADS1293_In_TypeDef selWilson3; // Wilson reference routing for the third buffer amplifier
  BIT wilsonint; // Wilson reference output internally connected/disconnected (1/0) to/from IN6
  BIT goldint; // 1: Goldberger reference outputs internally connected to IN4, IN5 and IN6, 0: Goldberger reference disabled
};

typedef struct ADS1293_ChCnfg_Struct ADS1293_ChCnfg_TypeDef; // Channel for Loop Read Back Mode configuration struct
struct ADS1293_ChCnfg_Struct
{
  BIT sts_en; // 0: Disable data status read back (default), 1: Enable data status read back
  BIT p1_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
  BIT p2_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
  BIT p3_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
  BIT e1_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
  BIT e2_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
  BIT e3_en; // 0: Disable data read back for this channel (default), 1: Enable data read back for this channel
};

typedef struct ADS1293_maskError_Struct ADS1293_maskError_TypeDef; // Mask Error on ALARMB Pin configuration struct
struct ADS1293_maskError_Struct
{
  BIT mask_cmor; // Mask alarm condition for CMOR=1; 0: Alarm condition active (default), 1: Alarm condition is masked
  BIT mask_rldrail; // Mask alarm condition for RLDRAIL=1; 0: Alarm condition active (default), 1: Alarm condition is masked
  BIT mask_batlow; // Mask alarm condition for BATLOW=1; 0: Alarm condition active (default), 1: Alarm condition is masked
  BIT mask_leadoff; // Mask alarm condition for LEADOFF=1; 0: Alarm condition active (default), 1: Alarm condition is masked 
  BIT mask_ch1err; // Mask alarm condition for CH1ERR=1; 0: Alarm condition active (default), 1: Alarm condition is masked 
  BIT mask_ch2err; // Mask alarm condition for CH2ERR=1; 0: Alarm condition active (default), 1: Alarm condition is masked
  BIT mask_ch3err; // Mask alarm condition for CH3ERR=1; 0: Alarm condition active (default), 1: Alarm condition is masked
  BIT mask_syncedgeer; // Mask alarm condition when SYNCEDGEERR=1; 0: Alarm condition is active (default), 1: Alarm condition is masked
};

typedef struct ADS1293_ChannelInpConfig_Struct ADS1293_ChannelInpConfig_TypeDef;
struct ADS1293_ChannelInpConfig_Struct
{
  ADS1293_Channel_TypeDef channel;
  ADS1293_FlexChCn_TypeDef neg_in;
  ADS1293_FlexChCn_TypeDef pos_in;
};

typedef struct ADS1293_LOD_AC_Config_Struct ADS1293_LOD_AC_Config_TypeDef;
struct ADS1293_LOD_AC_Config_Struct
{
  ADS1293_FreqDivFact_TypeDef freqDiv;
  REG8 clkDiv;
};

typedef struct ADS1293_InterRef_Struct ADS1293_InterRef_TypeDef;
struct ADS1293_InterRef_Struct
{
  BIT shdnInterRef;
  BIT shdnCmRef;
};

typedef struct ADS1293_OscClkConfig_Struct ADS1293_OscClkConfig_TypeDef;
struct ADS1293_OscClkConfig_Struct
{
  BIT shdnOsc;
  BIT enClkOut;
};

typedef struct ADS1293_AFEResFreq_Struct ADS1293_AFEResFreq_TypeDef;
struct ADS1293_AFEResFreq_Struct
{
  ADS1293_Channel_TypeDef channel;
  BIT hiResEnable;
  BIT clkFreq;
};

typedef struct ADS1293_AFEShdn_Struct ADS1293_AFEShdn_TypeDef;
struct ADS1293_AFEShdn_Struct
{
  ADS1293_Channel_TypeDef channel;
  BIT shdn_ina;
  BIT shdn_sdm;
};

typedef struct ADS1293_AFEFaultDet_Struct ADS1293_AFEFaultDet_TypeDef;
struct ADS1293_AFEFaultDet_Struct
{
  ADS1293_Channel_TypeDef channel;
  BIT enable;
};

typedef struct ADS1293_R1R3DecRate_Struct ADS1293_R1R3DecRate_TypeDef;
struct ADS1293_R1R3DecRate_Struct
{
  ADS1293_Channel_TypeDef channel;
  BIT r1;
  ADS1293_r3Rate_TypeDef r3;
};       

typedef struct ADS1293_ECGFilterEn_Struct ADS1293_ECGFilterEn_TypeDef;
struct ADS1293_ECGFilterEn_Struct
{
  ADS1293_Channel_TypeDef channel;
  BIT enable;
};

typedef struct ADS1293_DRDYB_Struct ADS1293_DRDYB_TypeDef;
struct ADS1293_DRDYB_Struct
{
  ADS1293_drdybSrc_TypeDef src;
  BIT ctl0Mask;
  BIT ctl1Mask;
};

typedef struct ADS1293_SYNCOUTB_Struct ADS1293_SYNCOUTB_TypeDef;
struct ADS1293_SYNCOUTB_Struct
{
  ADS1293_syncbSrc_TypeDef src;
  BIT disable;
};

typedef struct ADS1293_AlarmFilter_Struct ADS1293_AlarmFilter_TypeDef;
struct ADS1293_AlarmFilter_Struct
{
  REG8 afilterLod;
  REG8 afilterOther;
};

typedef struct ADS1293_InSel_Struct ADS1293_InSel_TypeDef;
struct ADS1293_InSel_Struct
{
  BIT IN1; // IN1
  BIT IN2; // IN2
  BIT IN3; // IN3
  BIT IN4; // IN4
  BIT IN5; // IN5
  BIT IN6; // IN6
};

typedef struct CONF_ADS1293_Struct ADS1293_Typedef;
//struct CONF_ADS1293_Struct
//{
//  ADS1293_ADSSel_TypeDef ads_select;
//  ADS1293_OpMode_TypeDef opmode;
//  ADS1293_ChannelInpConfig_TypeDef in_ch1;
//  ADS1293_ChannelInpConfig_TypeDef in_ch2;
//  ADS1293_ChannelInpConfig_TypeDef in_ch3;
//  ADS1293_LodCnReg_TypeDef lod;
//  ADS1293_InSel_TypeDef LOD_Enable_In;
//  ADS1293_DetectCurrent_TypeDef detect_current;
//  ADS1293_LOD_AC_Config_TypeDef lod_ac_config;
//  ADS1293_CmDetConfig_TypeDef cmd;
//  ADS1293_In_TypeDef rldout_input;
//  ADS1293_WilsonRef_TypeDef wls;
//  ADS1293_InterRef_TypeDef InterRefVoltage;
//  ADS1293_OscClkConfig_TypeDef clock;
//  ADS1293_AFEResFreq_TypeDef afe_ch1;
//  ADS1293_AFEResFreq_TypeDef afe_ch2;
//  ADS1293_AFEResFreq_TypeDef afe_ch3;
//  ADS1293_AFEShdn_TypeDef shdn_ch1;
//  ADS1293_AFEShdn_TypeDef shdn_ch2;
//  ADS1293_AFEShdn_TypeDef shdn_ch3;
//  ADS1293_AFEFaultDet_TypeDef fault_ch1;
//  ADS1293_AFEFaultDet_TypeDef fault_ch2;
//  ADS1293_AFEFaultDet_TypeDef fault_ch3;
//  ADS1293_r2Rate_TypeDef r2;
//  ADS1293_R1R3DecRate_TypeDef r1r3_ch1;
//  ADS1293_R1R3DecRate_TypeDef r1r3_ch2;
//  ADS1293_R1R3DecRate_TypeDef r1r3_ch3;
//  ADS1293_ECGFilterEn_TypeDef ecgfilterEn_ch1;
//  ADS1293_ECGFilterEn_TypeDef ecgfilterEn_ch2;
//  ADS1293_ECGFilterEn_TypeDef ecgfilterEn_ch3;
//  ADS1293_DRDYB_TypeDef drdy;
//  ADS1293_SYNCOUTB_TypeDef sync;
//  ADS1293_maskError_TypeDef mask_err;
//  ADS1293_AlarmFilter_TypeDef alarm_filter;
//  ADS1293_ChCnfg_TypeDef loopRdBack;
//};

/* Exported variables --------------------------------------------------------*/
extern SPI_HandleTypeDef SpiHandle;
extern volatile BYTE ADS1293_ADCDataReady; // Data Ready status flag

/* Exported function prototypes ----------------------------------------------*/  
ADS1293_Status_TypeDef BSP_ADS1293_AFEClkDeInit(void);
extern ADS1293_Status_TypeDef BSP_ADS1293_Init(ADS1293_SelectConfig ADS_Mode);
extern ADS1293_Status_TypeDef BSP_ADS1293_ReadReg(REG8 addr, REG8 *value);
extern ADS1293_Status_TypeDef BSP_ADS1293_WriteReg(REG8 addr, REG8 *value);
extern ADS1293_Status_TypeDef BSP_ADS1293_AutoIncReadReg(REG8 addr, REG8 *buffer, uint8_t count);
extern ADS1293_Status_TypeDef BSP_ADS1293_SPIStreamReadReg(REG8 *buffer, uint8_t count);
extern ADS1293_Status_TypeDef BSP_ADS1293_SetOpMode(ADS1293_OpMode_TypeDef opmode);
extern ADS1293_Status_TypeDef BSP_ADS1293_StartConv(BIT stflag);
extern ADS1293_Status_TypeDef BSP_ADS1293_StartConvforALL(void);
extern  ADS1293_Status_TypeDef BSP_ADS1293_StopConvforALL(void);
extern ADS1293_Status_TypeDef BSP_ADS1293_ChInpConfig(ADS1293_ChannelInpConfig_TypeDef in_chx);
extern ADS1293_Status_TypeDef BSP_ADS1293_ChShutDown(ADS1293_Channel_TypeDef channel);
extern ADS1293_Status_TypeDef BSP_ADS1293_LODMode(ADS1293_LodCnReg_TypeDef *lod_mode);
extern ADS1293_Status_TypeDef BSP_ADS1293_LODEnable(ADS1293_InSel_TypeDef inputs);
extern ADS1293_Status_TypeDef BSP_ADS1293_LODCurrent(ADS1293_DetectCurrent_TypeDef detect_current);
extern ADS1293_Status_TypeDef BSP_ADS1293_LODACConfig(ADS1293_LOD_AC_Config_TypeDef lod_ac);
extern ADS1293_Status_TypeDef BSP_ADS1293_CMDConfig(ADS1293_CmDetConfig_TypeDef *config);
extern ADS1293_Status_TypeDef BSP_ADS1293_RLDOUTConfig(ADS1293_In_TypeDef input);
extern ADS1293_Status_TypeDef BSP_ADS1293_WILSONRefConfig(ADS1293_WilsonRef_TypeDef *config);
extern ADS1293_Status_TypeDef BSP_ADS1293_InterRefVoltageConfig(ADS1293_InterRef_TypeDef InterRefVoltage);
extern ADS1293_Status_TypeDef BSP_ADS1293_ClockConfig(ADS1293_OscClkConfig_TypeDef clock);
extern ADS1293_Status_TypeDef BSP_ADS1293_ClockStart(BIT startClk);
extern ADS1293_Status_TypeDef BSP_ADS1293_R1R3DecRateConfig(ADS1293_R1R3DecRate_TypeDef dec_rate);
extern ADS1293_Status_TypeDef BSP_ADS1293_ECGFilterEnable(ADS1293_ECGFilterEn_TypeDef filter_en);
extern ADS1293_Status_TypeDef BSP_ADS1293_AFEResFreqConfig(ADS1293_AFEResFreq_TypeDef afe_conf);
extern ADS1293_Status_TypeDef BSP_ADS1293_AFEShutDown(ADS1293_AFEShdn_TypeDef afe_shdn);
extern ADS1293_Status_TypeDef BSP_ADS1293_AFEFaultDetect(ADS1293_AFEFaultDet_TypeDef afe_fault);
extern ADS1293_Status_TypeDef BSP_ADS1293_DRDYBConfig(ADS1293_DRDYB_TypeDef drdy);
extern ADS1293_Status_TypeDef BSP_ADS1293_SYNCOUTBConfig(ADS1293_SYNCOUTB_TypeDef sync);
extern ADS1293_Status_TypeDef BSP_ADS1293_ChLoopRdBackConfig(ADS1293_ChCnfg_TypeDef *config);
extern ADS1293_Status_TypeDef BSP_ADS1293_ErrMask(ADS1293_maskError_TypeDef *config);
extern ADS1293_Status_TypeDef BSP_ADS1293_AlarmFilter(ADS1293_AlarmFilter_TypeDef alarm_filter);
extern ADS1293_Status_TypeDef BSP_ADS1293_ADCDataRead(BYTE *read_buf, BYTE count);
extern ADS1293_Status_TypeDef ADS1293_DRDYBInit(void);
extern void ADS1293_DRDYn_Callback(uint16_t GPIO_Pin);
extern ADS1293_Status_TypeDef BSP_ADS1293_ADSSel(ADS1293_ADSSel_TypeDef ads_select);
extern ADS1293_Status_TypeDef BSP_ADS1293_R2DecRateConfig(ADS1293_r2Rate_TypeDef r2);
extern ADS1293_Status_TypeDef BSP_ADS1293_AFEClkInit(uint32_t timer16_outFreq, uint32_t timer16_inFreq, uint32_t duty_cycle);
extern ADS1293_Status_TypeDef BSP_ADS1293_GetConfig(ADS1293_Typedef *config);
ADS1293_Status_TypeDef BSP_ADS1293_SetGeneralConfig(ADS1293_ADSSel_TypeDef ads_select);
extern ADS1293_Status_TypeDef BSP_ADS1293_GetStreamCount(uint8_t *count);
ADS1293_Status_TypeDef BSP_ADS1293_GetBuffer(eSensorBufferStatus_t eBuffer,uint8_t ChannelIndex , uint8_t *ads_data_array);

uint16_t DAL_ADS1293_GetSample(eSensorBufferStatus_t eBuffer, uint32 Index, uint8_t* Value, xSensorConfig* Config );
void BSP_ADS1293_IIR_Filter(uint32_t IIR_FilterIn, uint16_t* IIR_FilterOut, uint8_t ChannelIndex);

/* For Debug Purpose */
ADS1293_Status_TypeDef DAL_ADS1293_DebugPrintBufferSample(eSensorBufferStatus_t eBuffer, uint32_t u32SampleIndex, uint16_t u16FrameCounter );
ADS1293_Status_TypeDef DAL_ADS1293_DebugPrintLastBufferSample(eSensorBufferStatus_t eBuffer);
ADS1293_Status_TypeDef DAL_ADS1293_DebugPrintCompleteBuffer(eSensorBufferStatus_t eBuffer);
extern void BSP_ADS1293_PrintConfRegisters(void);

void BSP_ADS1293_Impedance(
                           uint8_t *COMMAND,
                           uint8_t *ADS1_ERROR_LOD, 
                           uint8_t *ADS2_ERROR_LOD, 
                           uint8_t *ADS3_ERROR_LOD, 
                           uint8_t *ADS4_ERROR_LOD, 
                           uint8_t *ADSx_LEVEL_VOLTAGE_LOD,
                           uint8_t *ADSx_conv_complete);

uint16_t BSP_ADS1293_GetZImpedance(uint8_t* u8pData);

void BSP_ADS1293_StartZImpedance(void);

void BSP_ADS1293_SetChannelMapping( ADS1293_SelectConfig ADS_Mode );


/* Private function prototypes -----------------------------------------------*/
ADS1293_Status_TypeDef BSP_ADS1293_ModifyReg(REG8 regAddr,  REG8 regSetMask, REG8 regClrMask);
ADS1293_Status_TypeDef BSP_ADS1293_SPIWrite(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count);
ADS1293_Status_TypeDef BSP_ADS1293_SPIRead(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count);
ADS1293_Status_TypeDef BSP_ADS1293_CSInit(void);
GPIO_PIN BSP_ADS1293_GetCSPin(void);

void BSP_ADS1293_LL_SPI_RW(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz);
ADS1293_Status_TypeDef BSP_ADS1293_LL_SPIRead(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count);

void BSP_ADS1293_RSTNPinInit(void);
void BSP_ADS1293_HardReset(void);

#if defined( ADS_ENABLE_SINUS_INJECT ) || defined( ADS_ENABLE_MASS_TEST_BENCH_SINUS_INJECT )  
void ADS1293_ManageSinusInjection(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__BSP_ADS1293_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
