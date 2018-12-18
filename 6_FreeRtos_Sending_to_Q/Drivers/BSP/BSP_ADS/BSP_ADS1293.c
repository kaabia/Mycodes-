/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_ADS1293.h"
#include "BCP.h"

/* Private macro -------------------------------------------------------------*/
#define toRegPos(data, bitMask) ((REG8)((data) << ADS1293_POSITION_VAL(bitMask))) // Put a given data in the correspondent register position
#define isEqual(regData, newData, bitMask) (((REG8)((regData)&(bitMask))) == ((REG8)(newData)))


#if defined(USE_ADS_16BIT)
#define ADS_DATA_SIZE    2
#define ADS_SAMPLE_SIZE    (ADS_CHANNEL_COUNT * ADS_DATA_SIZE)
#elif defined(USE_ADS_24BIT)
#define ADS_DATA_SIZE    3
#define ADS_SAMPLE_SIZE    (ADS_CHANNEL_COUNT * ADS_DATA_SIZE)
#else
#error "ADS CHANNEL DATAE SIZE NOT DEFINED"
#endif

#define READ_DATA_ADDR (ADS1293_DATA_CH1_ECG_H_REG | ADS1293_RD_BIT)


/* Private variables ---------------------------------------------------------*/

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;
TIM_HandleTypeDef adsTimHandle;

ADS1293_ADSSel_TypeDef selected_ads = AFE_CSn_FRONT; // Default selected ADS

extern QueueHandle_t xQueueSensorBufferStatus;

static eSensorBufferStatus_t eBufferStatus;

extern xBCPMessage_t xSensorBCPBufferYing[];
extern xBCPMessage_t xSensorBCPBufferYang[];

extern uint8_t ADS_ChannelMapping[];


/**
* @brief  Configures the SPI master port assigned to the ADS1293. Must be called before calling any of the other functions
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_Init(ADS1293_SelectConfig ADS_Mode)
{  
  
  ADS1293_Status_TypeDef err;
  
  /* Set channel mapping */
  BSP_ADS1293_SetChannelMapping(ADS_Mode);
  
  // Init the ADS1293 Reset pin
  BSP_ADS1293_RSTNPinInit();

  // Init SPI peripheral
  err = (ADS1293_Status_TypeDef)DRV_SPI1_Init(&SpiHandle);
  
#ifdef ADS_ENABLE_SINUS_INJECT
  BSP_ADS1293_Detect_Test_Mode();
#endif
  
  if( err == HAL_OK )
  {
    // Init the AFE CLCK
    BSP_ADS1293_AFEClkInit(380000, 10000000, 50);   
    //BSP_ADS1293_AFEClkInit(400000, 10000000, 50);   
    
    // Init the interrupt pin for the ADS RDYB input 
    ADS1293_DRDYBInit();
    
    // Init the CSs output
    BSP_ADS1293_CSInit();
    
    // Hard Reset ADS1293
    BSP_ADS1293_HardReset();
  }
  
  return err;
}


/**
* @brief  Set ADS1293 operation mode.
* @param  opmode: specifies the operation mode.
*   This parameter can be a combination of the following values:
*     @arg STD_MODE: STANDBY mode deactivated, PWR_DOWN mode deacivated
*     @arg STDBY_En: STANDBY mode activated
*     @arg PWRDWN_En: PWR_DOWN mode acivated 
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_SetOpMode(ADS1293_OpMode_TypeDef opMode)
{
  REG8 mode = toRegPos(opMode, (ADS1293_CONFIG_PWR_DOWN|ADS1293_CONFIG_STANDBY)); 
  return BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, mode, (ADS1293_CONFIG_PWR_DOWN|ADS1293_CONFIG_STANDBY));
}

/**
* @brief  Start conversion.
* @param  stflag: Start/Stop flag
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_StartConv(BIT stflag)
{
  REG8 flag = toRegPos(stflag, ADS1293_CONFIG_START_CON); 
  return BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
}


ADS1293_Status_TypeDef BSP_ADS1293_StartConvforALL(void)
{
  ADS1293_Status_TypeDef err;
  REG8 flag = toRegPos(START_CONVERSATION, ADS1293_CONFIG_START_CON); 
  
  selected_ads = AFE_CSn_FRONT;
  err = BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD1;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD2;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD3;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0); // Enable and set EXTI line 3 Interrupt to the lowest priority
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  
  
  return err;
}


ADS1293_Status_TypeDef BSP_ADS1293_StopConvforALL(void)
{
  ADS1293_Status_TypeDef err;
  err = ADS1293_OK;
  
  /*
  REG8 flag = toRegPos(STOP_CONVERSATION, ADS1293_CONFIG_START_CON); 
  
  selected_ads = AFE_CSn_FRONT;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD1;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD2;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  selected_ads = AFE_CSn_PRECORD3;
  err |= BSP_ADS1293_ModifyReg(ADS1293_CONFIG_REG, flag, ADS1293_CONFIG_START_CON);
  
  */
  
  HAL_NVIC_DisableIRQ(EXTI3_IRQn);
  
  
  return err;
}




/**
* @brief  Select channel positive-input and negative-input pins.
* @param  channel: The channel to configure
* @param  neg_in: Negative terminal input
* @param  pos_in: Positive terminal input
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ChInpConfig(ADS1293_ChannelInpConfig_TypeDef in_chx)
{
  REG8 flexChxCnRegAddr;
  REG8 flexChxCnRegSetMask = toRegPos(in_chx.pos_in, ADS1293_FLEX_CHx_CN_POSx)      \
    | toRegPos(in_chx.neg_in, ADS1293_FLEX_CHx_CN_NEGx);
  
  switch (in_chx.channel)
  {
  case CH1:
    flexChxCnRegAddr = ADS1293_FLEX_CH1_CN_REG;
    break;
  case CH2:
    flexChxCnRegAddr = ADS1293_FLEX_CH2_CN_REG;
    break;
  case CH3:
    flexChxCnRegAddr = ADS1293_FLEX_CH3_CN_REG;
    break;
  case PACE:
    flexChxCnRegAddr = ADS1293_FLEX_PACE_CN_REG;
    break;   
  default:
    return ADS1293_ERROR;
  }
  
  return BSP_ADS1293_ModifyReg(flexChxCnRegAddr, flexChxCnRegSetMask, (ADS1293_FLEX_CHx_CN_POSx|ADS1293_FLEX_CHx_CN_NEGx));
}

/**
* @brief  Shut down unused channel signal path.
* @param  channel: The channel to configure
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ChShutDown(ADS1293_Channel_TypeDef channel)
{
  REG8 afeShdnCnClrMask;
  
  switch (channel)
  {
  case CH1:
    afeShdnCnClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH1|ADS1293_AFE_SHDN_CN_SHDN_INA_CH1;
    break;
  case CH2:
    afeShdnCnClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH2|ADS1293_AFE_SHDN_CN_SHDN_INA_CH2;
    break;
  case CH3:
    afeShdnCnClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH3|ADS1293_AFE_SHDN_CN_SHDN_INA_CH3;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  return BSP_ADS1293_ModifyReg(ADS1293_AFE_SHDN_CN_REG, afeShdnCnClrMask, afeShdnCnClrMask);
}

/**
* @brief  Lead-Off Detection mode configuration
* @param  lod_mode: Lead-Off Detect mode configuration
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_LODMode(ADS1293_LodCnReg_TypeDef *lod_mode)
{
  REG8 lodCnRegVal;
  
  if(!lod_mode)
    return ADS1293_ERROR;
  
  lodCnRegVal = toRegPos(lod_mode->aclvl_lod, ADS1293_LOD_CN_ACLVL_LOD)                 \
    | toRegPos(lod_mode->selac_lod, ADS1293_LOD_CN_SELAC_LOD)       \
      | toRegPos(lod_mode->shdn_lod, ADS1293_LOD_CN_SHDN_LOD)         \
        | toRegPos(lod_mode->acad_lod, ADS1293_LOD_CN_ACAD_LOD); 
  
  return BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &lodCnRegVal);
}

/**
* @brief  
* @param  
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_LODEnable(ADS1293_InSel_TypeDef inputs)
{
  REG8 lodEnRegVal;
  
  lodEnRegVal = toRegPos(inputs.IN1, ADS1293_LOD_EN_EN_LOD_1)                 \
    | toRegPos(inputs.IN2, ADS1293_LOD_EN_EN_LOD_2)       \
      | toRegPos(inputs.IN3, ADS1293_LOD_EN_EN_LOD_3)       \
        | toRegPos(inputs.IN4, ADS1293_LOD_EN_EN_LOD_4)       \
          | toRegPos(inputs.IN5, ADS1293_LOD_EN_EN_LOD_5)       \
            | toRegPos(inputs.IN6, ADS1293_LOD_EN_EN_LOD_6);
  
  return BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &lodEnRegVal);
}

/**
* @brief  Shut down unused channel signal path.
* @param  detect_current: Lead-Off detection current
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_LODCurrent(ADS1293_DetectCurrent_TypeDef detect_current)
{  
  uint8_t tmp = detect_current;
  return BSP_ADS1293_WriteReg(ADS1293_LOD_CURRENT_REG, &tmp);
}

/**
* @brief  AC Lead-Off Detect configuration
* @param  freqDiv: AC lead off test frequency division factor
* @param  clkDiv: AC lead off clock divider ratio
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_LODACConfig(ADS1293_LOD_AC_Config_TypeDef lod_ac)
{  
  REG8 lodAcCnRegVal = toRegPos(lod_ac.freqDiv, ADS1293_LOD_AC_CN_ACDIV_FACTOR)     \
    | toRegPos(lod_ac.clkDiv, ADS1293_LOD_AC_CN_ACDIV_LOD);
  return BSP_ADS1293_WriteReg(ADS1293_LOD_AC_CN_REG, &lodAcCnRegVal);
}

/**
* @brief  Enable the common-mode detector on the given input pins, configure the bandwidth mode and the capacitive load drive capability
* @param  
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_CMDConfig(ADS1293_CmDetConfig_TypeDef *config)
{  
  REG8 cmDetCnRegVal = toRegPos(config->cmd_bw, ADS1293_CMDET_CN_CMDET_BW)                 \
    | toRegPos(config->cmd_capdrive, ADS1293_CMDET_CN_CMDET_CAPDRIVE);  
  
  if(BSP_ADS1293_WriteReg(ADS1293_CMDET_CN_REG, &cmDetCnRegVal))
    return ADS1293_ERROR;
  
  return BSP_ADS1293_WriteReg(ADS1293_CMDET_EN_REG, (uint8_t *)&config->inputs);
}

/**
* @brief  RLD amplifier output pin configuration
* @param input: output pin to be connected to the RLD amplifier output (IN1...IN6), or to disconnect the RLD output
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_RLDOUTConfig(ADS1293_In_TypeDef input)
{
  REG8 rldCnRegSetMask = toRegPos(input, ADS1293_RLD_CN_SELRLD) | ((REG8)1 << 3);
  return BSP_ADS1293_ModifyReg(ADS1293_RLD_CN_REG, rldCnRegSetMask, ADS1293_RLD_CN_SELRLD);
}  

/**
* @brief  WILSON/ Goldberger reference configuration
* @param config: Wilson Reference configuartion struct
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_WILSONRefConfig(ADS1293_WilsonRef_TypeDef *config)
{
  REG8 wilsonEnxRegVal;
  
  REG8 wilsonCnVal = toRegPos(config->goldint, ADS1293_WILSON_CN_GOLDINT)        \
    | toRegPos(config->wilsonint, ADS1293_WILSON_CN_WILSONINT);
  
  wilsonEnxRegVal = toRegPos(config->selWilson1, ADS1293_WILSON_ENx_SELWILSONx); 
  if(BSP_ADS1293_WriteReg(ADS1293_WILSON_EN1_REG, &wilsonEnxRegVal))
    return ADS1293_ERROR;
  
  wilsonEnxRegVal = toRegPos(config->selWilson2, ADS1293_WILSON_ENx_SELWILSONx);  
  if(BSP_ADS1293_WriteReg(ADS1293_WILSON_EN2_REG, &wilsonEnxRegVal))
    return ADS1293_ERROR;
  
  wilsonEnxRegVal = toRegPos(config->selWilson3, ADS1293_WILSON_ENx_SELWILSONx); 
  if(BSP_ADS1293_WriteReg(ADS1293_WILSON_EN3_REG, &wilsonEnxRegVal))
    return ADS1293_ERROR;
  
  return BSP_ADS1293_WriteReg(ADS1293_WILSON_CN_REG, &wilsonCnVal);
} 

/**
* @brief  WILSON/Goldberger reference configuration
* @param shdnInterRef: Shut down or enable the internal 2.4-V reference voltage
* @param shdnCmRef: Shut down or enable the common-mode and right-leg drive reference voltage circuitry
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_InterRefVoltageConfig(ADS1293_InterRef_TypeDef InterRefVoltage)
{
  REG8 refCnVal = toRegPos(InterRefVoltage.shdnCmRef, ADS1293_REF_CN_SHDN_CMREF)        \
    | toRegPos(InterRefVoltage.shdnInterRef, ADS1293_REF_CN_SHDN_REF);
  return BSP_ADS1293_WriteReg(ADS1293_REF_CN_REG, &refCnVal);
}

/**
* @brief  Clock Source and Output Clock Control
* @param shdnOsc: Select clock source
*     @arg 0: Use internal clock with external crystal on XTAL1 and XTAL2 pins (default)
*     @arg 1: Shut down internal oscillator and use external clock from CLK pin
* @param enClkOut:Enable CLK pin output driver
*     @arg 0: Clock output driver disabled (default)
*     @arg 1: Clock output driver enabled
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ClockConfig(ADS1293_OscClkConfig_TypeDef clock)
{
  REG8 oscCnRegSetMask = toRegPos(clock.shdnOsc, ADS1293_OSC_CN_SHDN_OSC)        \
    | toRegPos(clock.enClkOut, ADS1293_OSC_CN_EN_CLKOUT);
  
  if(BSP_ADS1293_ModifyReg(ADS1293_OSC_CN_REG, oscCnRegSetMask, (ADS1293_OSC_CN_SHDN_OSC|ADS1293_OSC_CN_EN_CLKOUT)))
    return ADS1293_ERROR;
  
  HAL_Delay(10);
  
  oscCnRegSetMask = oscCnRegSetMask | toRegPos(0x01, ADS1293_OSC_CN_STRTCLK);
  
  return BSP_ADS1293_ModifyReg(ADS1293_OSC_CN_REG, oscCnRegSetMask, (ADS1293_OSC_CN_SHDN_OSC|ADS1293_OSC_CN_EN_CLKOUT|ADS1293_OSC_CN_STRTCLK ) );
}

/**
* @brief  Start the clock
* @param startClk: Select clock source
*     @arg 0: Clock to digital disabled (default)
*     @arg 1: Enable clock to digital
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ClockStart(BIT startClk)
{
  REG8 oscCnRegSetMask = toRegPos(startClk, ADS1293_OSC_CN_STRTCLK);
  return BSP_ADS1293_ModifyReg(ADS1293_OSC_CN_REG, oscCnRegSetMask, ADS1293_OSC_CN_STRTCLK);
}

/**
* @brief  Set R2 Decimation Rate
* @param r2: R2 decimation rate
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_R2DecRateConfig(ADS1293_r2Rate_TypeDef r2)
{
  REG8 r2RateRegVal = toRegPos(r2, ADS1293_R2_RATE_R2_RATE);
  return BSP_ADS1293_WriteReg(ADS1293_R2_RATE_REG, &r2RateRegVal);
}


/**
* @brief  Set channel R3 Decimation Rate 
* @param r1: Channel Pace data rate 
* @param r3: Channel R3-decimation rate
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_R1R3DecRateConfig(ADS1293_R1R3DecRate_TypeDef dec_rate)
{
  REG8 r1RateRegClrMask;
  REG8 r1RateRegSetMask; 
  REG8 r3RateChxRegAddr;
  
  switch (dec_rate.channel)
  {
  case CH1:
    r1RateRegClrMask = ADS1293_R1_RATE_R1_RATE_CH1;
    r3RateChxRegAddr = ADS1293_R3_RATE1_REG;
    break;
  case CH2:
    r1RateRegClrMask = ADS1293_R1_RATE_R1_RATE_CH2;
    r3RateChxRegAddr = ADS1293_R3_RATE2_REG;
    break;
  case CH3:
    r1RateRegClrMask = ADS1293_R1_RATE_R1_RATE_CH3;
    r3RateChxRegAddr = ADS1293_R3_RATE3_REG;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  if(BSP_ADS1293_WriteReg(r3RateChxRegAddr, (uint8_t *)&dec_rate.r3))
    return ADS1293_ERROR;
  
  r1RateRegSetMask = toRegPos(dec_rate.r1, r1RateRegClrMask);
  
  return BSP_ADS1293_ModifyReg(ADS1293_P_DRATE_REG, r1RateRegSetMask, r1RateRegClrMask);
}

/**
* @brief  Enable/Disable the ECG channel filetr
* @param  channel: The channel to configure
* @param  enable: Enable/Disable flag
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ECGFilterEnable(ADS1293_ECGFilterEn_TypeDef filter_en)
{
  REG8 disEFilterRegClrMask;
  REG8 disEFilterRegSetMask;
  
  switch (filter_en.channel)
  {
  case CH1:
    disEFilterRegClrMask = ADS1293_DIS_EFILTER_DIS_E1;
    break;
  case CH2:
    disEFilterRegClrMask = ADS1293_DIS_EFILTER_DIS_E2;
    break;
  case CH3:
    disEFilterRegClrMask = ADS1293_DIS_EFILTER_DIS_E3;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  disEFilterRegSetMask =  toRegPos(filter_en.enable, disEFilterRegClrMask);
  
  return BSP_ADS1293_ModifyReg(ADS1293_DIS_EFILTER_REG, disEFilterRegSetMask, disEFilterRegClrMask);
}

/**
* @brief  Analog Front-End Frequency and Resolution
* @param  channel: The channel to configure
* @param  clkFreq: The selected channel Clock frequency 
* @param  hiResEnable: High-resolution mode for Channel 2 instrumentation amplifier
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_AFEResFreqConfig(ADS1293_AFEResFreq_TypeDef afe_conf)
{
  REG8 afeResRegClrMask;
  REG8 afeResRegHiResClrMask;
  REG8 afeResRegFsHighClrMask;
  REG8 afeResRegSetMask;
  
  switch (afe_conf.channel)
  {
  case CH1:
    afeResRegHiResClrMask = ADS1293_AFE_RES_EN_HIRES_CH1;
    afeResRegFsHighClrMask = ADS1293_AFE_RES_FS_HIGH_CH1;
    break;
  case CH2:
    afeResRegHiResClrMask = ADS1293_AFE_RES_EN_HIRES_CH2;
    afeResRegFsHighClrMask = ADS1293_AFE_RES_FS_HIGH_CH2;
    break;
  case CH3:
    afeResRegHiResClrMask = ADS1293_AFE_RES_EN_HIRES_CH3;
    afeResRegFsHighClrMask = ADS1293_AFE_RES_FS_HIGH_CH3;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  afeResRegSetMask = toRegPos(afe_conf.clkFreq, afeResRegFsHighClrMask)                 \
    | toRegPos(afe_conf.hiResEnable, afeResRegHiResClrMask);
  
  afeResRegClrMask = (afeResRegFsHighClrMask|afeResRegHiResClrMask);
  
  return BSP_ADS1293_ModifyReg(ADS1293_AFE_RES_REG, afeResRegSetMask, afeResRegClrMask);
}

/**
* @brief  Analog Front-End Shutdown Control
* @param  channel: The channel to configure
* @param  shdn_ina: Flag for Shut down the instrumentation amplifie 
* @param  shdn_sdm: Flag for Shut down the sigma-delta modulator 
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_AFEShutDown(ADS1293_AFEShdn_TypeDef afe_shdn)
{
  
  REG8 afeShdnCnRegClrMask;
  REG8 afeShdnCnRegInaClrMask;
  REG8 afeShdnCnRegSdmClrMask;
  REG8 afeShdnCnRegSetMask;
  
  switch (afe_shdn.channel)
  {
  case CH1:
    afeShdnCnRegInaClrMask = ADS1293_AFE_SHDN_CN_SHDN_INA_CH1;
    afeShdnCnRegSdmClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH1;
    break;
  case CH2:
    afeShdnCnRegInaClrMask = ADS1293_AFE_SHDN_CN_SHDN_INA_CH2;
    afeShdnCnRegSdmClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH2;
    break;
  case CH3:
    afeShdnCnRegInaClrMask = ADS1293_AFE_SHDN_CN_SHDN_INA_CH3;
    afeShdnCnRegSdmClrMask = ADS1293_AFE_SHDN_CN_SHDN_SDM_CH3;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  afeShdnCnRegSetMask = toRegPos(afe_shdn.shdn_sdm, afeShdnCnRegSdmClrMask)                 \
    | toRegPos(afe_shdn.shdn_ina, afeShdnCnRegInaClrMask);
  
  afeShdnCnRegClrMask = (afeShdnCnRegSdmClrMask|afeShdnCnRegInaClrMask);
  
  return BSP_ADS1293_ModifyReg(ADS1293_AFE_SHDN_CN_REG, afeShdnCnRegSetMask, afeShdnCnRegClrMask);
}

/**
* @brief  Analog Front-End Fault Detection Control
* @param  channel: The channel to configure
* @param  enable: Flag for Disable/Enable the instrumentation amplifier fault detection on the selected channel
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_AFEFaultDetect(ADS1293_AFEFaultDet_TypeDef afe_fault)
{
  REG8 afeFaultCnRegClrMask;
  REG8 afeFaultCnRegSetMask;
  
  switch (afe_fault.channel)
  {
  case CH1:
    afeFaultCnRegClrMask = ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH1;
    break;
  case CH2:
    afeFaultCnRegClrMask = ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH2;
    break;
  case CH3:
    afeFaultCnRegClrMask = ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH3;
    break; 
  default:
    return ADS1293_ERROR;
  }
  
  afeFaultCnRegSetMask = toRegPos(afe_fault.enable, afeFaultCnRegClrMask);
  return BSP_ADS1293_ModifyReg(ADS1293_AFE_FAULT_CN_REG, afeFaultCnRegSetMask, afeFaultCnRegClrMask);
}

/**
* @brief  Data Ready Pin Source
* @param  src: Select channel to drive the DRDYB pin
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_DRDYBConfig(ADS1293_DRDYB_TypeDef drdy)
{
  REG8 drdybSrcRegSetMask = toRegPos(drdy.src, ADS1293_DRDYB_SRC_DRDYB_SRC);
  REG8 maskDrdybRegSetMask = toRegPos(drdy.ctl1Mask, ADS1293_MASK_DRDYB_DRDYBMASK_CTL1)   \
    | toRegPos(drdy.ctl0Mask, ADS1293_MASK_DRDYB_DRDYBMASK_CTL0);
  if(BSP_ADS1293_WriteReg(ADS1293_DRDYB_SRC_REG, &drdybSrcRegSetMask))
    return ADS1293_ERROR;
  
  return BSP_ADS1293_WriteReg(ADS1293_MASK_DRDYB_REG, &maskDrdybRegSetMask);
}

/**
* @brief  Syncb In/Out Pin Configuration
* @param  enable: Enable/Disable SYNCOUTB pin
*     @arg 0: Driver enabled and pin configured as output
*     @arg 1: Driver disabled and pin configured as input (default)
*     Note: Bit should be set to 1 for slave devices.
* @param  src: Select channel to drive the SYNCOUTB pin
*         Note: Choose the slowest pace or ECG channel as source.
*               Bits[5:0] must be cleared to 0 for (src value => 'SYNCB_CN_SYNCB_SRC_NONE') slave devices.
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_SYNCOUTBConfig(ADS1293_SYNCOUTB_TypeDef sync)
{
  REG8 syncbSrcRegSetMask = toRegPos(sync.src, ADS1293_SYNCB_CN_SYNCB_SRC)    \
    | toRegPos(sync.disable, ADS1293_SYNCB_CN_DIS_SYNCBOUT);
  return BSP_ADS1293_WriteReg(ADS1293_SYNCOUTB_SRC_REG, &syncbSrcRegSetMask);
}

/**
* @brief  Configure Channel for Loop Read Back Mode
* @param  config: configuration structure
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ChLoopRdBackConfig(ADS1293_ChCnfg_TypeDef *config)
{
  REG8 chCnfgRegSetMask = toRegPos(config->e3_en, ADS1293_CH_CNFG_E3_EN)         \
    | toRegPos(config->e2_en, ADS1293_CH_CNFG_E2_EN)       \
      | toRegPos(config->e1_en, ADS1293_CH_CNFG_E1_EN)       \
        | toRegPos(config->p3_en, ADS1293_CH_CNFG_P3_EN)       \
          | toRegPos(config->p2_en, ADS1293_CH_CNFG_P2_EN)       \
            | toRegPos(config->p1_en, ADS1293_CH_CNFG_P1_EN)       \
              | toRegPos(config->sts_en, ADS1293_CH_CNFG_STS_EN);
  return BSP_ADS1293_WriteReg(ADS1293_CH_CNFG_REG, &chCnfgRegSetMask);
}

/**
* @brief  Configure Channel for Loop Read Back Mode
* @param  config: configuration structure
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ErrMask(ADS1293_maskError_TypeDef *config)
{
  REG8 maskErrRegSetMask = toRegPos(config->mask_syncedgeer, ADS1293_MASK_ERR_MASK_SYNCEDGEERR)  \
    | toRegPos(config->mask_ch3err, ADS1293_MASK_ERR_MASK_CH3ERR)          \
      | toRegPos(config->mask_ch2err, ADS1293_MASK_ERR_MASK_CH2ERR)          \
        | toRegPos(config->mask_ch1err, ADS1293_MASK_ERR_MASK_CH1ERR)          \
          | toRegPos(config->mask_leadoff, ADS1293_MASK_ERR_MASK_LEADOFF)        \
            | toRegPos(config->mask_batlow, ADS1293_MASK_ERR_MASK_BATLOW)          \
              | toRegPos(config->mask_rldrail, ADS1293_MASK_ERR_MASK_RLDRAIL)        \
                | toRegPos(config->mask_cmor, ADS1293_MASK_ERR_MASK_CMOR);
  
  return BSP_ADS1293_WriteReg(ADS1293_MASK_ERR_REG, &maskErrRegSetMask);
}

/**
* @brief  Digital Filter for Analog Alarm Signals
* @param  afilterLod: Number of consecutive lead off alarm signal counts +1 before ALARMB is asserted, 0011: (default)
* @param  afilterOther: Number of consecutive analog alarm signal counts +1 before ALARMB is asserted, 0011: (default)
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_AlarmFilter(ADS1293_AlarmFilter_TypeDef alarm_filter)
{
  REG8 alarmFilterRegSetMask = toRegPos(alarm_filter.afilterOther, ADS1293_ALARM_FILTER_AFILTER_OTHER)  \
    | toRegPos(alarm_filter.afilterLod, ADS1293_ALARM_FILTER_AFILTER_LOD);
  return BSP_ADS1293_WriteReg(ADS1293_ALARM_FILTER_REG, &alarmFilterRegSetMask);
}

/**
* @brief  Read adc output into read_buf
* @param  read_buf: Read buffer
* @param  count: bytes to read
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ADCDataRead(uint8_t *read_buf, uint8_t count)
{
  return BSP_ADS1293_AutoIncReadReg(0x37, read_buf, count);  
}

/**
* @brief  Configures EXTI lines 3 (connected to PA.3 pin) in interrupt mode
* @param  None
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef ADS1293_DRDYBInit(void)
{
  CONF_ADS_PIN_AFE_DRDYn_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = CONF_ADS_PIN_AFE_DRDYn;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CONF_ADS_PIN_AFE_DRDYn_GPIO, &GPIO_InitStruct);
  
  return ADS1293_OK;		
}

/**
* @brief EXTI line 3 detection callback
* @param GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void ADS1293_DRDYn_Callback(uint16_t GPIO_Pin)
{
  static uint32_t u32BufferDataPointer = 0; 
  static eSensorBufferInUse_t eCurrentBuffer = SENSOR_BUFFER_1;
  static BaseType_t xHigherPriorityTaskWoken;
  
  uint8_t Data[ADS_CHANNEL_COUNT * 3];
  uint8_t u8pProcessedData[ADS_CHANNEL_COUNT * 3];
  xBCPMessage_t* xMessage;
  uint8_t i;
  
  #ifdef PREGNAUTE
  uint32_t DATA24Bits[ADS_CHANNEL_COUNT * 3];
  uint32_t OutDATA24Bits[ADS_CHANNEL_COUNT * 3];
  uint32_t IIR_FilterIn = 0;
  uint16_t IIR_FilterOut = 0;
  #endif  
  
  if (GPIO_Pin == CONF_ADS_PIN_AFE_DRDYn)
  {
    /* We have not woken a task at the start of the ISR. */
    xHigherPriorityTaskWoken = pdFALSE;
   
    // Read ADS data with LL read  
    BSP_ADS1293_LL_SPIRead(READ_DATA_ADDR, &Data[0], GPIOC, GPIO_PIN_1, 9);
    BSP_ADS1293_LL_SPIRead(READ_DATA_ADDR, &Data[9], GPIOC, GPIO_PIN_2, 9);
    BSP_ADS1293_LL_SPIRead(READ_DATA_ADDR, &Data[18], GPIOC, GPIO_PIN_3, 9);
    BSP_ADS1293_LL_SPIRead(READ_DATA_ADDR, &Data[27], GPIOC, GPIO_PIN_4, 9);

#ifdef USE_IMPEDANCE    
    BSP_ADS1293_Impedance(
                          &vADSx_StartImpedance, 
                          &vADS1_ERROR_LOD, 
                          &vADS2_ERROR_LOD, 
                          &vADS3_ERROR_LOD, 
                          &vADS4_ERROR_LOD, 
                          vADSx_LEVEL_VOLTAGE_LOD, 
                          &vADSx_conv_complete);
#endif  
    
#if defined( PREGNAUTE )
    for(i = 0; i< ADS_CHANNEL_COUNT; i++ )
    {
      // get the 24 bits data
      DATA24Bits[i] = (uint32_t)(Data[i*3] << 16) | (uint32_t)(Data[1+(i*3)] << 8) | (uint32_t)Data[2+(i*3)];
      // applicate the HP filter
      IIR_FilterIn = DATA24Bits[i];
      BSP_ADS1293_IIR_Filter(IIR_FilterIn, &IIR_FilterOut, i);
      // OutDATA24Bits[i] = (IIR_FilterOut >> 3);
      OutDATA24Bits[i] = IIR_FilterOut;
      Data[i*2] = (uint8_t)(OutDATA24Bits[i] >> 8);
      Data[1+(i*2)] = (uint8_t)OutDATA24Bits[i];
    }
    for(i = 0; i< ADS_CHANNEL_COUNT; i++ )
    {
        u8pProcessedData[(i*2)] = Data[ (ADS_ChannelMapping[i]*2)+1 ];
        u8pProcessedData[(i*2)+1] = Data[ (ADS_ChannelMapping[i]*2) ]; 
    }
#elif defined( USE_ADS_16BIT )
      for(i = 0; i< ADS_CHANNEL_COUNT; i++ )
      {
          Data[i * 2] = Data[(i * 3)];  
          Data[ (i * 2) + 1] = Data[(i * 3)+ 1];    
      }   
      for(i = 0; i< ADS_CHANNEL_COUNT; i++ )
      {
          u8pProcessedData[(i*2)] = Data[ (ADS_ChannelMapping[i]*2)+1 ];
          u8pProcessedData[(i*2)+1] = Data[ (ADS_ChannelMapping[i]*2) ]; 
      }
#elif defined( USE_ADS_24BIT )    
      for(i = 0; i< ADS_CHANNEL_COUNT; i++ )
      {
          u8pProcessedData[ (i*3)] = Data[ (ADS_ChannelMapping[i]*3)+2 ];
          u8pProcessedData[ (i*3)+1] = Data[ (ADS_ChannelMapping[i]*3)+1 ];
          u8pProcessedData[ (i*3)+2] = Data[ (ADS_ChannelMapping[i]*3) ];  
      }
#else
    // Nothing to do for 24 bit configuration  
#endif  
    
    /* Init buffer sensor if needed */
      if( u32BufferDataPointer == 0 )
      {
          if( eCurrentBuffer == SENSOR_BUFFER_1 )
          {
            BCP_InitSensorBuffer( SENSOR_BUFFER_YING_FULL ); 
          }
          else
          {
            BCP_InitSensorBuffer( SENSOR_BUFFER_YANG_FULL );     
          }
      }
    
    // Compute BCP message index  
    i = u32BufferDataPointer / ADS_SAMPLES_COUNT_PER_MQTT_MESSAGE;
    // Set BCP Message  
    if( eCurrentBuffer == SENSOR_BUFFER_1 )
    {
        xMessage = &xSensorBCPBufferYing[i];
    }
    else
    {
        xMessage = &xSensorBCPBufferYang[i];
    }
    
    // Append data to BCP Message 
    BCP_AppendSensorSampleToPayload( xMessage, BCP_SENSOR_ID_ECG, &u8pProcessedData[0], ADS_SAMPLE_SIZE );

    /* Increment Sample Index buffer from 0 to ADS_BUFFER_SIZE */
    u32BufferDataPointer++; 
    
    /* Check out percentage of buffer */
    switch( u32BufferDataPointer )
    {
      
    case  ADS_BUFFER_AT_50_PERCENT :
      if( eCurrentBuffer == SENSOR_BUFFER_1 )
        eBufferStatus = SENSOR_BUFFER_YING_ONE_SECOND_DATA_COMPLETED; 
      else
        eBufferStatus = SENSOR_BUFFER_YANG_ONE_SECOND_DATA_COMPLETED;      
      if( xQueueSensorBufferStatus != 0 )
        xQueueSendToFrontFromISR( xQueueSensorBufferStatus, &eBufferStatus, &xHigherPriorityTaskWoken );     
      break;

    case  ADS_BUFFER_SIZE :
      /* Buffer is Full -> Reset Pointer */
      u32BufferDataPointer = 0;
      if( eCurrentBuffer == SENSOR_BUFFER_1 )
      {
        eCurrentBuffer = SENSOR_BUFFER_2;
        eBufferStatus = SENSOR_BUFFER_YING_FULL; 
      }
      else
      {
        eCurrentBuffer = SENSOR_BUFFER_1;
        eBufferStatus = SENSOR_BUFFER_YANG_FULL;    
      }
      if( xQueueSensorBufferStatus != 0 )
        xQueueSendToFrontFromISR( xQueueSensorBufferStatus, &eBufferStatus, &xHigherPriorityTaskWoken );     
      break;

    default :
        break;
    }
    
    /* Did sending to the queue unblock a higher priority task? */
    if( xHigherPriorityTaskWoken )
    {
      /* Actual macro used here is port specific. */
      //taskYIELD_FROM_ISR ();
    }
    
    #if defined ( ADS_ENABLE_SINUS_INJECT ) || defined ( ADS_ENABLE_MASS_TEST_BENCH_SINUS_INJECT )
        ADS1293_ManageSinusInjection();
   #endif
    
    
    
    
  }
  
}





/**
* @brief Debug Print
* @param eBuffer
* @param u8pString
* @retval 
*/
ADS1293_Status_TypeDef DAL_ADS1293_DebugPrintLastBufferSample(eSensorBufferStatus_t eBuffer)
{ 
  return DAL_ADS1293_DebugPrintBufferSample(eBuffer, ADS_BUFFER_SIZE-1, 0);
}


/**
* @brief Debug Print
* @param eBuffer
* @param u8pString
* @retval 
*/
ADS1293_Status_TypeDef DAL_ADS1293_DebugPrintCompleteBuffer(eSensorBufferStatus_t eBuffer)
{
  uint32_t i=0;
  uint16_t u16FrameCounter = 0;
  
  for (i=0; i< ADS_BUFFER_SIZE; i++ )
  {
    DAL_ADS1293_DebugPrintBufferSample(eBuffer, i , u16FrameCounter );
    u16FrameCounter++;
  }
  
  return ADS1293_OK;
}

/**
* @brief AFE_CLK  signal configuration
* @param timer16_outFreq: PWM Frequency (Timer TIM16 output frequency)
* @param timer16_inFreq: Timer TIM16 input frequency
* @param duty_cycle: PWM Duty Cycle
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_AFEClkInit(uint32_t timer16_outFreq, uint32_t timer16_inFreq, uint32_t duty_cycle)
{
  GPIO_InitTypeDef GPIO_InitStruct;  

  TIM_OC_InitTypeDef sConfig;
  uint32_t prescaler_value, periode_value, pulse_value;
  
  if(!timer16_outFreq)
    return ADS1293_ERROR;
  
  __HAL_RCC_TIM16_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  prescaler_value = (uint32_t)(SystemCoreClock / timer16_inFreq) - 1;
  periode_value = (uint32_t)((timer16_inFreq/timer16_outFreq) - 1);
  pulse_value = (uint32_t)(periode_value * duty_cycle / 100);
  
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF14_TIM16;
  GPIO_InitStruct.Pin  = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  adsTimHandle.Instance = TIM16;  
  adsTimHandle.Init.Prescaler         = prescaler_value;
  adsTimHandle.Init.Period            = periode_value;
  adsTimHandle.Init.ClockDivision     = 0;
  adsTimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  adsTimHandle.Init.RepetitionCounter = 0;  
  if(HAL_TIM_PWM_Init(&adsTimHandle))
    return ADS1293_ERROR;  
  
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
  sConfig.Pulse        = pulse_value;
  if(HAL_TIM_PWM_ConfigChannel(&adsTimHandle, &sConfig, TIM_CHANNEL_1))
    return ADS1293_ERROR;
  
  if(HAL_TIM_PWM_Start(&adsTimHandle, TIM_CHANNEL_1))
    return ADS1293_ERROR;
  
  return ADS1293_OK;
}

ADS1293_Status_TypeDef BSP_ADS1293_AFEClkDeInit(void)
{
  if (HAL_TIM_PWM_Stop(&adsTimHandle, TIM_CHANNEL_1))
    return ADS1293_ERROR;
  
  HAL_GPIO_DeInit(GPIOA,GPIO_PIN_6);
  
  if (HAL_TIM_PWM_DeInit(&adsTimHandle))
    return ADS1293_ERROR;
  
  return ADS1293_OK;
}

/**
* @brief  Set the default configuration
* @param  None
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_SetGeneralConfig(ADS1293_ADSSel_TypeDef ads_select) 
{
  ADS1293_Status_TypeDef err;
  ADS1293_Typedef config;
  
  // First of all select the ADS
  BSP_ADS1293_ADSSel(ads_select);
  
  BSP_ADS1293_GetConfig(&config);
  
  BSP_ADS1293_StartConv(STOP_CONVERSATION); // Stop current running conversation
  
  err = BSP_ADS1293_SetOpMode(config.opmode);
  
  err |= BSP_ADS1293_ChInpConfig(config.in_ch1);  
  err |= BSP_ADS1293_ChInpConfig(config.in_ch2);     
  err |= BSP_ADS1293_ChInpConfig(config.in_ch3);
  
  err |= BSP_ADS1293_LODMode(&config.lod);
  err |= BSP_ADS1293_LODEnable(config.LOD_Enable_In); // to be checked
  err |= BSP_ADS1293_LODCurrent(config.detect_current);
  err |= BSP_ADS1293_LODACConfig(config.lod_ac_config);
  
  err |= BSP_ADS1293_CMDConfig(&config.cmd);
  err |= BSP_ADS1293_RLDOUTConfig(config.rldout_input);
  err |= BSP_ADS1293_WILSONRefConfig(&config.wls);
  err |= BSP_ADS1293_InterRefVoltageConfig(config.InterRefVoltage);
  
  err |= BSP_ADS1293_ClockConfig(config.clock); // Configure and Start the system clock
  
  err |= BSP_ADS1293_AFEResFreqConfig(config.afe_ch1);
  err |= BSP_ADS1293_AFEResFreqConfig(config.afe_ch2);
  err |= BSP_ADS1293_AFEResFreqConfig(config.afe_ch3);
  
  err |= BSP_ADS1293_AFEShutDown(config.shdn_ch1);
  err |= BSP_ADS1293_AFEShutDown(config.shdn_ch2);
  err |= BSP_ADS1293_AFEShutDown(config.shdn_ch3);
  
  err |= BSP_ADS1293_AFEFaultDetect(config.fault_ch1);
  err |= BSP_ADS1293_AFEFaultDetect(config.fault_ch2);
  err |= BSP_ADS1293_AFEFaultDetect(config.fault_ch3);
  
  err |= BSP_ADS1293_R2DecRateConfig(config.r2);
  
  err |= BSP_ADS1293_R1R3DecRateConfig(config.r1r3_ch1);
  err |= BSP_ADS1293_R1R3DecRateConfig(config.r1r3_ch2);
  err |= BSP_ADS1293_R1R3DecRateConfig(config.r1r3_ch3);
  
  err |=  BSP_ADS1293_ECGFilterEnable(config.ecgfilterEn_ch1);
  err |=  BSP_ADS1293_ECGFilterEnable(config.ecgfilterEn_ch2);
  err |=  BSP_ADS1293_ECGFilterEnable(config.ecgfilterEn_ch3);
  
  err |= BSP_ADS1293_DRDYBConfig(config.drdy);  
  err |= BSP_ADS1293_SYNCOUTBConfig(config.sync);   
  err |= BSP_ADS1293_ErrMask(&config.mask_err);
  err |= BSP_ADS1293_AlarmFilter(config.alarm_filter);
  err |= BSP_ADS1293_ChLoopRdBackConfig(&config.loopRdBack);
  
  /* do not start directly conversion since the 3 other ADS should be initialized first */
  //err |= BSP_ADS1293_StartConv(START_CONVERSATION);
  
  
  
#ifdef ADS_ENABLE_MASS_TEST_BENCH_SINUS_INJECT
  REG8 u8Register = 0x07;
  REG8 u8Value = 0x04;
if( ads_select == AFE_CSn_FRONT )
{
      BSP_ADS1293_SPIWrite(u8Register, &u8Value, GPIOC, GPIO_PIN_1, 1);
}
#endif
  
  
  return err;
}

/**
* @brief  Gives the number of data bytes to be read
* @param count: The number of data bytes
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_GetStreamCount(uint8_t *count)
{
  REG8 regVal = 0;
  uint8_t ADS1293_CH_CNFG_Px_EN = ADS1293_CH_CNFG_P1_EN;
  uint8_t ADS1293_CH_CNFG_Ex_EN = ADS1293_CH_CNFG_E1_EN;
  uint8_t cnt = 0;
  
  if(BSP_ADS1293_ReadReg(ADS1293_CH_CNFG_REG, &regVal))
    return ADS1293_ERROR;
  
  if(regVal & ADS1293_CH_CNFG_STS_EN) cnt++;
  
  for(uint8_t i = 0; i < 3; i++)
  {
    if(regVal & (ADS1293_CH_CNFG_Px_EN << i)) cnt += 2;
    if(regVal & (ADS1293_CH_CNFG_Ex_EN << i)) cnt += 3;
  }
  
  *count = cnt;
  
  return ADS1293_OK;
}


/**
* @brief  Deactivate all CS pins
* @retval ADS1293 Status
*/
void BSP_ADS1293_PrintConfRegisters(void)
{
#ifdef __BSW_DEBUG
  
  static REG8 ADS1293_CONFIG[150];
  
  REG8 i = 0;
  
  for( i = 0; i< 0x51 ; i++ )
  {
    BSP_ADS1293_ReadReg( i , &ADS1293_CONFIG[i]);
  }
  
  for( i = 0; i< 0x51 ; i++ )
  {
    __BSW_PRINTF( " -> REG_VALUE %#04X : %#04X \n", i, ADS1293_CONFIG[i] );
    HAL_Delay(20);  
  }
  
#endif
}

/**
* @brief  BSP_ADS1293_IIR_Filter
* @param
- uint32_t IIR_FilterIn
- uint32_t* IIR_FilterOut
* @retval ADS1293 Status
*/
void BSP_ADS1293_IIR_Filter(uint32_t IIR_FilterIn, uint16_t* IIR_FilterOut, uint8_t ChannelIndex)
{
  static uint8_t init[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  static int32_t V0[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  int32_t temp = 0;
  
  // get the first value
  if (init[ChannelIndex] == 0) {
    V0[ChannelIndex] = (int32_t)IIR_FilterIn;
    init[ChannelIndex] = 1;
  } else {
    // calculate the new IIR output 
    V0[ChannelIndex] = (int32_t)((0.999*V0[ChannelIndex]) + (0.001*((int32_t)IIR_FilterIn))) ;
  }
  
  temp = (int32_t) (((int32_t)IIR_FilterIn - V0[ChannelIndex]) / 16); 
    if ( temp >  32700 ) {
    temp = 32700;
  } else if (temp <  -32700 ) {
    temp = -32700;
  }
  
  *IIR_FilterOut = (uint16_t)(temp + 0x8000);
  
}

void BSP_ADS1293_RSTNPinInit(void)
{
#ifdef USE_SEPARATED_RESET
  GPIO_InitTypeDef GPIO_InitStruct;
   
  HAL_GPIO_DeInit(CONF_GLOBAL_RESET_PORT,CONF_GLOBAL_RESET_PIN);
  
  CONF_GLOBAL_RESET_CLK_ENABLE(); 
  
  /* Configure ADS1293-RESET GPIO pin*/
  GPIO_InitStruct.Pin = CONF_GLOBAL_RESET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(CONF_GLOBAL_RESET_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(CONF_GLOBAL_RESET_PORT,CONF_GLOBAL_RESET_PIN,GPIO_PIN_SET);
#endif
}

void BSP_ADS1293_HardReset(void)
{
#ifdef USE_SEPARATED_RESET
  HAL_GPIO_WritePin(CONF_GLOBAL_RESET_PORT,CONF_GLOBAL_RESET_PIN,GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(CONF_GLOBAL_RESET_PORT,CONF_GLOBAL_RESET_PIN,GPIO_PIN_SET);
  HAL_Delay(100);
#endif
}

  

