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





/* Impedance */
#ifdef USE_IMPEDANCE
static uint8_t vADS1_ERROR_LOD = 0x00, vADS2_ERROR_LOD = 0x00, vADS3_ERROR_LOD = 0x00, vADS4_ERROR_LOD = 0x00;
static uint8_t vADSx_LEVEL_VOLTAGE_LOD[12]  = {0 , 0 , 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t vADSx_conv_complete = 0;
static uint8_t vADSx_StartImpedance = 0;
#endif

#ifdef USE_IMPEDANCE

/**
* @brief  BSP_ADS1293_Impedance
* @param  
- uint8_t *COMMAND
- uint8_t *ADS1_ERROR_LOD
- uint8_t *ADS2_ERROR_LOD
- uint8_t *ADS3_ERROR_LOD
- uint8_t *ADS4_ERROR_LOD
- uint8_t *ADSx_LEVEL_VOLTAGE_LOD
- uint8_t *ADSx_conv_complete
* @retval None
*/
void BSP_ADS1293_Impedance(
                           uint8_t *COMMAND,
                           uint8_t *ADS1_ERROR_LOD, 
                           uint8_t *ADS2_ERROR_LOD, 
                           uint8_t *ADS3_ERROR_LOD, 
                           uint8_t *ADS4_ERROR_LOD, 
                           uint8_t *ADSx_LEVEL_VOLTAGE_LOD,
                           uint8_t *ADSx_conv_complete)
{
  
  static uint8_t LOD_CN_ACLVL_LOD_Lx = 0x00;
  static ADS1293_eLeadOffDetect_t LODCurrentState = LOD_DEINIT;
  static ADS1293_eADSSel_t ADSxCurrentState = ADS1;
  uint8_t i = 0x00; 
  uint8_t ADS1293_REG_val = 0x00, ADS1293_ERROR_LOD_REG_reset_val = 0x00, ADS1293_ERROR_LOD_REG_val = 0x00;
  static uint8_t ADS1293_LOD_time = 0, LOD_INPUTSEL_val = 0x00;
  // static uint8_t ADS1_LEVEL_LOD = 0x00, ADS2_LEVEL_LOD = 0x00, ADS3_LEVEL_LOD = 0x00, ADS4_LEVEL_LOD = 0x00;
  static uint8_t ADSx_lead_counter = 0;
  
  if (*COMMAND == 1) 
  {
    LODCurrentState = LOD_INIT;
    *COMMAND = 0;
  }
  
  switch (LODCurrentState)
  {
    
  case LOD_INIT:
    
    for (i=0 ; i<4 ; i++) {
      // 1 - Select the ADSx
      if (i==0) {
        // ADS1  
        BSP_ADS1293_ADSSel(AFE_CSn_FRONT);
      } else if (i==1) {
        // ADS2  
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD1);
      } else if (i==2) {
        // ADS3  
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD2);
      } else if (i==3) {
        // ADS4  
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD3);
      }
      // 2- Clear reg values
      ADS1293_REG_val = 0x0; 
      BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
      // 3 - Update the regs values
      // -> ADS1293_LOD_CN_REG = 0x14
      ADS1293_REG_val = 0x14; 
      BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &ADS1293_REG_val); 
      // -> ADS1293_LOD_CURRENT_REG = 0x19
      ADS1293_REG_val = 0x19; 
      BSP_ADS1293_WriteReg(ADS1293_
                           LOD_CURRENT_REG, &ADS1293_REG_val); 
      // -> ADS1293_LOD_AC_CN_REG = 0xce
      ADS1293_REG_val = 0x0B; 
      BSP_ADS1293_WriteReg(ADS1293_LOD_AC_CN_REG, &ADS1293_REG_val);
    }
    // init internal var
    ADSxCurrentState = ADS1;
    LOD_INPUTSEL_val = 0x02;
    LOD_CN_ACLVL_LOD_Lx = 0x14;
    // go to the next state
    LODCurrentState = LOD_INPUTSEL;
    // ADSx_lead_counter 
    ADSx_lead_counter = 0;
    *ADSx_conv_complete = 0;
    
    break;
    
  case LOD_INPUTSEL: 
    
    switch (ADSxCurrentState)
    {
    case ADS1:
      // ADS1 = 
      BSP_ADS1293_ADSSel(AFE_CSn_FRONT);
      // 12, //LL == 0x02
      // 13, //LA == 0x04
      // 14, //RA == 0x08
      BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &LOD_INPUTSEL_val); 
      
      if (LOD_INPUTSEL_val > 0x08) {
        LOD_INPUTSEL_val = 0x01;
        ADSxCurrentState = ADS2;
        LODCurrentState = LOD_INPUTSEL;
      } else {
        // go to the next state
        LODCurrentState = LOD_CHANGELEVEL;
        // shift to get thenext value
        LOD_INPUTSEL_val = LOD_INPUTSEL_val << 1;
      }
      break;
      
    case ADS2: 
      // ADS2 = 
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD1);
      // 21, //V4 == 0x01
      // 22, //V7 == 0x02
      // 23, //V6 == 0x04
      BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &LOD_INPUTSEL_val);
      
      if (LOD_INPUTSEL_val > 0x04) {
        LOD_INPUTSEL_val = 0x01;
        ADSxCurrentState = ADS3;
        LODCurrentState = LOD_INPUTSEL;
      } else {
        // go to the next state
        LODCurrentState = LOD_CHANGELEVEL;
        // shift to get thenext value
        LOD_INPUTSEL_val = LOD_INPUTSEL_val << 1;
      }
      break;
      
    case ADS3: 
      // ADS3 = 
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD2);
      // 31, //V3 == 0x01
      // 32, //V1 == 0x02
      // 33, //V5 == 0x04
      BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &LOD_INPUTSEL_val);
      
      if (LOD_INPUTSEL_val > 0x04) {
        LOD_INPUTSEL_val = 0x01;
        ADSxCurrentState = ADS4;
        LODCurrentState = LOD_INPUTSEL;
      } else {
        // go to the next state
        LODCurrentState = LOD_CHANGELEVEL;
        // shift to get thenext value
        LOD_INPUTSEL_val = LOD_INPUTSEL_val << 1;
      }
      break;
      
    case ADS4:
      // ADS4 = 
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD3);
      // 41, //V2 == 0x01
      // 42, //V8 == 0x02
      // 43, //V9 == 0x04
      BSP_ADS1293_WriteReg(ADS1293_LOD_EN_REG, &LOD_INPUTSEL_val);
      // go to the next state
      LODCurrentState = LOD_CHANGELEVEL;
      // shift to get thenext value
      LOD_INPUTSEL_val = LOD_INPUTSEL_val << 1;
      //        }
      break;
      
    default:
      
      ADSxCurrentState = ADS1;
      break;
    }
    break;
    
  case LOD_CHANGELEVEL: 
    
    BSP_ADS1293_WriteReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_reset_val);
    
    switch (ADSxCurrentState)
    {
    case ADS1:
      BSP_ADS1293_ADSSel(AFE_CSn_FRONT);
      BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &LOD_CN_ACLVL_LOD_Lx);
      break;
      
    case ADS2: 
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD1);
      BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &LOD_CN_ACLVL_LOD_Lx);
      break;
      
    case ADS3: 
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD2);
      BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &LOD_CN_ACLVL_LOD_Lx);
      break;
      
    case ADS4:
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD3);
      BSP_ADS1293_WriteReg(ADS1293_LOD_CN_REG, &LOD_CN_ACLVL_LOD_Lx);
      break;
      
    default:
      
      ADSxCurrentState = ADS1;
      break;
    }
    
    LOD_CN_ACLVL_LOD_Lx++;
    // go to the next state
    LODCurrentState = LOD_WAITMS;
    break;
    
  case LOD_WAITMS: 
    
    // wait for x ms
    ADS1293_LOD_time++; 
    if (ADS1293_LOD_time > msCounter) {
      ADS1293_LOD_time = 0;
      // go to the next state
      LODCurrentState = LOD_READERRORLOD;
    }
    break;
    
  case LOD_READERRORLOD: 
    
    switch (ADSxCurrentState)
    {
      
    case ADS1:
      
      BSP_ADS1293_ADSSel(AFE_CSn_FRONT);
      BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
      BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
      break;
      
    case ADS2:
      
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD1);
      BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
      BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
      break;
      
    case ADS3:
      
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD2);
      BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val);
      BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
      break; 
      
    case ADS4:
      
      BSP_ADS1293_ADSSel(AFE_CSn_PRECORD3);
      BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val);
      BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
      break;
      
    default:
      ADSxCurrentState = ADS1;
      break;
    }
    
    // 1- check the read range
    if ( (ADS1293_ERROR_LOD_REG_val != 0) && (LOD_CN_ACLVL_LOD_Lx < 0x18) ) { 
      LODCurrentState = LOD_CHANGELEVEL;   
      
      // Store level 0x14 - 0x17
      ADSx_LEVEL_VOLTAGE_LOD[ADSx_lead_counter] = LOD_CN_ACLVL_LOD_Lx-1;
    } else {
      
      switch (ADSxCurrentState)
      {
        
      case ADS1:
        
        BSP_ADS1293_ADSSel(AFE_CSn_FRONT);
        BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
        BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
        *ADS1_ERROR_LOD = *ADS1_ERROR_LOD | (ADS1293_ERROR_LOD_REG_val & ADS1293_REG_val);
        // ADS1_LEVEL_LOD = LOD_CN_ACLVL_LOD_Lx-1;
        // __BSW_PRINTF("> ADS1_ERROR_LOD = %x \n", *ADS1_ERROR_LOD);
        break;
        
      case ADS2:
        
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD1);
        BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val);
        BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
        *ADS2_ERROR_LOD = *ADS2_ERROR_LOD | (ADS1293_ERROR_LOD_REG_val & ADS1293_REG_val);
        // ADS2_LEVEL_LOD = LOD_CN_ACLVL_LOD_Lx-1;
        // __BSW_PRINTF("> ADS2_ERROR_LOD = %x \n", *ADS1_ERROR_LOD);
        break;
        
      case ADS3:
        
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD2);
        BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
        BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val); 
        *ADS3_ERROR_LOD = *ADS3_ERROR_LOD | (ADS1293_ERROR_LOD_REG_val & ADS1293_REG_val);
        // ADS3_LEVEL_LOD = LOD_CN_ACLVL_LOD_Lx-1;
        // __BSW_PRINTF("> ADS3_ERROR_LOD = %x \n", LOD_CN_ACLVL_LOD_Lx);
        break;
        
      case ADS4:
        
        BSP_ADS1293_ADSSel(AFE_CSn_PRECORD3);
        BSP_ADS1293_ReadReg(ADS1293_LOD_EN_REG, &ADS1293_REG_val); 
        BSP_ADS1293_ReadReg(ADS1293_ERROR_LOD_REG, &ADS1293_ERROR_LOD_REG_val);
        *ADS4_ERROR_LOD = *ADS4_ERROR_LOD | (ADS1293_ERROR_LOD_REG_val & ADS1293_REG_val);
        // ADS4_LEVEL_LOD = LOD_CN_ACLVL_LOD_Lx-1;
        // __BSW_PRINTF("> ADS4_ERROR_LOD = %x \n", *ADS1_ERROR_LOD);
        break;
        
      default:
        ADSxCurrentState = ADS1;
        break;
      }
      
      if ( (ADSxCurrentState == ADS4) && (LOD_INPUTSEL_val > 0x04) ) { 
        if (ADS1293_ERROR_LOD_REG_val == 0) {
          ADSx_LEVEL_VOLTAGE_LOD[ADSx_lead_counter] = 0x18 - (LOD_CN_ACLVL_LOD_Lx-1);
        } else {
          ADSx_LEVEL_VOLTAGE_LOD[ADSx_lead_counter] = 0;
        }
        *ADSx_conv_complete = 1;
        //Notify BCP
        BCP_SafeNotifyFromISR( SENSOR_IMPEDANCE_COMPLETE, 0 );
        
        LODCurrentState = LOD_STORERES;
      } else {
        
        if (ADS1293_ERROR_LOD_REG_val == 0) {
          ADSx_LEVEL_VOLTAGE_LOD[ADSx_lead_counter] = 0x18 - (LOD_CN_ACLVL_LOD_Lx-1);
        } else {
          ADSx_LEVEL_VOLTAGE_LOD[ADSx_lead_counter] = 0;
        }
        ADSx_lead_counter++;
        // go to the next input
        LOD_CN_ACLVL_LOD_Lx = 0x14;
        LODCurrentState = LOD_INPUTSEL;
      }
    }
    
    break;
    
  case LOD_STORERES:
    
    // __BSW_PRINTF("> LOD_STORERES \n");
    
    
    LODCurrentState = LOD_DEINIT;
    break;
    
  case LOD_DEINIT:
    
    // __BSW_PRINTF("> LOD_DEINIT \n");
    // Deinit signal injection
    
    break;
    
  default:
    LODCurrentState = LOD_DEINIT;
    break;
  }
  
}


/**
* @brief Get buffer sample by COPY
* @param uint8_t* ADSxLEVELLOD
- uint16_t* ADSxConvComplete
* @retval 
*/
uint16_t BSP_ADS1293_GetZImpedance(uint8_t* u8pData)
{
  uint16_t u16Size;
  u16Size = sizeof(vADSx_LEVEL_VOLTAGE_LOD);
  
  for(uint8_t i=0; i< ADS_CHANNEL_COUNT; i++)
  {     
      u8pData[i] = vADSx_LEVEL_VOLTAGE_LOD[(ADS_ChannelMapping[i])];
  }
  
  return u16Size;
}

/**
* @brief Start conversion
* @param 
* @retval 
*/
void BSP_ADS1293_StartZImpedance(void)
{
  vADSx_StartImpedance = 1; 
}

#endif






/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/
