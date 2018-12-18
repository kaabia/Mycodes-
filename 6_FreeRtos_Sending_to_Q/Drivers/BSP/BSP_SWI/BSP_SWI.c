/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_SWI.h"
#include "BSP_WIFI.h"
#include "BSP_LED.h"
#include "BSP_ACC.h"
#include "DRV_I2C.h"
#include "DRV_UART.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* BSP init function */
void BSP_Init(void){
  
  //BSP_WIFI_Init();
  //BSP_LED_Init();
  
 /* 
  
  //DRV_I2C_INIT using parameters 
  DRV_I2C_HandleTypeDef i2cTypedef ;
  i2cTypedef.Instance = I2C1;
  i2cTypedef.Timing = 0x00100005;
  DRV_I2C_Init(&i2cTypedef);
  
  
  // BSP_ACC_INIT using parameters 
  BSP_ACC_ConfigStruct ConfigStruct;
  BSP_ACC_AK8963ConfigStruct AK8963ConfigStruct;
  // PWR_MGMT_1
  ConfigStruct.CLK_SEL_Value = 0x01;
  //Config
  ConfigStruct.FIFO_Mode = Full_FIFO_blocked;
  ConfigStruct.EXT_SYNC_SET = FSYNC_DIS;
  ConfigStruct.DLPF_CFG = DLPF_CFG_3;
  //Smplrt_Div_Conf
  ConfigStruct.SMPLRT_DIV_Value = 0x04;
  //GYRO_CONF
  ConfigStruct.XGYRO_Cten = XGYRO_Cten_Disable;
  ConfigStruct.YGYRO_Cten = YGYRO_Cten_Disable;
  ConfigStruct.ZGYRO_Cten = ZGYRO_Cten_Disable;
  ConfigStruct.GYRO_FS_SEL = FS_250;
  ConfigStruct.FCHOICE_B = Fchoice_b_0;
  //ACCEL_CONFIG
  ConfigStruct.AX_ST_EN = AX_ST_Disable;
  ConfigStruct.AY_ST_EN = AY_ST_Disable;
  ConfigStruct.AZ_ST_EN = AZ_ST_Disable;
  ConfigStruct.ACCEL_FS_SEL = FS_2;
  //ACCEL_CONFIG2
  ConfigStruct.ACCEL_FCHOICE_B = Accel_Fchoice_b_0;
  ConfigStruct.A_DLPF_CFG = ADLPF_CFG_3;    
  
  AK8963ConfigStruct.BIT = Setting_16_bit;
  AK8963ConfigStruct.Mode = Continuous_Measurement_1;
  BSP_ACC_Init(&ConfigStruct, &AK8963ConfigStruct);
  HAL_Delay(100);
  */
}

/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/
