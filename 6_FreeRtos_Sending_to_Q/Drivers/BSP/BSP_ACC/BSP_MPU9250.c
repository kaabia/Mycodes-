/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_MPU9250.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MPU9250_ID_W                0xD0
#define MPU9250_ID_R                0xD1   
#define AK8963_ID               0x0C   

#define MPU9250_DATA_RESET      0x80 
#define MPU9250_DATA_SLEEP      0x40
#define MPU9250_DATA_SLEEP_MASK 0x3F

#define CLK_SEL_MAX_VALUE       0x07
#define RESET_AK8963            0x01

#define DISABLE_ACC             0x38
#define DISABLE_GYRO            0x07
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
BSP_MPU9250_ConfigStruct BSP_MPU9250_GetDefaultConfig(void);
BSP_MPU9250_AK8963ConfigStruct BSP_MPU9250_GetAK8963DefaultConfig(void);

/**
* @brief  Initialization of the MPU9250 Sensor.
* @param  ConfigStruct: pointer of the MPU9250 structure to be set
* @param  AK8963ConfigStruct: pointer of AK8963 structure .
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_Init(void)
{
  BSP_MPU9250_ConfigStruct ConfigStruct;
  BSP_MPU9250_AK8963ConfigStruct AK8963ConfigStruct;
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  // MPU9250 Config
  ConfigStruct = BSP_MPU9250_GetDefaultConfig();
  ConfigStruct.CLK_SEL_Value = ConfGet(MPU9250,CLK);
  
  //AK8963 Config
  AK8963ConfigStruct = BSP_MPU9250_GetAK8963DefaultConfig();
  
  errorValue = BSP_MPU9250_Reset();
  HAL_Delay(100);
  
  errorValue |= BSP_MPU9250_Config(&ConfigStruct);
//  errorValue |= BSP_MPU9250_AK8963Reset();
//  //HAL_Delay(4000);
//  HAL_Delay(100);
//  errorValue |= BSP_MPU9250_AK8963Config(&AK8963ConfigStruct);
  
  return (errorValue);
}
   

BSP_MPU9250_StatusEnum BSP_MPU9250_SelfTest(void)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
  int16_t gAvg[3], aAvg[3], aSTAvg[3], gSTAvg[3];
  uint8_t selfTest[6];
  float factoryTrim[6], destination[6];
  
  errorValue |= BSP_MPU9250_SetRegValue(SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
  errorValue |= BSP_MPU9250_SetRegValue(CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  errorValue |= BSP_MPU9250_SetRegValue(GYRO_CONFIG, 1<<0);  // Set full scale range for the gyro to 250 dps
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG, 1<<0); // Set full scale range for the accelerometer to 2 g
  
  for( int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer
  
  errorValue |= BSP_MPU9250_GetBufferValue(ACCEL_XOUT_H, 6, &rawData[0]);        // Read the six raw data registers into data array
  aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
  errorValue |= BSP_MPU9250_GetBufferValue(GYRO_XOUT_H, 6, &rawData[0]);       // Read the six raw data registers sequentially into data array
  gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  }
  
   for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average current readings
  aAvg[ii] /= 200;
  gAvg[ii] /= 200;
  }
  
  // Configure the accelerometer for self-test
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
  errorValue |= BSP_MPU9250_SetRegValue(GYRO_CONFIG,  0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
  HAL_Delay(25);  // Delay a while to let the device stabilize
  
   for( int ii = 0; ii < 200; ii++) {  // get average self-test values of gyro and acclerometer
  
  errorValue |= BSP_MPU9250_GetBufferValue(ACCEL_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers into data array
  aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
  errorValue |= BSP_MPU9250_GetBufferValue(GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
  gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
  gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  }
  
  for (int ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average self-test readings
  aSTAvg[ii] /= 200;
  gSTAvg[ii] /= 200;
  }   
  
  // Configure the gyro and accelerometer for normal operation
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG, 0x00); 
  errorValue |= BSP_MPU9250_SetRegValue(GYRO_CONFIG,  0x00);
  HAL_Delay(25);  // Delay a while to let the device stabilize
  
   // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_X_ACCEL, (selfTest+0)); // X-axis accel self-test results
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_Y_ACCEL, (selfTest+1)); // Y-axis accel self-test results
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_Z_ACCEL, (selfTest+2)); // Z-axis accel self-test results
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_X_GYRO, (selfTest+3));  // X-axis gyro self-test results
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_Y_GYRO, (selfTest+4));  // Y-axis gyro self-test results
   errorValue |= BSP_MPU9250_GetRegValue(SELF_TEST_Z_GYRO, (selfTest+5));  // Z-axis gyro self-test results

    // Retrieve factory self-test value from self-test code reads
   factoryTrim[0] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
   factoryTrim[1] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
   factoryTrim[2] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
   factoryTrim[3] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
   factoryTrim[4] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
   factoryTrim[5] = (float)(2620/1<<0)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation
 
  // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get percent, must multiply by 100
   for (int i = 0; i < 3; i++) {
     destination[i]   = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i];   // Report percent differences
     destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3]; // Report percent differences
   }
   
   __BSW_PRINTF ("x-axis self test: acceleration trim within :%.2f%% of factory value \n", destination[0]); 
   __BSW_PRINTF ("y-axis self test: acceleration trim within :%.2f%% of factory value \n", destination[1]); 
   __BSW_PRINTF ("z-axis self test: acceleration trim within :%.2f%% of factory value \n", destination[2]); 
   __BSW_PRINTF ("x-axis self test: gyration  trim within :%.2f%% of factory value \n", destination[3]); 
   __BSW_PRINTF ("y-axis self test: gyration  trim within :%.2f%% of factory value \n", destination[4]); 
   __BSW_PRINTF ("z-axis self test: gyration  trim within :%.2f%% of factory value \n", destination[5]); 
   
  return (errorValue);
}

BSP_MPU9250_StatusEnum BSP_MPU9250_Calibrate(void)
{  
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  float dest1[3], dest2[3];
  
 // reset device
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  HAL_Delay(100);
   
 // get stable time source; Auto select clock source to be PLL gyroscope reference if ready 
 // else use the internal oscillator, bits 2:0 = 001
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1, 0x01);  
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_2, 0x00);
  HAL_Delay(200);                                    

// Configure device for bias calculation
  errorValue |= BSP_MPU9250_SetRegValue(INT_ENABLE, 0x00);   // Disable all interrupts
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x00);      // Disable FIFO
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1, 0x00);   // Turn on internal clock source
  errorValue |= BSP_MPU9250_SetRegValue(I2C_MST_CTRL, 0x00); // Disable I2C master
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, 0x0C);    // Reset FIFO and DMP
  HAL_Delay(15);
  
// Configure MPU6050 gyro and accelerometer for bias calculation
  errorValue |= BSP_MPU9250_SetRegValue(CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  errorValue |= BSP_MPU9250_SetRegValue(SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  errorValue |= BSP_MPU9250_SetRegValue(GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity
 
  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

    // Configure FIFO to capture accelerometer and gyro data for bias calculation
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, 0x40);   // Enable FIFO  
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
  HAL_Delay(40); // accumulate 40 samples in 40 milliseconds = 480 bytes

// At end of sample accumulation, turn off FIFO sensor read
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  errorValue |= BSP_MPU9250_GetBufferValue(FIFO_COUNTH, 2, data); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging
  
  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    errorValue |= BSP_MPU9250_GetBufferValue(FIFO_R_W, 12, data); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
    
    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
            
}
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;
    
  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) accelsensitivity;}
   
// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;
  
// Push gyro biases to hardware registers
  errorValue |= BSP_MPU9250_SetRegValue(XG_OFFSET_H, data[0]);
  errorValue |= BSP_MPU9250_SetRegValue(XG_OFFSET_L, data[1]);
  errorValue |= BSP_MPU9250_SetRegValue(YG_OFFSET_H, data[2]);
  errorValue |= BSP_MPU9250_SetRegValue(YG_OFFSET_L, data[3]);
  errorValue |= BSP_MPU9250_SetRegValue(ZG_OFFSET_H, data[4]);
  errorValue |= BSP_MPU9250_SetRegValue(ZG_OFFSET_L, data[5]);
  
// Output scaled gyro biases for display in the main program
  dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity;  
  dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;
  
  __BSW_PRINTF ("x-axis gyro biases result : %f \n", dest1[0]); 
  __BSW_PRINTF ("y-axis gyro biases result : %f \n", dest1[1]); 
  __BSW_PRINTF ("z-axis gyro biases result : %f \n", dest1[2]); 

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  errorValue |= BSP_MPU9250_GetBufferValue( XA_OFFSET_H, 2,data); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  errorValue |= BSP_MPU9250_GetBufferValue(YA_OFFSET_H, 2, data);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  errorValue |= BSP_MPU9250_GetBufferValue(ZA_OFFSET_H, 2, data);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  
  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis
  
  for(ii = 0; ii < 3; ii++) {
    if((accel_bias_reg[ii] & mask)) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }
  
  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);
  
  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers
 
// Apparently this is not working for the acceleration biases in the MPU-9250
// Are we handling the temperature correction bit properly?
// Push accelerometer biases to hardware registers
  errorValue |= BSP_MPU9250_SetRegValue(XA_OFFSET_H, data[0]);
  errorValue |= BSP_MPU9250_SetRegValue(XA_OFFSET_L, data[1]);
  errorValue |= BSP_MPU9250_SetRegValue(YA_OFFSET_H, data[2]);
  errorValue |= BSP_MPU9250_SetRegValue(YA_OFFSET_L, data[3]);
  errorValue |= BSP_MPU9250_SetRegValue(ZA_OFFSET_H, data[4]);
  errorValue |= BSP_MPU9250_SetRegValue(ZA_OFFSET_L, data[5]);

// Output scaled accelerometer biases for display in the main program
   dest2[0] = (float)accel_bias[0]/(float)accelsensitivity; 
   dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
   dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;
   
  __BSW_PRINTF ("x-axis accelerometer biases result : %f \n", 1000*dest2[0]); 
  __BSW_PRINTF ("y-axis accelerometer biases result : %f \n", 1000*dest2[1]); 
  __BSW_PRINTF ("z-axis accelerometer biases result : %f \n", 1000*dest2[2]);
   
   return errorValue;
}


/**
* @brief  Configuration of Accelerometer.
* @param  ConfigStruct: pointer of the ACC structure to be set
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_Config(BSP_MPU9250_ConfigStruct *ConfigStruct)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t dataToSend;
  
  /* Check the ACC structure allocation */
  if(ConfigStruct == NULL)
  {
    return MPU9250_ERROR;
  }
  
  errorValue |= BSP_MPU9250_Wakeup();
  HAL_Delay(100);
  
  // PWR_MGMT_1
  dataToSend = 0x00;
  dataToSend = ConfigStruct->CLK_SEL_Value & CLK_SEL_MAX_VALUE; // max CLK_SEL_Value
  
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1,dataToSend);
  //HAL_Delay(200);
  
  //Config
  dataToSend = 0x00;
  dataToSend = ConfigStruct->FIFO_Mode;
  dataToSend |= ConfigStruct->EXT_SYNC_SET;
  dataToSend |= ConfigStruct->DLPF_CFG;
  
  errorValue |= BSP_MPU9250_SetRegValue(CONFIG,dataToSend);
  
  //Smplrt_Div_Conf
  dataToSend = 0x00;
  dataToSend = ConfigStruct->SMPLRT_DIV_Value;
  
  errorValue |= BSP_MPU9250_SetRegValue(SMPLRT_DIV,dataToSend);
  
  //GYRO_CONF
  dataToSend = 0x00;
  dataToSend = ConfigStruct->XGYRO_Cten;
  dataToSend |= ConfigStruct->YGYRO_Cten;
  dataToSend |= ConfigStruct->ZGYRO_Cten;
  dataToSend |= ConfigStruct->GYRO_FS_SEL;
  dataToSend |= ConfigStruct->FCHOICE_B;
  
  errorValue |= BSP_MPU9250_SetRegValue(GYRO_CONFIG,dataToSend);
  
  //ACCEL_CONFIG
  dataToSend = 0x00;
  dataToSend = ConfigStruct->AX_ST_EN;
  dataToSend |= ConfigStruct->AY_ST_EN;
  dataToSend |= ConfigStruct->AZ_ST_EN;
  dataToSend |= ConfigStruct->ACCEL_FS_SEL;
  
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG,dataToSend);
  
  //ACCEL_CONFIG2
  dataToSend = 0x00;
  dataToSend = ConfigStruct->ACCEL_FCHOICE_B;
  dataToSend |= ConfigStruct->A_DLPF_CFG;
  
  errorValue |= BSP_MPU9250_SetRegValue(ACCEL_CONFIG2,dataToSend);
  HAL_Delay(100);
  
  return errorValue;
}

/**
* @brief  Configuration of Magnetometer.
* @param  AK8963ConfigStruct: pointer of the AK8963 structure to be set
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_AK8963Config(BSP_MPU9250_AK8963ConfigStruct *AK8963ConfigStruct)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t dataToSend;
  
  /* Check the ACC structure allocation */
  if(AK8963ConfigStruct == NULL)
  {
    return MPU9250_ERROR;
  }
  
  dataToSend = 0x00;
  dataToSend = AK8963ConfigStruct->BIT ; 
  dataToSend |= AK8963ConfigStruct->Mode;
  
  errorValue |= BSP_MPU9250_SetAK8963RegValue(AK8963_CNTL1,dataToSend);
  //HAL_Delay(100);
  //HAL_Delay(1000);
  
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV0_ADDR, AK8963_ID|ReadEnable);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV0_REG, AK8963_HXL);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV0_CTRL, 0x87);
HAL_Delay(200);
  
  return errorValue;
}

/**
* @brief  Sleep mode of device.
* @param  none.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_Sleep(void)
{  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t *regValue;
  
  regValue = 0x00;
  
  errorValue |= BSP_MPU9250_GetRegValue(PWR_MGMT_1, regValue);
  //regValue[0] &= MPU9250_DATA_SLEEP_MASK;
  regValue[0] |= MPU9250_DATA_SLEEP;
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1, *regValue); 
  
  return (errorValue);
}

/**
* @brief  Reset the internal registers and restores the default settings of device.
* @param  none.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_Reset(void)
{  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  errorValue = BSP_MPU9250_SetRegValue(PWR_MGMT_1,MPU9250_DATA_RESET);
  
  return errorValue;
}

/**
* @brief  Reset the internal registers and restores the default settings of AK8963.
* @param  none.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_AK8963Reset(void)
{  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  errorValue =  BSP_MPU9250_SetAK8963RegValue(AK8963_CNTL2, RESET_AK8963);
  
  return errorValue;
}

/**
* @brief  wake up device.
* @param  none.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_Wakeup(void)
{
  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t *regValue;
  
  regValue = 0x00;
  
  errorValue |= BSP_MPU9250_GetRegValue(PWR_MGMT_1, regValue);
  regValue[0] &= ~MPU9250_DATA_SLEEP;
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_1, *regValue);
  
  return errorValue ;
}

/**
* @brief  Set Register Value
* @param  Register: Address of register to be Set.
* @param  value: data register to be Set.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_SetRegValue(BSP_MPU9250_AddressEnum Register, uint8_t value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  i2cError = DRV_I2C_Write( MPU9250_ID_W, Register ,value);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MPU9250_ERROR; 
  }
  
  return errorValue ;
}

/**
* @brief  Set AK8963 Register Value
* @param  Register: Address of register to be Set.
* @param  value: data register to be Set.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_SetAK8963RegValue(BSP_MPU9250_AK8963_AddressEnum Register, uint8_t value)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, I2C_MST_EN_BitCtrl);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_ADDR, AK8963_ID|WriteEnable);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_REG, Register);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_DO, value);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_CTRL, I2C_SLV4_EN);
  
  return errorValue ;
}

/**
* @brief  Get Register Value
* @param  Register: Address of register to be read.
* @param  value: data register to be read.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_GetRegValue(BSP_MPU9250_AddressEnum Register, uint8_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  i2cError = DRV_I2C_Read(MPU9250_ID_R, Register ,value);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MPU9250_ERROR; 
  }
  
  return errorValue ;
}

/**
* @brief  Get AK8963 Register Value
* @param  Register: Address of register to be Read.
* @param  value: data register to be read.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_GetAK8963RegValue(BSP_MPU9250_AK8963_AddressEnum Register, 
                                             uint8_t *value)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, I2C_MST_EN_BitCtrl);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_ADDR, AK8963_ID|ReadEnable);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_REG, Register);
  errorValue |= BSP_MPU9250_SetRegValue(I2C_SLV4_CTRL, I2C_SLV4_EN);
  HAL_Delay(200);
  
  errorValue |= BSP_MPU9250_GetRegValue(I2C_SLV4_DI, value);
  
  return errorValue;
  
  //  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  //  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  //  
  //  i2cError = DRV_I2C_Read(AK8963_ID, Register ,value);
  //  
  //  if (i2cError != I2C_OK) 
  //  { 
  //    errorValue = MPU9250_ERROR; 
  //  }
  //  
  //  return errorValue ;
}

/**
* @brief  Get Buffer Value
* @param  Register: Address of register to be read.
* @param  size: size of buffer to read
* @param  value: data register to be read.
* @retval  BSP_MPU9250_StatusEnum.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_GetBufferValue(BSP_MPU9250_AddressEnum RegAddress , uint8_t size, uint8_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  
  i2cError = DRV_I2C_ReadBuffer(MPU9250_ID_R, RegAddress , 1, value, size);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MPU9250_ERROR; 
  }
  
  return errorValue ;
}

/**
* @brief Read the data of AXIS of gyroscope.
* @param uint16_t* GyroData.
* @retval none.
*/

BSP_MPU9250_StatusEnum BSP_MPU9250_ReadGyroData(int16_t* GyroData)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t  DataBuffer[6] ;
  
  errorValue = BSP_MPU9250_GetBufferValue(GYRO_XOUT_H , 6, DataBuffer);
  GyroData[X_AXIS] = ((int16_t)DataBuffer[0] << 8) | (int16_t)DataBuffer[1] ;  // Turn the MSB and LSB into a 16-bit value
  GyroData[Y_AXIS] = ((int16_t)DataBuffer[2] << 8) | (int16_t)DataBuffer[3] ;  
  GyroData[Z_AXIS] = ((int16_t)DataBuffer[4] << 8) | (int16_t)DataBuffer[5] ;  
  
  return (errorValue);
}

/**
* @brief Read the data of AXIS of Temperature.
* @param int16_t* TempData.
* @retval none.
*/

BSP_MPU9250_StatusEnum BSP_MPU9250_ReadTempData(int16_t* TempData)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t  DataBuffer[2] ;
  
  errorValue = BSP_MPU9250_GetBufferValue(TEMP_OUT_H , 2, DataBuffer);
  TempData[0] = ((int16_t)DataBuffer[0] << 8) | (int16_t)DataBuffer[1] ;  // Turn the MSB and LSB into a 16-bit value
  
  return (errorValue);
}

BSP_MPU9250_StatusEnum BSP_MPU9250_ReadFIFOData(int16_t *accel_bias, int16_t *gyro_bias)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  
  errorValue |= BSP_MPU9250_SetRegValue(USER_CTRL, 0x40);   // Enable FIFO  
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO 
  HAL_Delay(40); // accumulate 40 samples in 40 milliseconds = 480 bytes
  
  // At end of sample accumulation, turn off FIFO sensor read
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  errorValue |= BSP_MPU9250_GetBufferValue(FIFO_COUNTH, 2, data); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging
  
  for (ii = 0; ii < packet_count; ii++) {
    errorValue |= BSP_MPU9250_GetBufferValue(FIFO_R_W, 12, &data[0]); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;    
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;
    
    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
    
  }
  accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;
  
  return (errorValue);
}

BSP_MPU9250_StatusEnum BSP_MPU9250_Accelero_ReadFIFOData(uint16_t* fifoSamples, int16_t *accel_data)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint16_t j_fifo = 0;
  int16_t accel_temp[3] = {0, 0, 0};
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  
  // At end of sample accumulation, turn off FIFO sensor read
  errorValue |= BSP_MPU9250_SetRegValue(FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  errorValue |= BSP_MPU9250_GetBufferValue(FIFO_COUNTH, 2, data); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/6;// How many sets of full gyro and accelerometer data for averaging
  *fifoSamples = packet_count;
//  __BSW_PRINTF("MIDDLE_fifo_count = %d \n", packet_count);
  j_fifo = 0;
  for (ii = 0; ii < packet_count; ii++) {
    errorValue |= BSP_MPU9250_GetBufferValue(FIFO_R_W, 6, &data[0]); // read data for averaging
    accel_data[j_fifo] = (int16_t) (((int16_t)data[0] << 8) | data[1]) ;  // Form signed 16-bit integer for each sample in FIFO
    j_fifo++;
    accel_data[j_fifo] = (int16_t) (((int16_t)data[2] << 8) | data[3]) ;
    j_fifo++;
    accel_data[j_fifo] = (int16_t) (((int16_t)data[4] << 8) | data[5]) ;
    j_fifo++;   
  }
//  __BSW_PRINTF("j_fifo = %d \n", j_fifo);
  return (errorValue);
}

/**
* @brief Read the data of Accelerometre.
* @param uint16_t* AccData.
* @retval none.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_ReadAccData(int16_t* Data)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t  DataBuffer[6] ;
  
  errorValue = BSP_MPU9250_GetBufferValue(ACCEL_XOUT_H , 6, DataBuffer);
  Data[X_AXIS] = ((int16_t)DataBuffer[0] << 8) | (int16_t)DataBuffer[1] ;// Turn the MSB and LSB into a signed 16-bit value 
  Data[Y_AXIS] = ((int16_t)DataBuffer[2] << 8) | (int16_t)DataBuffer[3] ;  
  Data[Z_AXIS] = ((int16_t)DataBuffer[4] << 8) | (int16_t)DataBuffer[5] ; 
  
  return (errorValue);
}

/**
* @brief Read the data of Magnetometer.
* @param uint16_t* MagData.
* @retval none.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_ReadMagData(int16_t* MagnData)
{
  
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t  MagData[6] ;
  
  errorValue = BSP_MPU9250_GetBufferValue(EXT_SENS_DATA_00 , 6, MagData);
  MagnData[X_AXIS] = ((int16_t)MagData[1] << 8) | (int16_t)MagData[0] ;  
  MagnData[Y_AXIS] = ((int16_t)MagData[3] << 8) | (int16_t)MagData[2] ;  
  MagnData[Z_AXIS] = ((int16_t)MagData[5] << 8) | (int16_t)MagData[4] ; 
  
  return (errorValue);
}
/**
* @brief Enable or disable ACC module.
* @param BSP_MPU9250_BitStatusEnum can be enable or disable.
* @retval error value.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_ACCManagement(BSP_MPU9250_BitStatusEnum BitStatus)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t regValue;
  
  errorValue |= BSP_MPU9250_GetRegValue(PWR_MGMT_2, &regValue);
  
  if (BitStatus == Enable)
  {
    regValue &= ~DISABLE_ACC;
  }
  else
  {
    regValue |= DISABLE_ACC;
  }
  
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_2, regValue);
  
  return errorValue ;
}
/**
* @brief Enable or disable Gyro module.
* @param BSP_MPU9250_BitStatusEnum can be enable or disable.
* @retval error value.
*/
BSP_MPU9250_StatusEnum BSP_MPU9250_GYROManagement(BSP_MPU9250_BitStatusEnum BitStatus)
{
  BSP_MPU9250_StatusEnum errorValue = MPU9250_OK;
  uint8_t regValue;
  
  errorValue |= BSP_MPU9250_GetRegValue(PWR_MGMT_2, &regValue);
  
  if (BitStatus == Enable)
  {
    regValue &= ~DISABLE_GYRO;
  }
  else
  {
    regValue |= DISABLE_GYRO;
  }
  
  errorValue |= BSP_MPU9250_SetRegValue(PWR_MGMT_2, regValue);
  
  return errorValue ;
}


BSP_MPU9250_ConfigStruct BSP_MPU9250_GetDefaultConfig(void)
{
  BSP_MPU9250_ConfigStruct defaultConfig;
  
  // PWR_MGMT_1
  defaultConfig.CLK_SEL_Value = 0x01;
  //Config
  defaultConfig.FIFO_Mode = Full_FIFO_blocked;
  defaultConfig.EXT_SYNC_SET = FSYNC_DIS;
  defaultConfig.DLPF_CFG = DLPF_CFG_3;
  //Smplrt_Div_Conf
  defaultConfig.SMPLRT_DIV_Value = 0x04;
  //GYRO_CONF
  defaultConfig.XGYRO_Cten = XGYRO_Cten_Disable;
  defaultConfig.YGYRO_Cten = YGYRO_Cten_Disable;
  defaultConfig.ZGYRO_Cten = ZGYRO_Cten_Disable;
  defaultConfig.GYRO_FS_SEL = FS_250;
  defaultConfig.FCHOICE_B = Fchoice_b_0;
  //ACCEL_CONFIG
  defaultConfig.AX_ST_EN = AX_ST_Disable;
  defaultConfig.AY_ST_EN = AY_ST_Disable;
  defaultConfig.AZ_ST_EN = AZ_ST_Disable;
  defaultConfig.ACCEL_FS_SEL = FS_2;
  //ACCEL_CONFIG2
  defaultConfig.ACCEL_FCHOICE_B = Accel_Fchoice_b_0;
  defaultConfig.A_DLPF_CFG = ADLPF_CFG_3;    
  
  return defaultConfig;
}

BSP_MPU9250_AK8963ConfigStruct BSP_MPU9250_GetAK8963DefaultConfig(void)
{
  BSP_MPU9250_AK8963ConfigStruct  AK8963DefaultConfig;
  
  AK8963DefaultConfig.BIT = Setting_16_bit;
  AK8963DefaultConfig.Mode = Continuous_Measurement_1;
  
  return AK8963DefaultConfig;
}
/************************ (C) COPYRIGHT OPCMA ****************END OF FILE****/
