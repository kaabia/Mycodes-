/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_ACC/BSP_MPU9250.h $
* $OPCMA Tunisie$
* $Revision: 448 $
* $Date: 2016-06-10 15:14:30 +0100 (ven., 10 juin 2016) $
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_MPU9250_H
#define __BSP_MPU9250_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /*******************************Includes ************************************/
#include "DRV_I2C.h" 
  
  /* ***************************Exported constants ****************************/
  typedef enum MPU9250_AddressEnum              BSP_MPU9250_AddressEnum;
  typedef enum MPU9250_AK8963_AddressEnum       BSP_MPU9250_AK8963_AddressEnum;
  typedef enum MPU9250_FIFOModeEnum             BSP_MPU9250_FIFOModeEnum;
  typedef enum MPU9250_ExtSyncSetEnum           BSP_MPU9250_ExtSyncSetEnum;
  typedef enum MPU9250_DLPFCfgEnum              BSP_MPU9250_DLPFCfgEnum;
  typedef enum MPU9250_XGYROCtenEnum            BSP_MPU9250_XGYROCtenEnum;
  typedef enum MPU9250_YGYROCtenEnum            BSP_MPU9250_YGYROCtenEnum;
  typedef enum MPU9250_ZGYROCtenEnum            BSP_MPU9250_ZGYROCtenEnum;
  typedef enum MPU9250_GyroFSEnum               BSP_MPU9250_GyroFSEnum;
  typedef enum MPU9250_FchoiceBEnum             BSP_MPU9250_FchoiceBEnum;
  typedef enum MPU9250_XAccSTEnum               BSP_MPU9250_XAccSTEnum;
  typedef enum MPU9250_YAccSTEnum               BSP_MPU9250_YAccSTEnum;
  typedef enum MPU9250_ZAccSTEnum               BSP_MPU9250_ZAccSTEnum;
  typedef enum MPU9250_AccelFSEnum              BSP_MPU9250_AccelFSEnum;
  typedef enum MPU9250_AccelFchoiceBEnum        BSP_MPU9250_AccelFchoiceBEnum;
  typedef enum MPU9250_ADLPFCfgEnum             BSP_MPU9250_ADLPFCfgEnum;
  typedef enum MPU9250_LposcClkselEnum          BSP_MPU9250_LposcClkselEnum;
  typedef enum MPU9250_FIFOEnableEnum           BSP_MPU9250_FIFOEnableEnum;
  typedef enum MPU9250_I2CMasterCntrlEnum       BSP_MPU9250_I2CMasterCntrlEnum;
  typedef enum MPU9250_I2CMSTCLKEnum            BSP_MPU9250_I2CMSTCLKEnum;
  typedef enum MPU9250_SLVxRNWEnum              BSP_MPU9250_SLVxRNWEnum;
  typedef enum MPU9250_SLVxCtrlEnum             BSP_MPU9250_SLVxCtrlEnum;
  typedef enum MPU9250_SLV4CtrlEnum             BSP_MPU9250_SLV4CtrlEnum;
  typedef enum MPU9250_I2CMSTStatusEnum         BSP_MPU9250_I2CMSTStatusEnum;
  typedef enum MPU9250_INTPinCFGEnum            BSP_MPU9250_INTPinCFGEnum;
  typedef enum MPU9250_INTEnableEnum            BSP_MPU9250_INTEnableEnum;
  typedef enum MPU9250_MSTDelayCtrlEnum         BSP_MPU9250_MSTDelayCtrlEnum;
  typedef enum MPU9250_SignalPathResetEnum      BSP_MPU9250_SignalPathResetEnum;
  typedef enum MPU9250_MOTDetectCtrlEnum        BSP_MPU9250_MOTDetectCtrlEnum;
  typedef enum MPU9250_UserControlEnum          BSP_MPU9250_UserControlEnum;
  typedef enum MPU9250_PWRMGMT1Enum             BSP_MPU9250_PWRMGMT1Enum;
  typedef enum MPU9250_PWRMGMT2Enum             BSP_MPU9250_PWRMGMT2Enum;
  typedef enum MPU9250_AK8963ST2Enum            BSP_MPU9250_AK8963ST2Enum;
  typedef enum MPU9250_AK8963ModeEnum           BSP_MPU9250_AK8963ModeEnum;
  typedef enum MPU9250_AK8963BITEnum            BSP_MPU9250_AK8963BITEnum;
  typedef enum MPU9250_BitStatusEnum            BSP_MPU9250_BitStatusEnum;  
  typedef struct MPU9250_ConfigStruct           BSP_MPU9250_ConfigStruct;
  typedef struct MPU9250_AK8963ConfigStruct     BSP_MPU9250_AK8963ConfigStruct;
  typedef enum MPU9250_StatusEnum               BSP_MPU9250_StatusEnum;

  // Register Map for Gyroscope and Accelerometer -------------------------------
  enum MPU9250_AddressEnum
  {
    SELF_TEST_X_GYRO = 0x00, // SELF_TEST_X_GYRO Register Address                 
    SELF_TEST_Y_GYRO,        //0x01 SELF_TEST_Y_GYRO Register Address                                                                           
    SELF_TEST_Z_GYRO,        //0x02 SELF_TEST_Z_GYRO Register Address 
    SELF_TEST_X_ACCEL = 0x0D,// SELF_TEST_X_ACCEL Register Address 
    SELF_TEST_Y_ACCEL,       //0x0E SELF_TEST_y_ACCEL Register Address    
    SELF_TEST_Z_ACCEL,       //0x0F SELF_TEST_Z_ACCEL Register Address
    XG_OFFSET_H = 0x13,      // XG_OFFSET_H Register Address
    XG_OFFSET_L,             //0x14 XG_OFFSET_L Register Address
    YG_OFFSET_H,             //0x15 YG_OFFSET_H Register Address
    YG_OFFSET_L,             //0x16 YG_OFFSET_L Register Address
    ZG_OFFSET_H,             //0x17 ZG_OFFSET_H Register Address
    ZG_OFFSET_L,             //0x18 ZG_OFFSET_L Register Address
    SMPLRT_DIV,              //0x19 SMPLRT_DIV Register Address
    CONFIG,                  //0x1A CONFIG Register Address    
    GYRO_CONFIG,             //0x1B GYRO_CONFIG Register Address    
    ACCEL_CONFIG,            //0x1C ACCEL_CONFIG Register Address   
    ACCEL_CONFIG2,           //0x1D ACCEL_CONFIG2 Register Address     
    LP_ACCEL_ODR,            //0x1E LP_ACCEL_ODR Register Address
    WOM_THR,                //0x1F WOM_THR Register Address
    FIFO_EN = 0x23,          // FIFO_EN Register Address
    I2C_MST_CTRL,            //0x24 I2C_MST_CTRL Register Address
    I2C_SLV0_ADDR,           //0x25 I2C_SLV0_ADDR Register Address
    I2C_SLV0_REG,            //0x26 I2C_SLV0_REG Register Address
    I2C_SLV0_CTRL,           //0x27 I2C_SLV0_CTRL Register Address
    I2C_SLV1_ADDR,           //0x28 I2C_SLV1_ADDR Register Address
    I2C_SLV1_REG,            //0x29 I2C_SLV1_REG Register Address
    I2C_SLV1_CTRL,           //0x2A I2C_SLV1_CTRL Register Address
    I2C_SLV2_ADDR,           //0x2B I2C_SLV2_ADDR Register Address
    I2C_SLV2_REG,            //0x2C I2C_SLV2_REG Register Address
    I2C_SLV2_CTRL,           //0x2D I2C_SLV2_CTRL Register Address
    I2C_SLV3_ADDR,           //0x2E I2C_SLV3_ADDR Register Address
    I2C_SLV3_REG,            //0x2F I2C_SLV3_REG Register Address
    I2C_SLV3_CTRL,           //0x30 I2C_SLV3_CTRL Register Address
    I2C_SLV4_ADDR,           //0x31 I2C_SLV4_ADDR Register Address
    I2C_SLV4_REG,            //0x32 I2C_SLV4_REG Register Address
    I2C_SLV4_DO,             //0x33 I2C_SLV4_DO Register Address
    I2C_SLV4_CTRL,           //0x34 I2C_SLV4_CTRL Register Address
    I2C_SLV4_DI,             //0x35 I2C_SLV4_DI Register Address
    I2C_MST_STATUS,          //0x36 I2C_MST_STATUS Register Address
    INT_PIN_CFG,             //0x37 INT_PIN_CFG Register Address
    INT_ENABLE,              //0x38 INT_ENABLE Register Address
    INT_STATUS = 0x3A,       //INT_STATUS Register Address
    ACCEL_XOUT_H,            //0x3B ACCEL_XOUT_H Register Address
    ACCEL_XOUT_L,            //0x3C ACCEL_XOUT_L Register Address
    ACCEL_YOUT_H,            //0x3D ACCEL_YOUT_H Register Address
    ACCEL_YOUT_L,            //0x3E ACCEL_YOUT_L Register Address
    ACCEL_ZOUT_H,            //0x3F ACCEL_ZOUT_H Register Address
    ACCEL_EOUT_L,            //0x40 ACCEL_ZOUT_L Register Address
    TEMP_OUT_H,              //0x41 TEMP_OUT_H Register Address
    TEMP_OUT_L,              //0x42 TEMP_OUT_L Register Address
    GYRO_XOUT_H,             //0x43 GYRO_XOUT_H Register Address
    GYRO_XOUT_L,             //0x44 GYRO_XOUT_L Register Address
    GYRO_YOUT_H,             //0x45 GYRO_YOUT_H Register Address
    GYRO_YOUT_L,             //0x46 GYRO_YOUT_L Register Address
    GYRO_ZOUT_H,             //0x47 GYRO_ZOUT_H Register Address
    GYRO_ZOUT_L,             //0x48 GYRO_ZOUT_L Register Address
    EXT_SENS_DATA_00,        //0x49 EXT_SENS_DATA_00 Register Address
    EXT_SENS_DATA_01,        //0x4A EXT_SENS_DATA_01 Register Address
    EXT_SENS_DATA_02,        //0x4B EXT_SENS_DATA_02 Register Address
    EXT_SENS_DATA_03,        //0x4C EXT_SENS_DATA_03 Register Address
    EXT_SENS_DATA_04,        //0x4D EXT_SENS_DATA_04 Register Address
    EXT_SENS_DATA_05,        //0x4E EXT_SENS_DATA_05 Register Address
    EXT_SENS_DATA_06,        //0x4F EXT_SENS_DATA_06 Register Address
    EXT_SENS_DATA_07,        //0x50 EXT_SENS_DATA_07 Register Address
    EXT_SENS_DATA_08,        //0x51 EXT_SENS_DATA_08 Register Address
    EXT_SENS_DATA_09,        //0x52 EXT_SENS_DATA_09 Register Address
    EXT_SENS_DATA_10,        //0x53 EXT_SENS_DATA_10 Register Address
    EXT_SENS_DATA_11,        //0x54 EXT_SENS_DATA_11 Register Address
    EXT_SENS_DATA_12,        //0x55 EXT_SENS_DATA_12 Register Address
    EXT_SENS_DATA_13,        //0x56 EXT_SENS_DATA_13 Register Address
    EXT_SENS_DATA_14,        //0x57 EXT_SENS_DATA_14 Register Address
    EXT_SENS_DATA_15,        //0x58 EXT_SENS_DATA_15 Register Address
    EXT_SENS_DATA_16,        //0x59 EXT_SENS_DATA_16 Register Address
    EXT_SENS_DATA_17,        //0x5A EXT_SENS_DATA_17 Register Address
    EXT_SENS_DATA_18,        //0x5B EXT_SENS_DATA_18 Register Address
    EXT_SENS_DATA_19,        //0x5C EXT_SENS_DATA_19 Register Address
    EXT_SENS_DATA_20,        //0x5D EXT_SENS_DATA_20 Register Address
    EXT_SENS_DATA_21,        //0x5E EXT_SENS_DATA_21 Register Address
    EXT_SENS_DATA_22,        //0x5F EXT_SENS_DATA_22 Register Address
    EXT_SENS_DATA_23,        //0x60 EXT_SENS_DATA_23 Register Address
    I2C_SLV0_DO = 0x63,      //I2C_SLV0_DO Register Address
    I2C_SLV1_DO,             //0x64 I2C_SLV1_DO Register Address
    I2C_SLV2_DO,             //0x65 I2C_SLV2_DO Register Address
    I2C_SLV3_DO,             //0x66 I2C_SLV3_DO Register Address
    I2C_MST_DELAY_CTRL,      //0x67 I2C_MST_DELAY_CTRL Register Address
    SIGNAL_PATH_RESET,       //0x68 SIGNAL_PATH_RESET Register Address
    MOT_DETECT_CTRL,         //0x69 MOT_DETECT_CTRL Register Address
    USER_CTRL,               //0x6A USER_CTRL Register Address
    PWR_MGMT_1,              //0x6B PWR_MGMT_1 Register Address
    PWR_MGMT_2,              //0x6C PWR_MGMT_2 Register Address
    FIFO_COUNTH = 0x72,      //FIFO_COUNTH Register Address
    FIFO_COUNTL,             //0x73 FIFO_COUNTL Register Address
    FIFO_R_W,                //0x74 FIFO_R_W Register Address
    MPU_9250_WHO_AM_I,       //0x75 WHO_AM_I Register Address
    XA_OFFSET_H = 0x77,      //XA_OFFSET_H Register Address
    XA_OFFSET_L,             //0x78 XA_OFFSET_L Register Address
    YA_OFFSET_H = 0x7A,      //YA_OFFSET_H Register Address
    YA_OFFSET_L,             //0x7B YA_OFFSET_L Register Address
    ZA_OFFSET_H = 0x7D,     //ZA_OFFSET_H Register Address
    ZA_OFFSET_L,             //0x7E ZA_OFFSET_L Register Address
  } ;
  // Register Map for Magnetometer------------------------------------------------  
  enum MPU9250_AK8963_AddressEnum
  {
    AK8963_WIA = 0x00,// AK8963_WIA Register Address
    AK8963_INFO,     // AK8963_INFO Register Address
    AK8963_ST1,      // AK8963_ST1 Register Address
    AK8963_HXL,      // AK8963_HXL Register Address
    AK8963_HXH,      // AK8963_HXH Register Address
    AK8963_HYL,      // AK8963_HYL Register Address
    AK8963_HYH,      // AK8963_HYH Register Address
    AK8963_HZL,      // AK8963_HZL Register Address 
    AK8963_HZH,      // AK8963_HZH Register Address
    AK8963_ST2,      // AK8963_ST2 Register Address
    AK8963_CNTL1,    // AK8963_CNTL Register Address
    AK8963_CNTL2,    // AK8963_CNTL Register Address
    AK8963_ASTC,     // AK8963_ASTC Register Address
    AK8963_TS1,      // AK8963_TS1 Register Address
    AK8963_TS2,      // AK8963_TS2 Register Address
    AK8963_I2CDIS,   // AK8963_I2CDIS Register Address
    AK8963_ASAX,     // AK8963_ASAX Register Address
    AK8963_ASAY,     // AK8963_ASAY Register Address
    AK8963_ASAZ,     // AK8963_ASAZ Register Address
  } ;
  /************************ Exported types ************************************/   
  enum MPU9250_FIFOModeEnum
  {
    Full_FIFO_blocked = 0x00, 
    Full_FIFO_notblocked = 0x40
  };
  
  enum MPU9250_ExtSyncSetEnum
  {
    FSYNC_DIS =         0x00,
    FSYNC_TEMP_OUT_L =  0x08, 
    FSYNC_GYRO_XOUT_L = 0x10,
    FSYNC_GYRO_YOUT_L = 0x18,
    FSYNC_GYRO_ZOUT_L = 0x20,
    FSYNC_ACC_XOUT_L =  0x28,
    FSYNC_ACC_YOUT_L =  0x30,
    FSYNC_ACC_ZOUT_L =  0x38
  };
  
  enum MPU9250_DLPFCfgEnum
  {
    DLPF_CFG_0 = 0x00, 
    DLPF_CFG_1 = 0x01,
    DLPF_CFG_2 = 0x02, 
    DLPF_CFG_3 = 0x03,
    DLPF_CFG_4 = 0x04,
    DLPF_CFG_5 = 0x05,
    DLPF_CFG_6 = 0x06,
    DLPF_CFG_7 = 0x07
  };
  
  enum MPU9250_XGYROCtenEnum
  {
    XGYRO_Cten_Enable = 0x80, 
    XGYRO_Cten_Disable = 0x00
  };
  
  enum MPU9250_YGYROCtenEnum
  {
    YGYRO_Cten_Enable = 0x40, 
    YGYRO_Cten_Disable = 0x00
  };
  
  enum MPU9250_ZGYROCtenEnum
  {
    ZGYRO_Cten_Enable = 0x20, 
    ZGYRO_Cten_Disable = 0x00
  };
  
  enum MPU9250_GyroFSEnum
  {
    FS_250 = 0x00, 
    FS_500 = 0x08,
    FS_1000 = 0x10,
    FS_2000 = 0x18
  };
  
  enum MPU9250_FchoiceBEnum
  {
    Fchoice_b_0 = 0x00, 
    Fchoice_b_1 = 0x01, 
    Fchoice_b_2 = 0x02,
    Fchoice_b_3 = 0x03
  };
  
  enum MPU9250_XAccSTEnum
  {
    AX_ST_Enable = 0x80,
    AX_ST_Disable = 0x00
  };
  
  enum MPU9250_YAccSTEnum
  {
    AY_ST_Enable = 0x40,
    AY_ST_Disable = 0x00
  };
  
  enum MPU9250_ZAccSTEnum
  {
    AZ_ST_Enable = 0x20,
    AZ_ST_Disable = 0x00
  };
  
  enum MPU9250_AccelFSEnum
  {
    FS_2 = 0x00, 
    FS_4 = 0x08,
    FS_8 = 0x10,
    FS_16 = 0x18
  };
  
  enum MPU9250_AccelFchoiceBEnum
  {
    Accel_Fchoice_b_0 = 0x00, 
    Accel_Fchoice_b_1 = 0x04, 
    Accel_Fchoice_b_2 = 0x08,
    Accel_Fchoice_b_3 = 0x0C
  };
  
  enum MPU9250_ADLPFCfgEnum
  {
    ADLPF_CFG_0 = 0x00, 
    ADLPF_CFG_1 = 0x01,
    ADLPF_CFG_2 = 0x02, 
    ADLPF_CFG_3 = 0x03,
  };
  
  enum MPU9250_LposcClkselEnum
  {
    OutPutFreq_0024 = 0x00, 
    OutPutFreq_0029 = 0x01,
    OutPutFreq_0098 = 0x02,
    OutPutFreq_0195 = 0x03,
    OutPutFreq_0391 = 0x04,
    OutPutFreq_0781 = 0x05,
    OutPutFreq_1563 = 0x06,
    OutPutFreq_3125 = 0x07,
    OutPutFreq_6250 = 0x08, 
    OutPutFreq_12500 = 0x09, 
    OutPutFreq_25000 = 0x0A, 
    OutPutFreq_50000 = 0x0B
  };
  
  enum MPU9250_FIFOEnableEnum
  {
    TEMP_BitControl = 0x80, 
    GYRO_XOUT_BitControl = 0x40, 
    GYRO_YOUT_BitControl = 0x20, 
    GYRO_ZOUT_BitControl = 0x10, 
    ACCEL_BitControl = 0x08, 
    SLV2_BitControl = 0x04, 
    SLV1_BitControl = 0x02, 
    SLV0_BitControl = 0x01
  };
  
  enum MPU9250_I2CMasterCntrlEnum
  {
    MULT_MST_BitControl = 0x80, 
    WAIT_FOR_ES_BitControl = 0x40, 
    SLV_3_FIFO_BitControl = 0x20, 
    I2C_MST_P_NSR_BitControl = 0x10, 
  };
  
  enum MPU9250_I2CMSTCLKEnum
  {
    CLK_348 = 0x00,
    CLK_333 ,
    CLK_320 ,
    CLK_308 ,
    CLK_296 ,
    CLK_286 ,
    CLK_276 ,
    CLK_267 ,
    CLK_258 ,
    CLK_500 ,
    CLK_471 ,
    CLK_444 ,
    CLK_421 ,
    CLK_400 ,
    CLK_381 ,
    CLK_364 
  };
  
  enum MPU9250_SLVxRNWEnum
  {
    ReadEnable = 0x80,
    WriteEnable = 0x00
  };
  
  enum MPU9250_SLVxCtrlEnum
  {
    I2C_SLVx_EN = 0x80, 
    I2C_SLVx_BYTE_SW = 0x40, 
    I2C_SLVx_REG_DIS = 0x20, 
    I2C_SLVx_GRP = 0x10
  };
  
  enum MPU9250_SLV4CtrlEnum
  {
    I2C_SLV4_EN = 0x80, 
    SLV4_DONE_INT_E = 0x40, 
    I2C_SLV4_REG_DIS = 0x20
  };
  
  enum MPU9250_I2CMSTStatusEnum
  {
    PASS_THROUGH_BitCtrl = 0x80, 
    I2C_SLV4_DONE_BitCtrl = 0x40, 
    I2C_LOST_ARB_BitCtrl = 0x20, 
    I2C_SLV4_NACK_BitCtrl = 0x10, 
    I2C_SLV3_NACK_BitCtrl = 0x08, 
    I2C_SLV2_NACK_BitCtrl = 0x04,
    I2C_SLV1_NACK_BitCtrl = 0x02, 
    I2C_SLV0_NACK_BitCtrl = 0x01
  };
  
  enum MPU9250_INTPinCFGEnum
  {
    ACTL_BitCtrl = 0x80, 
    OPEN_BitCtrl = 0x40, 
    LATCH_INT_EN_BitCtrl = 0x20, 
    INT_ANYRD_2CLEAR_BitCtrl = 0x10, 
    ACTL_FSYNC_BitCtrl = 0x08, 
    FSYNC_INT_MODE_EN_BitCtrl = 0x04,
    BYPASS_EN_BitCtrl = 0x02
  };
  
  enum MPU9250_INTEnableEnum
  {
    WOM_EN_BitCtrl = 0x40, 
    FIFO_OVERFLOW_EN_BitCtrl = 0x10, 
    FSYNC_INT_EN_BitCtrl = 0x08, 
    RAW_RDY_EN_BitCtrl = 0x01
  };
  
  enum MPU9250_MSTDelayCtrlEnum
  {
    DELAY_ES_SHADOW_BitCtrl = 0x80, 
    I2C_SLV4_DLY_EN_BitCtrl = 0x10, 
    I2C_SLV3_DLY_EN_BitCtrl = 0x08, 
    I2C_SLV2_DLY_EN_BitCtrl = 0x04,
    I2C_SLV1_DLY_EN_BitCtrl = 0x02,
    I2C_SLV0_DLY_EN_BitCtrl = 0x01
  };
  
  enum MPU9250_SignalPathResetEnum
  {
    GYRO_RST_BitCtrl = 0x04,
    ACCEL_RST_BitCtrl = 0x02,
    TEMP_RST_BitCtrl = 0x01
  };
  
  enum MPU9250_MOTDetectCtrlEnum
  {
    ACCEL_INTEL_EN_BitCtrl = 0x80, 
    ACCEL_INTEL_MODE_BitCtrl = 0x40 
  };
  
  enum MPU9250_UserControlEnum
  {
    FIFO_EN_BitCtrl = 0x40, 
    I2C_MST_EN_BitCtrl = 0x20, 
    I2C_IF_DIS_BitCtrl = 0x10, 
    FIFO_RST_BitCtrl = 0x04,
    I2C_MST_RST_BitCtrl = 0x02, 
    SIG_COND_RST_BitCtrl = 0x01
  };
  
  enum MPU9250_PWRMGMT1Enum
  {
    H_RESET_BitCtrl = 0x80, 
    SLEEP_BitCtrl = 0x40, 
    CYCLE_BitCtrl = 0x20, 
    GYRO_STANDBY_BitCtrl = 0x10, 
    PD_PTAT_NACK_BitCtrl = 0x08
  };
  
  enum MPU9250_PWRMGMT2Enum
  {
    DISABLE_XA_BitCtrl = 0x20,
    DISABLE_YA_BitCtrl = 0x10, 
    DISABLE_ZA_BitCtrl = 0x08, 
    DISABLE_XG_BitCtrl = 0x04, 
    DISABLE_YG_BitCtrl = 0x02, 
    DISABLE_ZG_BitCtrl = 0x01
  };
  
  enum MPU9250_AK8963ST2Enum
  {
    BITM_BitCtrl = 0x10,
    HOFL_BitCtrl = 0x08
  };
  
  enum MPU9250_AK8963ModeEnum
  {
    Power_Down                  = 0x00,//"0000":Power-down mode
    Single_Measurement          = 0x01, //"0001": Single measurement mode
    Continuous_Measurement_1    = 0x02,//"0010": Continuous measurement mode 1
    Continuous_Measurement_2    = 0x06,//"0110": Continuous measurement mode 2
    External_Trigger            = 0x04,//"0100": External trigger measurement mode
    Self_Test                   = 0x08,//"1000": Self-test mode
    Fuse_ROM                    = 0x0F//"1111": Fuse ROM access mode
  };
  
  enum MPU9250_AK8963BITEnum
  {
    Setting_14_bit = 0x00,
    Setting_16_bit  = 0x10
  };
  
  enum MPU9250_BitStatusEnum
  {
    Disable       = 0x00,
    Enable        = 0x01
  };
  
  
  struct MPU9250_ConfigStruct
  {
    // PWR_MGMT_1
    uint8_t                     CLK_SEL_Value;
    
    //Config
    BSP_MPU9250_FIFOModeEnum        FIFO_Mode;
    BSP_MPU9250_ExtSyncSetEnum      EXT_SYNC_SET;
    BSP_MPU9250_DLPFCfgEnum         DLPF_CFG;
    
    //Smplrt_Div_Conf
    uint8_t                     SMPLRT_DIV_Value;
    
    //GYRO_CONF
    BSP_MPU9250_XGYROCtenEnum       XGYRO_Cten;
    BSP_MPU9250_YGYROCtenEnum       YGYRO_Cten;
    BSP_MPU9250_ZGYROCtenEnum       ZGYRO_Cten;
    BSP_MPU9250_GyroFSEnum          GYRO_FS_SEL;
    BSP_MPU9250_FchoiceBEnum        FCHOICE_B;
    
    //ACCEL_CONFIG
    BSP_MPU9250_XAccSTEnum          AX_ST_EN;
    BSP_MPU9250_YAccSTEnum          AY_ST_EN;
    BSP_MPU9250_ZAccSTEnum          AZ_ST_EN;
    BSP_MPU9250_AccelFSEnum         ACCEL_FS_SEL;
    
    //ACCEL_CONFIG2
    BSP_MPU9250_AccelFchoiceBEnum   ACCEL_FCHOICE_B;
    BSP_MPU9250_ADLPFCfgEnum        A_DLPF_CFG;    
  };
  
  struct MPU9250_AK8963ConfigStruct
  {
    BSP_MPU9250_AK8963BITEnum       BIT;
    BSP_MPU9250_AK8963ModeEnum      Mode;
  };
  
  enum MPU9250_StatusEnum
  {
    MPU9250_OK       = 0x00,
    MPU9250_ERROR    = 0x01
  };
  
  /* Private define ------------------------------------------------------------*/
#define X_AXIS  0
#define Y_AXIS  1
#define Z_AXIS  2
  /* Exported function prototypes ----------------------------------------------*/
  BSP_MPU9250_StatusEnum BSP_MPU9250_Init(void);
  BSP_MPU9250_StatusEnum BSP_MPU9250_SelfTest(void);
  BSP_MPU9250_StatusEnum BSP_MPU9250_Calibrate(void);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_Config(BSP_MPU9250_ConfigStruct *ConfigStruct);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_AK8963Config(BSP_MPU9250_AK8963ConfigStruct *AK8963ConfigStruct);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_Wakeup();
  BSP_MPU9250_StatusEnum BSP_MPU9250_Reset();
  BSP_MPU9250_StatusEnum BSP_MPU9250_AK8963Reset();
  BSP_MPU9250_StatusEnum BSP_MPU9250_SleepMode();
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_SetRegValue(BSP_MPU9250_AddressEnum Registre, uint8_t value);
  BSP_MPU9250_StatusEnum BSP_MPU9250_SetAK8963RegValue(BSP_MPU9250_AK8963_AddressEnum Registre, uint8_t value);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_GetRegValue(BSP_MPU9250_AddressEnum Registre, uint8_t *value);
  BSP_MPU9250_StatusEnum BSP_MPU9250_GetAK8963RegValue(BSP_MPU9250_AK8963_AddressEnum Registre, uint8_t *value);
  BSP_MPU9250_StatusEnum BSP_MPU9250_GetBufferValue(BSP_MPU9250_AddressEnum offset , uint8_t size, uint8_t *value);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_ReadGyroData(int16_t* GyroData);
  BSP_MPU9250_StatusEnum BSP_MPU9250_ReadAccData(int16_t* Data);
  BSP_MPU9250_StatusEnum BSP_MPU9250_ReadMagData(int16_t* MagnData);
  BSP_MPU9250_StatusEnum BSP_MPU9250_ReadTempData(int16_t* TempData);
  BSP_MPU9250_StatusEnum BSP_MPU9250_ReadFIFOData(int16_t *accel_bias, int16_t *gyro_bias);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_ACCManagement(BSP_MPU9250_BitStatusEnum BitStatus);
  BSP_MPU9250_StatusEnum BSP_MPU9250_GYROManagement(BSP_MPU9250_BitStatusEnum BitStatus);
  
  BSP_MPU9250_StatusEnum BSP_MPU9250_Accelero_ReadFIFOData(uint16_t* fifoSamples, int16_t *accel_data);
  
#endif /*__BSP_MPU9250_H */
  /************************ (C) COPYRIGHT OPCMA ****************END OF FILE****/
  