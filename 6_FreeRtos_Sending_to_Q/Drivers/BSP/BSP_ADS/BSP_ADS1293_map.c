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
#include "BSW_CONFIG.h"
#include "BCP.h"

extern xNVM_ConfigFileStruct_t xConfigFile;

/* Selected ADS */
extern ADS1293_ADSSel_TypeDef selected_ads;

/* Global Channel Mapping */
ADS1293_ChannelMappingType_t ADS_ChannelMappingSet = ADS1293_NO_TS;
uint8_t ADS_ChannelMapping[ADS_CHANNEL_COUNT] = {0 , 1 , 2 , 3, 4, 5, 6, 7, 8, 9, 10, 11 };


/* Channel Mapping */
const uint8_t ADS_SINUS_INJECT_ChannelMapping[ADS_CHANNEL_COUNT] = {0 , 1 , 2 , 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t ADS_TSCLOSE_ChannelMappingV1[ADS_CHANNEL_COUNT] = {0, 1, 2, 7, 9, 6, 3, 8, 5, 4, 10, 11};
const uint8_t ADS_TSCLOSE_ChannelMappingV2[ADS_CHANNEL_COUNT] = {0, 1, 2, 7, 11, 6, 3, 8, 5, 4, 9, 10};
const uint8_t ADS_TSOPEN_ChannelMapping[ADS_CHANNEL_COUNT] = {0 , 1 , 2 , 6, 5, 7, 3, 8, 4, 9, 10, 11};
// SOMNONAUTE
const uint8_t ADS_TS_SOMNONAUTE_ChannelMapping[ADS_CHANNEL_COUNT] = {0, 1, 2, 3, 4, 9, 6, 7, 10, 5, 8, 11};
const uint8_t ADS_CAP_SOMNONAUTE_ChannelMapping[ADS_CHANNEL_COUNT] = {0, 1, 2, 7, 6, 3, 10, 9, 11, 4, 5, 8};
// PREGNAUTE
const uint8_t ADS_PREGNAUTE_ChannelMapping[ADS_CHANNEL_COUNT] = {4, 0, 10, 11, 5, 9, 3, 1, 8, 7, 6, 2}; 



void BSP_ADS1293_SetChannelMapping( ADS1293_SelectConfig ADS_Mode )
{
  uint8_t R3_RATE = 0;
  uint8_t u8EepromType = 0;
  uint8_t u8pSerialNumber[60];
  uint8_t param1 = 0 , param2 = 0 ;
  
  /*Select for Mapping */   
  switch( BCP_GetEepromType() )
  {
    
  case EEPROM_OPEN_TSHIRT :
    ADS_ChannelMappingSet = ADS1293_TS_OPEN;
    memcpy(ADS_ChannelMapping, ADS_TSOPEN_ChannelMapping, sizeof(ADS_ChannelMapping) );
//    __BSW_PRINTF( "[SENS]CONFIG TO OPEN TSHIRT\n" );
    break;
    
  case EEPROM_CLOSE_TSHIRT_V1 :
    ADS_ChannelMappingSet = ADS1293_TS_CLOSE_V1;
    memcpy(ADS_ChannelMapping, ADS_TSCLOSE_ChannelMappingV1, sizeof(ADS_ChannelMapping) );
//    __BSW_PRINTF( "[SENS]CONFIG TO CLOSE TSHIRT V1\n" );
    break;
    
  case EEPROM_CLOSE_TSHIRT_V2 :
    ADS_ChannelMappingSet = ADS1293_TS_CLOSE_V2;
    memcpy(ADS_ChannelMapping, ADS_TSCLOSE_ChannelMappingV2, sizeof(ADS_ChannelMapping) );
//    __BSW_PRINTF( "[SENS]CONFIG TO CLOSE TSHIRT V2\n" );
    break;
    
  case EEPROM_TS_SOMNONAUTE :
    ADS_ChannelMappingSet = ADS1293_TS_SOMNONAUTE;
    memcpy(ADS_ChannelMapping, ADS_TS_SOMNONAUTE_ChannelMapping, sizeof(ADS_ChannelMapping) );
//    __BSW_PRINTF( "[SENS]CONFIG TO SOMNONAUTE\n" );
    break;
    
  case EEPROM_PREGNAUTE :
    ADS_ChannelMappingSet = ADS1293_TS_PREGNAUTE;
    memcpy(ADS_ChannelMapping, ADS_PREGNAUTE_ChannelMapping, sizeof(ADS_ChannelMapping) );
    // FORCE ADS TO 1000Hz
    xConfigFile.eECGFrequency = BCP_ECG_FEQUENCY_1000_HZ;
//    __BSW_PRINTF( "[SENS]CONFIG TO PREGNAUTE\n" );
    break;
    
  case EEPROM_CAP_SOMNONAUTE :
    ADS_ChannelMappingSet = ADS1293_CAP_SOMNONAUTE;
    memcpy(ADS_ChannelMapping, ADS_CAP_SOMNONAUTE_ChannelMapping, sizeof(ADS_ChannelMapping) );
    // FORCE ADS TO 1000Hz
    xConfigFile.eECGFrequency = BCP_ECG_FEQUENCY_1000_HZ;
//    __BSW_PRINTF( "[SENS]CONFIG TO CAP SOMNONAUTE\n" );
    break;
    
  default :
    ADS_ChannelMappingSet = ADS1293_TS_CLOSE_V1;
    memcpy(ADS_ChannelMapping, ADS_TSCLOSE_ChannelMappingV1, sizeof(ADS_ChannelMapping) );
//    __BSW_PRINTF( "[SENS]NO EEPROM -> CONFIG TO CLOSE TSHIRT V1\n" );
    break;   
  }
    
  
  switch( xConfigFile.eECGFrequency )
  {
    case BCP_ECG_FEQUENCY_250_HZ :
      xConfigFile.u32EcgSamplingPeriodMs = 4;
      R3_RATE = R3_RATE_CHx_R3_RATE_CHx_16; 
      break;
      
    case BCP_ECG_FEQUENCY_1000_HZ :
      xConfigFile.u32EcgSamplingPeriodMs = 1;
      R3_RATE = R3_RATE_CHx_R3_RATE_CHx_4; 
      break;
      
    default :
      xConfigFile.eECGFrequency = BCP_ECG_FEQUENCY_250_HZ;
      R3_RATE = R3_RATE_CHx_R3_RATE_CHx_16; 
      xConfigFile.u32EcgSamplingPeriodMs = 4;
      break;
  }
  

  /* ADS Configuration */
  ConfSet(ADS1293, OpMode, STD_MODE);
  vaConfSet(ADS1293, InCh1, CH1, FLEX_CHx_CN_IN4, FLEX_CHx_CN_IN3);
  vaConfSet(ADS1293, InCh2, CH2, FLEX_CHx_CN_IN2, FLEX_CHx_CN_IN3);
  vaConfSet(ADS1293, InCh3, CH3, FLEX_CHx_CN_IN2, FLEX_CHx_CN_IN4);
  
  if( ADS_Mode == ADS1293_SinusInjectConfig )
  {
  vaConfSet(ADS1293, LODMode, LOD_CN_ACLVL_LOD_L1, 1, 0, 1);
  vaConfSet(ADS1293, LOD_Enable, 0, 0, 0, 0, 0, 0);
  ConfSet(ADS1293, LOD_Current, DET_CURR_400nA);
  vaConfSet(ADS1293, LOD_AC_Config, K16, 0x4E);
  }
  else if ( ADS_Mode == ADS1293_TestBenchConfig )
  {
  EEPROM_ExtractData( &u8EepromType, u8pSerialNumber);
  param1 = u8pSerialNumber[44];
  param2 = u8pSerialNumber[45];
  
  vaConfSet(ADS1293, LODMode, LOD_CN_ACLVL_LOD_L1, 1, 0, 1);
  vaConfSet(ADS1293, LOD_Enable, 0, 0, 0, 0, 0, 0);
  
  //ConfSet(ADS1293, LOD_Current, DET_CURR_80nA);
  //vaConfSet(ADS1293, LOD_AC_Config, K16, 0x4E);
  ConfSet(ADS1293, LOD_Current, param2);
  vaConfSet(ADS1293, LOD_AC_Config, param1, 0x01);

  }
  else
  {
  vaConfSet(ADS1293, LODMode, LOD_CN_ACLVL_LOD_L1, 0, 1, 0);
  vaConfSet(ADS1293, LOD_Enable, 0, 0, 0, 0, 0, 0);
  ConfSet(ADS1293, LOD_Current, DET_CURR_80nA);
  vaConfSet(ADS1293, LOD_AC_Config, K1, 0x00);
  }

  vaConfSet(ADS1293, CMDConfig, 0, 0, 0);
  ConfSet(ADS1293, RLDOUTConfig, DISC);
  vaConfSet(ADS1293, WILSONRefConfig, DISC, DISC, DISC, DISC, DISC);
  vaConfSet(ADS1293, InterRefVoltageConfig, 0x00, 0x00);
  vaConfSet(ADS1293, ClockConfig, 0x01, 0x00);
  
  vaConfSet(ADS1293, AFEResFreqConfigCh1, CH1, 0x00, 0x00);
  vaConfSet(ADS1293, AFEResFreqConfigCh2, CH2, 0x00, 0x00);
  vaConfSet(ADS1293, AFEResFreqConfigCh3, CH3, 0x00, 0x00);
  
  vaConfSet(ADS1293, AFEShutDownCh1, CH1, 0x00, 0x00);
  vaConfSet(ADS1293, AFEShutDownCh2, CH2, 0x00, 0x00);
  vaConfSet(ADS1293, AFEShutDownCh3, CH3, 0x00, 0x00);
  vaConfSet(ADS1293, AFEFaultDetectCh1, CH1, 0x00);
  vaConfSet(ADS1293, AFEFaultDetectCh2, CH2, 0x00);
  vaConfSet(ADS1293, AFEFaultDetectCh3, CH3, 0x00);
  ConfSet(ADS1293, R2DecRateConfig, R2_RATE_R2_RATE_6);
  
  vaConfSet(ADS1293, R1R3DecRateConfigCh1, CH1, 0x00, R3_RATE);
  vaConfSet(ADS1293, R1R3DecRateConfigCh2, CH2, 0x00, R3_RATE);
  vaConfSet(ADS1293, R1R3DecRateConfigCh3, CH3, 0x00, R3_RATE);
  
  vaConfSet(ADS1293, ECGFilterEnableCh1, CH1, 0x00);
  vaConfSet(ADS1293, ECGFilterEnableCh2, CH2, 0x00);
  vaConfSet(ADS1293, ECGFilterEnableCh3, CH3, 0x00);
  vaConfSet(ADS1293, DRDYBConfig, DRDYB_SRC_DRDYB_SRC_CH1_ECG, 0x00, 0x00);
  vaConfSet(ADS1293, SYNCOUTBConfig, ADS1293_SYNCB_CN_SYNCB_SRC_CH1_ECG, ENABLE_SYNCBOUT);
  vaConfSet(ADS1293, ErrMask, 0, 0, 0, 0, 0, 0, 0, 0);
  vaConfSet(ADS1293, AlarmFilter, 0x03, 0x03);
  vaConfSet(ADS1293, LoopRdBackConfig, 0, 0, 0, 0, 1, 1, 1);
  
}

/**
* @brief  Get the default configuration
* @param  None
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_GetConfig(ADS1293_Typedef *config)
{
  config->opmode = ConfGet(ADS1293, OpMode);
  
  // AFE_CSn_FRONT = 0x00,
  // AFE_CSn_PRECORD1 = 0x01,
  // AFE_CSn_PRECORD2 = 0x02,
  // AFE_CSn_PRECORD3 = 0x03,
  
  if ( selected_ads ==  AFE_CSn_FRONT ) // Master
  {
    // ADS
    switch( ADS_ChannelMappingSet )
    {
    case ADS1293_TS_OPEN :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN4;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN4;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN2;        
      break;
    case ADS1293_TS_CLOSE_V1 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN4;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN4;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN2;
      break;
    case ADS1293_TS_CLOSE_V2 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN4;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN4;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN2;
      break;
    case ADS1293_TS_PREGNAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN4; 
      break;
    case ADS1293_TS_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN4;
      break;
    case ADS1293_CAP_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN4;
      break;
    default :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN4;   
      break;   
    }
    
    config->sync.disable = 0x0;
    config->sync.src = SYNCB_CN_SYNCB_SRC_CH1_ECG;
    
    config->wls.selWilson1 =  IN2;
    config->wls.selWilson2 =  IN3;
    config->wls.selWilson3 =  IN4;
    config->wls.goldint = 0;
    config->wls.wilsonint= 0;
  } 
  else if ( selected_ads ==  AFE_CSn_PRECORD1 )  
  {
    
    switch( ADS_ChannelMappingSet )
    {
    case ADS1293_TS_OPEN :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V1 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V2 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_PREGNAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN1;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in = FLEX_CHx_CN_IN5;
      break;
    case ADS1293_CAP_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in = FLEX_CHx_CN_IN5;
      break;
    default :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;   
    }

    config->sync.disable = 0x1;
    config->sync.src = SYNCB_CN_SYNCB_SRC_NONE;

    config->wls.selWilson1 =  DISC;
    config->wls.selWilson2 =  DISC;
    config->wls.selWilson3 =  DISC;
    config->wls.goldint = 0;
    config->wls.wilsonint= 0;
  }
  else if ( selected_ads ==  AFE_CSn_PRECORD2 )  
  {
    
    switch( ADS_ChannelMappingSet )
    {
    case ADS1293_TS_OPEN :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V1 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V2 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_PREGNAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN3;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in = FLEX_CHx_CN_IN5;
      break;
    case ADS1293_CAP_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in = FLEX_CHx_CN_IN5;
      break;
    default :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;   
    }
    
    config->sync.disable = 0x1;
    config->sync.src = SYNCB_CN_SYNCB_SRC_NONE;
    
    config->wls.selWilson1 =  DISC;
    config->wls.selWilson2 =  DISC;
    config->wls.selWilson3 =  DISC;
    config->wls.goldint = 0;
    config->wls.wilsonint= 0;
  }
  else if ( selected_ads ==  AFE_CSn_PRECORD3 )  
  {
    
    switch( ADS_ChannelMappingSet )
    {
    case ADS1293_TS_OPEN :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V1 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_CLOSE_V2 :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN6;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    case ADS1293_TS_PREGNAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    default :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;   
    case ADS1293_TS_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN2;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN3;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN5;
      break;
    case ADS1293_CAP_SOMNONAUTE :
      config->in_ch1.channel = CH1;
      config->in_ch1.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch1.pos_in =  FLEX_CHx_CN_IN1;
      config->in_ch2.channel = CH2;
      config->in_ch2.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch2.pos_in =  FLEX_CHx_CN_IN2;
      config->in_ch3.channel = CH3;
      config->in_ch3.neg_in =  FLEX_CHx_CN_IN5;
      config->in_ch3.pos_in =  FLEX_CHx_CN_IN3;
      break;
    }
    
    config->sync.disable = 0x1;
    config->sync.src = SYNCB_CN_SYNCB_SRC_NONE;
    
    config->wls.selWilson1 =  DISC;
    config->wls.selWilson2 =  DISC;
    config->wls.selWilson3 =  DISC;
    config->wls.goldint = 0;
    config->wls.wilsonint= 0;
  }
  
  
  config->lod = ConfGet(ADS1293, LODMode);
  config->LOD_Enable_In = ConfGet(ADS1293, LOD_Enable);
  config->detect_current = ConfGet(ADS1293, LOD_Current);
  config->lod_ac_config = ConfGet(ADS1293, LOD_AC_Config);
  config->cmd = ConfGet(ADS1293, CMDConfig);
  //config->wls = ConfGet(ADS1293, WILSONRefConfig);
  config->rldout_input = ConfGet(ADS1293, RLDOUTConfig);
  config->InterRefVoltage = ConfGet(ADS1293, InterRefVoltageConfig);
  config->clock = ConfGet(ADS1293, ClockConfig);
  config->afe_ch1 = ConfGet(ADS1293, AFEResFreqConfigCh1);
  config->afe_ch2 = ConfGet(ADS1293, AFEResFreqConfigCh2);
  config->afe_ch3 = ConfGet(ADS1293, AFEResFreqConfigCh3);
  config->shdn_ch1 = ConfGet(ADS1293, AFEShutDownCh1);
  config->shdn_ch2 = ConfGet(ADS1293, AFEShutDownCh2);
  config->shdn_ch3 = ConfGet(ADS1293, AFEShutDownCh3);
  config->fault_ch1 = ConfGet(ADS1293, AFEFaultDetectCh1);
  config->fault_ch2 = ConfGet(ADS1293, AFEFaultDetectCh2);
  config->fault_ch3 = ConfGet(ADS1293, AFEFaultDetectCh3);
  config->r2 = ConfGet(ADS1293, R2DecRateConfig);
  config->r1r3_ch1 = ConfGet(ADS1293, R1R3DecRateConfigCh1);
  config->r1r3_ch2 = ConfGet(ADS1293, R1R3DecRateConfigCh2);
  config->r1r3_ch3 = ConfGet(ADS1293, R1R3DecRateConfigCh3);
  config->ecgfilterEn_ch1 = ConfGet(ADS1293, ECGFilterEnableCh1);
  config->ecgfilterEn_ch2 = ConfGet(ADS1293, ECGFilterEnableCh2);
  config->ecgfilterEn_ch3 = ConfGet(ADS1293, ECGFilterEnableCh3);
  config->drdy = ConfGet(ADS1293, DRDYBConfig);
  config->mask_err = ConfGet(ADS1293, ErrMask);
  config->alarm_filter = ConfGet(ADS1293, AlarmFilter);
  config->loopRdBack = ConfGet(ADS1293, LoopRdBackConfig);
  
  return ADS1293_OK;
}
