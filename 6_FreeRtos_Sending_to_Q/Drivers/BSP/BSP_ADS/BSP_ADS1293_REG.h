/**
  ******************************************************************************
  * $HeadURL:$
  * $Author:$
  * $Revision:$
  * $Date:$
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_ADS1293_REG_H
#define __BSP_ADS1293_REG_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
  /* Private typedef -----------------------------------------------------------*/
  /* Private define ------------------------------------------------------------*/

/************************************************************
* ADS1293 REGISTER SET ADDRESSES
************************************************************/

#define ADS1293_CONFIG_REG                              (0x00)                   /* Main Configuration */

#define ADS1293_FLEX_CH1_CN_REG                         (0x01)                   /* Flex Routing Swich Control for Channel 1 */
#define ADS1293_FLEX_CH2_CN_REG                         (0x02)                   /* Flex Routing Swich Control for Channel 2 */
#define ADS1293_FLEX_CH3_CN_REG                         (0x03)                   /* Flex Routing Swich Control for Channel 3 */
#define ADS1293_FLEX_PACE_CN_REG                        (0x04)                   /* Flex Routing Swich Control for Pace Channel */
#define ADS1293_FLEX_VBAT_CN_REG                        (0x05)                   /* Flex Routing Swich Control for Battery Monitoriing */

#define ADS1293_LOD_CN_REG                              (0x06)                   /* Lead Off Detect Control */
#define ADS1293_LOD_EN_REG                              (0x07)                   /* Lead Off Detect Enable */
#define ADS1293_LOD_CURRENT_REG                         (0x08)                   /* Lead Off Detect Current */
#define ADS1293_LOD_AC_CN_REG                           (0x09)                   /* AC Lead Off Detect Current */

#define ADS1293_CMDET_EN_REG                            (0x0A)                   /* Common Mode Detect Enable */
#define ADS1293_CMDET_CN_REG                            (0x0B)                   /* Commond Mode Detect Control */
#define ADS1293_RLD_CN_REG                              (0x0C)                   /* Right Leg Drive Control */

#define ADS1293_WILSON_EN1_REG                          (0x0D)                   /* Wilson Reference Input one Selection */
#define ADS1293_WILSON_EN2_REG                          (0x0E)                   /* Wilson Reference Input two Selection */
#define ADS1293_WILSON_EN3_REG                          (0x0F)                   /* Wilson Reference Input three Selection */
#define ADS1293_WILSON_CN_REG                           (0x10)                   /* Wilson Reference Input Control */

#define ADS1293_REF_CN_REG                              (0x11)                   /* Internal Reference Voltage Control */

#define ADS1293_OSC_CN_REG                              (0x12)                   /* Clock Source and Output Clock Control */

#define ADS1293_AFE_RES_REG                             (0x13)                   /* Analog Front-End Frequency and Resolution */
#define ADS1293_AFE_SHDN_CN_REG                         (0x14)                   /* Analog Front-End Shutdown Control */
#define ADS1293_AFE_FAULT_CN_REG                        (0x15)                   /* Analog Front-End Fault Detection Control */
#define ADS1293_AFE_DITHER_EN_REG                       (0x16)                   /* Enable Dithering in Signma-Delta */
#define ADS1293_AFE_PACE_CN_REG                         (0x17)                   /* Analog Pace Channel Output Routing Control */

#define ADS1293_ERROR_LOD_REG                           (0x18)                   /* Lead Off Detect Error Status */
#define ADS1293_ERROR_STATUS_REG                        (0x19)                   /* Other Error Status */
#define ADS1293_ERROR_RANGE1_REG                        (0x1A)                   /* Channel 1 Amplifier Out of Range Status */
#define ADS1293_ERROR_RANGE2_REG                        (0x1B)                   /* Channel 1 Amplifier Out of Range Status */
#define ADS1293_ERROR_RANGE3_REG                        (0x1C)                   /* Channel 1 Amplifier Out of Range Status */
#define ADS1293_ERROR_SYNC_REG                          (0x1D)                   /* Synchronization Error */

#define ADS1293_R2_RATE_REG                             (0x21)                   /* R2 Decimation Rate */
#define ADS1293_R3_RATE1_REG                            (0x22)                   /* R3 Decimation Rate for Channel 1 */
#define ADS1293_R3_RATE2_REG                            (0x23)                   /* R3 Decimation Rate for Channel 2 */
#define ADS1293_R3_RATE3_REG                            (0x24)                   /* R3 Decimation Rate for Channel 3 */
#define ADS1293_P_DRATE_REG                             (0x25)                   /* 2x Pace Data Rate */
#define ADS1293_DIS_EFILTER_REG                         (0x26)                   /* ECG Filter Disable */
#define ADS1293_DRDYB_SRC_REG                           (0x27)                   /* Data Ready Pin Source */
#define ADS1293_SYNCOUTB_SRC_REG                        (0x28)                   /* Sync Out Pin Source */
#define ADS1293_MASK_DRDYB_REG                          (0x29)                   /* Optional Mask Control for DRDYB Output */
#define ADS1293_MASK_ERR_REG                            (0x2A)                   /* Mask Error on ALARMB Pin */

#define ADS1293_ALARM_FILTER_REG                        (0x2E)                   /* Digital Filter for Analog Alarm Signals */
#define ADS1293_CH_CNFG_REG                             (0x2F)                   /* Configure Channel for Loop Read Back Mode */

#define ADS1293_DATA_STATUS_REG                         (0x30)                   /* ECG and Pace Data Ready Status */
#define ADS1293_DATA_CH1_PACE_H_REG                     (0x31)                   /* Channel1 Pace Data High [15:8] */
#define ADS1293_DATA_CH1_PACE_L_REG                     (0x32)                   /* Channel1 Pace Data Low [7:0] */
#define ADS1293_DATA_CH2_PACE_H_REG                     (0x33)                   /* Channel2 Pace Data High [15:8] */
#define ADS1293_DATA_CH2_PACE_L_REG                     (0x34)                   /* Channel2 Pace Data Low [7:0] */
#define ADS1293_DATA_CH3_PACE_H_REG                     (0x35)                   /* Channel3 Pace Data High [15:8] */
#define ADS1293_DATA_CH3_PACE_L_REG                     (0x36)                   /* Channel3 Pace Data Low [7:0] */
#define ADS1293_DATA_CH1_ECG_H_REG                      (0x37)                   /* Channel1 ECG Data High [23:16] */
#define ADS1293_DATA_CH1_ECG_M_REG                      (0x38)                   /* Channel1 ECG Data Medium [15:8] */
#define ADS1293_DATA_CH1_ECG_L_REG                      (0x39)                   /* Channel1 ECG Data Low [7:0] */
#define ADS1293_DATA_CH2_ECG_H_REG                      (0x3A)                   /* Channel2 ECG Data High [23:16] */
#define ADS1293_DATA_CH2_ECG_M_REG                      (0x3B)                   /* Channel2 ECG Data Medium [15:8] */
#define ADS1293_DATA_CH2_ECG_L_REG                      (0x3C)                   /* Channel2 ECG Data Low [7:0] */
#define ADS1293_DATA_CH3_ECG_H_REG                      (0x3D)                   /* Channel3 ECG Data High [23:16] */
#define ADS1293_DATA_CH3_ECG_M_REG                      (0x3E)                   /* Channel3 ECG Data Medium [15:8] */
#define ADS1293_DATA_CH3_ECG_L_REG                      (0x3F)                   /* Channel3 ECG Data Low [7:0] */

#define ADS1293_REVID_REG                               (0x40)                   /* Revision ID */
#define ADS1293_DATA_LOOP_REG                           (0x50)                   /* Loop Read Back Address */


/************************************************************
* ADS1293 REGISTER BITS DEFINITION
************************************************************/

/*******************  Bits definition for ADS1293_CONFIG register  *****************/
#define ADS1293_CONFIG_START_CON                        ((uint8_t)0x01U)
#define ADS1293_CONFIG_STANDBY                          ((uint8_t)0x02U)
#define ADS1293_CONFIG_PWR_DOWN                         ((uint8_t)0x04U)           

/*******************  Bits definition for ADS1293_FLEX_CHx_CN register  *****************/
#define ADS1293_FLEX_CHx_CN_NEGx                        ((uint8_t)0x07U)
#define ADS1293_FLEX_CHx_CN_NEGx_DISC                   ((uint8_t)0x00U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN1                    ((uint8_t)0x01U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN2                    ((uint8_t)0x02U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN3                    ((uint8_t)0x03U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN4                    ((uint8_t)0x04U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN5                    ((uint8_t)0x05U)
#define ADS1293_FLEX_CHx_CN_NEGx_IN6                    ((uint8_t)0x06U)
#define ADS1293_FLEX_CHx_CN_POSx                        ((uint8_t)0x38U)
#define ADS1293_FLEX_CHx_CN_POSx_DISC                   ((uint8_t)0x00U)
#define ADS1293_FLEX_CHx_CN_POSx_IN1                    ((uint8_t)0x08U)
#define ADS1293_FLEX_CHx_CN_POSx_IN2                    ((uint8_t)0x10U)
#define ADS1293_FLEX_CHx_CN_POSx_IN3                    ((uint8_t)0x18U)
#define ADS1293_FLEX_CHx_CN_POSx_IN4                    ((uint8_t)0x20U)
#define ADS1293_FLEX_CHx_CN_POSx_IN5                    ((uint8_t)0x28U)
#define ADS1293_FLEX_CHx_CN_POSx_IN6                    ((uint8_t)0x30U)
#define ADS1293_FLEX_CHx_CN_TSTx                        ((uint8_t)0xC0U)
#define ADS1293_FLEX_CHx_CN_TSTx_CHx_DISC               ((uint8_t)0x00U)
#define ADS1293_FLEX_CHx_CN_TSTx_CHx_POS                ((uint8_t)0x40U)
#define ADS1293_FLEX_CHx_CN_TSTx_CHx_NEG                ((uint8_t)0x80U)
#define ADS1293_FLEX_CHx_CN_TSTx_CHx_ZERO               ((uint8_t)0xC0U)

/*******************  Bits definition for ADS1293_FLEX_VBAT_CN register  *****************/
#define ADS1293_FLEX_VBAT_CN_VBAT_MONI_CH1              ((uint8_t)0x01U)
#define ADS1293_FLEX_VBAT_CN_VBAT_MONI_CH2              ((uint8_t)0x02U)
#define ADS1293_FLEX_VBAT_CN_VBAT_MONI_CH3              ((uint8_t)0x04U)

/*******************  Bits definition for LOD_CN register  *****************/  
#define ADS1293_LOD_CN_ACLVL_LOD                        ((uint8_t)0x03U)
#define ADS1293_LOD_CN_ACLVL_LOD_L1                     ((uint8_t)0x00U)
#define ADS1293_LOD_CN_ACLVL_LOD_L2                     ((uint8_t)0x01U)
#define ADS1293_LOD_CN_ACLVL_LOD_L3                     ((uint8_t)0x02U)
#define ADS1293_LOD_CN_ACLVL_LOD_L4                     ((uint8_t)0x03U)
#define ADS1293_LOD_CN_SELAC_LOD                        ((uint8_t)0x04U)
#define ADS1293_LOD_CN_SHDN_LOD                         ((uint8_t)0x08U)
#define ADS1293_LOD_CN_ACAD_LOD                         ((uint8_t)0x10U)
  
/*******************  Bits definition for LOD_EN register  *****************/
#define ADS1293_LOD_EN_EN_LOD_1                         ((uint8_t)0x01U)
#define ADS1293_LOD_EN_EN_LOD_2                         ((uint8_t)0x02U)
#define ADS1293_LOD_EN_EN_LOD_3                         ((uint8_t)0x04U)
#define ADS1293_LOD_EN_EN_LOD_4                         ((uint8_t)0x08U)
#define ADS1293_LOD_EN_EN_LOD_5                         ((uint8_t)0x10U)
#define ADS1293_LOD_EN_EN_LOD_6                         ((uint8_t)0x20U)

/*******************  Bits definition for LOD_CURRENT register  *****************/
#define ADS1293_LOD_CURRENT_CUR_LOD                     ((uint8_t)0xFFU)

/*******************  Bits definition for LOD_AC_CN register  *****************/
#define ADS1293_LOD_AC_CN_ACDIV_LOD                     ((uint8_t)0x7FU)
#define ADS1293_LOD_AC_CN_ACDIV_FACTOR                  ((uint8_t)0x80U)
  
/*******************  Bits definition for CMDET_EN register  *****************/
#define ADS1293_CMDET_EN_CMDET_EN_INx                   ((uint8_t)0x3FU)
#define ADS1293_CMDET_EN_CMDET_EN_IN1                   ((uint8_t)0x01U)
#define ADS1293_CMDET_EN_CMDET_EN_IN2                   ((uint8_t)0x02U)
#define ADS1293_CMDET_EN_CMDET_EN_IN3                   ((uint8_t)0x04U)
#define ADS1293_CMDET_EN_CMDET_EN_IN4                   ((uint8_t)0x08U)
#define ADS1293_CMDET_EN_CMDET_EN_IN5                   ((uint8_t)0x10U)
#define ADS1293_CMDET_EN_CMDET_EN_IN6                   ((uint8_t)0x20U)

/*******************  Bits definition for CMDET_CN register  *****************/
#define ADS1293_CMDET_CN_CMDET_CAPDRIVE                 ((uint8_t)0x03U)
#define ADS1293_CMDET_CN_CMDET_CAPDRIVE_LOW_CAP         ((uint8_t)0x00U)        
#define ADS1293_CMDET_CN_CMDET_CAPDRIVE_MEDIUM_LOW_CAP  ((uint8_t)0x01U)
#define ADS1293_CMDET_CN_CMDET_CAPDRIVE_MEDIUM_HIGH_CAP ((uint8_t)0x02U)
#define ADS1293_CMDET_CN_CMDET_CAPDRIVE_HIGH_CAP        ((uint8_t)0x03U)
#define ADS1293_CMDET_CN_CMDET_BW                       ((uint8_t)0x04U)

/*******************  Bits definition for RLD_CN register  *****************/
#define ADS1293_RLD_CN_SELRLD                           ((uint8_t)0x07U)
#define ADS1293_RLD_CN_SELRLD_DISC                      ((uint8_t)0x00U)
#define ADS1293_RLD_CN_SELRLD_IN1                       ((uint8_t)0x01U)
#define ADS1293_RLD_CN_SELRLD_IN2                       ((uint8_t)0x02U)
#define ADS1293_RLD_CN_SELRLD_IN3                       ((uint8_t)0x03U)
#define ADS1293_RLD_CN_SELRLD_IN4                       ((uint8_t)0x04U)
#define ADS1293_RLD_CN_SELRLD_IN5                       ((uint8_t)0x05U)
#define ADS1293_RLD_CN_SELRLD_IN6                       ((uint8_t)0x06U)

/** Added for test bench **/
#define ADS1293_RLD_CN_SHDN_RLD                         ((uint8_t)0x08U)  
  
/*******************  Bits definition for WILSON_ENx register  *****************/
#define ADS1293_WILSON_ENx_SELWILSONx                   ((uint8_t)0x07U)
#define ADS1293_WILSON_ENx_SELWILSONx_DISC              ((uint8_t)0x00U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN1               ((uint8_t)0x01U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN2               ((uint8_t)0x02U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN3               ((uint8_t)0x03U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN4               ((uint8_t)0x04U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN5               ((uint8_t)0x05U)
#define ADS1293_WILSON_ENx_SELWILSONx_IN6               ((uint8_t)0x06U)

/*******************  Bits definition for WILSON_CN register  *****************/  
#define ADS1293_WILSON_CN_WILSONINT                     ((uint8_t)0x01U)
#define ADS1293_WILSON_CN_GOLDINT                       ((uint8_t)0x02U)  
  
/*******************  Bits definition for REF_CN register  *****************/
#define ADS1293_REF_CN_SHDN_REF                         ((uint8_t)0x01U)
#define ADS1293_REF_CN_SHDN_CMREF                       ((uint8_t)0x02U)  

/*******************  Bits definition for OSC_CN register  *****************/
#define ADS1293_OSC_CN_EN_CLKOUT                        ((uint8_t)0x01U)
#define ADS1293_OSC_CN_SHDN_OSC                         ((uint8_t)0x02U)
#define ADS1293_OSC_CN_STRTCLK                          ((uint8_t)0x04U)
  
/*******************  Bits definition for AFE_RES register  *****************/
#define ADS1293_AFE_RES_EN_HIRES_CH1                    ((uint8_t)0x01U)
#define ADS1293_AFE_RES_EN_HIRES_CH2                    ((uint8_t)0x02U)
#define ADS1293_AFE_RES_EN_HIRES_CH3                    ((uint8_t)0x04U)
#define ADS1293_AFE_RES_FS_HIGH_CH1                     ((uint8_t)0x08U)
#define ADS1293_AFE_RES_FS_HIGH_CH2                     ((uint8_t)0x10U)
#define ADS1293_AFE_RES_FS_HIGH_CH3                     ((uint8_t)0x20U)

/*******************  Bits definition for AFE_SHDN_CN register  *****************/
#define ADS1293_AFE_SHDN_CN_SHDN_INA_CH1                ((uint8_t)0x01U)
#define ADS1293_AFE_SHDN_CN_SHDN_INA_CH2                ((uint8_t)0x02U)
#define ADS1293_AFE_SHDN_CN_SHDN_INA_CH3                ((uint8_t)0x04U)
#define ADS1293_AFE_SHDN_CN_SHDN_SDM_CH1                ((uint8_t)0x08U)
#define ADS1293_AFE_SHDN_CN_SHDN_SDM_CH2                ((uint8_t)0x10U)  
#define ADS1293_AFE_SHDN_CN_SHDN_SDM_CH3                ((uint8_t)0x20U)

/*******************  Bits definition for AFE_FAULT_CN register  *****************/  
#define ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH1          ((uint8_t)0x01U) 
#define ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH2          ((uint8_t)0x02U)
#define ADS1293_AFE_FAULT_CN_SHDN_FAULTDET_CH3          ((uint8_t)0x04U)
  
/*******************  Bits definition for AFE_PACE_CN register  *****************/ 
#define ADS1293_AFE_PACE_CN_SHDN_PACE                   ((uint8_t)0x01U)
#define ADS1293_AFE_PACE_CN_PACE2WCT                    ((uint8_t)0x02U)
#define ADS1293_AFE_PACE_CN_PACE2RLDIN                  ((uint8_t)0x04U)

/*******************  Bits definition for ERROR_LOD register  *****************/   
#define ADS1293_ERROR_LOD_OUT_LOD                       ((uint8_t)0x3FU)

/*******************  Bits definition for ERROR_STATUS register  *****************/     
#define ADS1293_ERROR_STATUS_CMOR                       ((uint8_t)0x01U)
#define ADS1293_ERROR_STATUS_RLDRAIL                    ((uint8_t)0x02U)  
#define ADS1293_ERROR_STATUS_BATLOW                     ((uint8_t)0x04U)  
#define ADS1293_ERROR_STATUS_LEADOFF                    ((uint8_t)0x08U)
#define ADS1293_ERROR_STATUS_CH1ERR                     ((uint8_t)0x10U)
#define ADS1293_ERROR_STATUS_CH2ERR                     ((uint8_t)0x20U)  
#define ADS1293_ERROR_STATUS_CH3ERR                     ((uint8_t)0x40U)
#define ADS1293_ERROR_STATUS_SYNCEDGEERR                ((uint8_t)0x80U)
  
/*******************  Bits definition for ERROR_RANGEx register  *****************/   
#define ADS1293_ERROR_RANGEx_DIF_HIGH_CHx               ((uint8_t)0x01U)
#define ADS1293_ERROR_RANGEx_OUTP_HIGH_CHx              ((uint8_t)0x02U)
#define ADS1293_ERROR_RANGEx_OUTP_LOW_CHx               ((uint8_t)0x04U)
#define ADS1293_ERROR_RANGEx_OUTN_HIGH_CHx              ((uint8_t)0x08U)
#define ADS1293_ERROR_RANGEx_OUTN_LOW_CHx               ((uint8_t)0x10U)
#define ADS1293_ERROR_RANGEx_SIGN_CHx                   ((uint8_t)0x20U)  
#define ADS1293_ERROR_RANGEx_SDM_OR_CHx                 ((uint8_t)0x40U)    
  
/*******************  Bits definition for ERROR_SYNC register  *****************/     
#define ADS1293_ERROR_SYNC_SYNC_CH1ERR                  ((uint8_t)0x01U)  
#define ADS1293_ERROR_SYNC_SYNC_CH2ERR                  ((uint8_t)0x02U)   
#define ADS1293_ERROR_SYNC_SYNC_CH3ERR                  ((uint8_t)0x04U)   
#define ADS1293_ERROR_SYNC_SYNC_PHASEERR                ((uint8_t)0x08U)   
  
/*******************  Bits definition for ERROR_MISC register  *****************/   
#define ADS1293_ERROR_MISC_CMOR_STATUS                  ((uint8_t)0x01U) 
#define ADS1293_ERROR_MISC_RLDRAIL_STATUS               ((uint8_t)0x02U)  
#define ADS1293_ERROR_MISC_BATLOW_STATUS                ((uint8_t)0x04U)  

/*******************  Bits definition for DIGO_STRENGTH register  *****************/  
#define ADS1293_DIGO_STRENGTH_DIGO_STRENGTH             ((uint8_t)0x03U)
#define ADS1293_DIGO_STRENGTH_DIGO_STRENGTH_LOW         ((uint8_t)0x00U)
#define ADS1293_DIGO_STRENGTH_DIGO_STRENGTH_MID_LOW     ((uint8_t)0x01U)  
#define ADS1293_DIGO_STRENGTH_DIGO_STRENGTH_MID_HIGH    ((uint8_t)0x02U)  
#define ADS1293_DIGO_STRENGTH_DIGO_STRENGTH_HIGH        ((uint8_t)0x03U)

/*******************  Bits definition for R2_RATE register  *****************/  
#define ADS1293_R2_RATE_R2_RATE                         ((uint8_t)0x0FU)
#define ADS1293_R2_RATE_R2_RATE_4                       ((uint8_t)0x01U)
#define ADS1293_R2_RATE_R2_RATE_5                       ((uint8_t)0x02U)  
#define ADS1293_R2_RATE_R2_RATE_6                       ((uint8_t)0x04U)
#define ADS1293_R2_RATE_R2_RATE_8                       ((uint8_t)0x08U)
  
/*******************  Bits definition for R3_RATE_CHx register  *****************/  
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx                 ((uint8_t)0xFFU)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_4               ((uint8_t)0x01U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_6               ((uint8_t)0x02U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_8               ((uint8_t)0x04U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_12              ((uint8_t)0x08U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_16              ((uint8_t)0x10U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_32              ((uint8_t)0x20U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_64              ((uint8_t)0x40U)
#define ADS1293_R3_RATE_CHx_R3_RATE_CHx_128             ((uint8_t)0x80U)
  
/*******************  Bits definition for R1_RATE register  *****************/ 
#define ADS1293_R1_RATE_R1_RATE_CH1                     ((uint8_t)0x01U)
#define ADS1293_R1_RATE_R1_RATE_CH2                     ((uint8_t)0x02U)
#define ADS1293_R1_RATE_R1_RATE_CH3                     ((uint8_t)0x04U)
  
/*******************  Bits definition for DIS_EFILTER register  *****************/   
#define ADS1293_DIS_EFILTER_DIS_E1                     ((uint8_t)0x01U) 
#define ADS1293_DIS_EFILTER_DIS_E2                     ((uint8_t)0x02U)   
#define ADS1293_DIS_EFILTER_DIS_E3                     ((uint8_t)0x04U)   
  
/*******************  Bits definition for DRDYB_SRC register  *****************/     
#define ADS1293_DRDYB_SRC_DRDYB_SRC                    ((uint8_t)0x3FU)   
#define ADS1293_DRDYB_SRC_DRDYB_SRC_NONE               ((uint8_t)0x00U)     
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH1_PACE           ((uint8_t)0x01U)  
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH2_PACE           ((uint8_t)0x02U)  
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH3_PACE           ((uint8_t)0x04U)  
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH1_ECG            ((uint8_t)0x08U)  
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH2_ECG            ((uint8_t)0x10U)  
#define ADS1293_DRDYB_SRC_DRDYB_SRC_CH3_ECG            ((uint8_t)0x20U)    
  
/*******************  Bits definition for SYNCB_CN register  *****************/   
#define ADS1293_SYNCB_CN_SYNCB_SRC                    ((uint8_t)0x3FU)
#define ADS1293_SYNCB_CN_SYNCB_SRC_NONE               ((uint8_t)0x00U)     
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH1_PACE           ((uint8_t)0x01U)  
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH2_PACE           ((uint8_t)0x02U)  
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH3_PACE           ((uint8_t)0x04U)  
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH1_ECG            ((uint8_t)0x08U)  
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH2_ECG            ((uint8_t)0x10U)  
#define ADS1293_SYNCB_CN_SYNCB_SRC_CH3_ECG            ((uint8_t)0x20U)
#define ADS1293_SYNCB_CN_DIS_SYNCBOUT                 ((uint8_t)0x40U)

/*******************  Bits definition for MASK_DRDYB register  *****************/   
#define ADS1293_MASK_DRDYB_DRDYBMASK_CTL0             ((uint8_t)0x01U)  
#define ADS1293_MASK_DRDYB_DRDYBMASK_CTL1             ((uint8_t)0x02U)    
  
/*******************  Bits definition for MASK_ERR register  *****************/     
#define ADS1293_MASK_ERR_MASK_CMOR                     ((uint8_t)0x01U)
#define ADS1293_MASK_ERR_MASK_RLDRAIL                  ((uint8_t)0x02U)  
#define ADS1293_MASK_ERR_MASK_BATLOW                   ((uint8_t)0x04U)
#define ADS1293_MASK_ERR_MASK_LEADOFF                  ((uint8_t)0x08U)  
#define ADS1293_MASK_ERR_MASK_CH1ERR                   ((uint8_t)0x10U)  
#define ADS1293_MASK_ERR_MASK_CH2ERR                   ((uint8_t)0x20U)    
#define ADS1293_MASK_ERR_MASK_CH3ERR                   ((uint8_t)0x40U)       
#define ADS1293_MASK_ERR_MASK_SYNCEDGEERR              ((uint8_t)0x80U)   
  
/*******************  Bits definition for ALARM_FILTER register  *****************/  
#define ADS1293_ALARM_FILTER_AFILTER_LOD               ((uint8_t)0x0FU)  
#define ADS1293_ALARM_FILTER_AFILTER_OTHER             ((uint8_t)0xF0U)  
  
/*******************  Bits definition for CH_CNFG register  *****************/   
#define ADS1293_CH_CNFG_STS_EN                         ((uint8_t)0x01U)  
#define ADS1293_CH_CNFG_P1_EN                          ((uint8_t)0x02U)
#define ADS1293_CH_CNFG_P2_EN                          ((uint8_t)0x04U)
#define ADS1293_CH_CNFG_P3_EN                          ((uint8_t)0x08U)
#define ADS1293_CH_CNFG_E1_EN                          ((uint8_t)0x10U)
#define ADS1293_CH_CNFG_E2_EN                          ((uint8_t)0x20U)
#define ADS1293_CH_CNFG_E3_EN                          ((uint8_t)0x40U)

/*******************  Bits definition for DATA_STATUS register  *****************/ 
#define ADS1293_DATA_STATUS_ALARMB                     ((uint8_t)0x02U) 
#define ADS1293_DATA_STATUS_P1_DRDY                    ((uint8_t)0x04U) 
#define ADS1293_DATA_STATUS_P2_DRDY                    ((uint8_t)0x08U) 
#define ADS1293_DATA_STATUS_P3_DRDY                    ((uint8_t)0x10U)
#define ADS1293_DATA_STATUS_E1_DRDY                    ((uint8_t)0x20U)
#define ADS1293_DATA_STATUS_E2_DRDY                    ((uint8_t)0x40U)
#define ADS1293_DATA_STATUS_E3_DRDY                    ((uint8_t)0x80U)

/*******************  Bits definition for DATA_CHx_PACE register  *****************/ 
#define ADS1293_DATA_CHx_PACE_DATA_CHx_PACE            ((uint8_t)0xFFU) 

/*******************  Bits definition for DATA_CHx_ECG register  *****************/ 
#define ADS1293_DATA_CHx_ECG_DATA_CHx_ECG              ((uint8_t)0xFFU) 

/*******************  Bits definition for REVID register  *****************/ 
#define ADS1293_REVID_REVID                            ((uint8_t)0xFFU) 

/*******************  Bits definition for DATA_LOOP register  *****************/ 
#define ADS1293_DATA_LOOP_PE_LPRD                      ((uint8_t)0xFFU)
  
// Useful definitions
#define ADS1293_RD_BIT                                  (0x80)
#define ADS1293_WR_BIT                                  (0x7F)

/*******************  General iput bits mask definition  *****************/
#define IN1_MASK                                        ((uint8_t)0x01U)
#define IN2_MASK                                        ((uint8_t)0x02U)
#define IN3_MASK                                        ((uint8_t)0x04U)
#define IN4_MASK                                        ((uint8_t)0x08U)
#define IN5_MASK                                        ((uint8_t)0x10U)
#define IN6_MASK                                        ((uint8_t)0x20U)
  

/* Exported function prototypes ----------------------------------------------*/


  
#ifdef __cplusplus
}
#endif

#endif /*__BSP_ADS1293_REG_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
