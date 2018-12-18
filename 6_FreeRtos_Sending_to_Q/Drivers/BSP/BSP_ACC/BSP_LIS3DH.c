/**
******************************************************************************
* $HeadURL: $
* $Author: $
* $Revision:  $
* $Date:$
******************************************************************************
*/

#include "BSP_LIS3DH.h"

/**
* @brief  Initialize the LIS3DH in stream mode.
* @param  Register: address of register
* @param  value: pointer of buffer
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_Init(void)
{
  BSP_LIS3DH_StatusEnum Status;
  
  Status = BSP_LIS3DH_Config();
  Status |= BSP_LIS3DH_EnableStream();
  
  return Status;
}


/** @brief Initializes LIS3DH device with following configuration:
* 100Hz sampling rate
* Enable all axis
* @retval BSP_LIS3DH_StatusEnum
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_Config(void)
{
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  uint8_t valu;
  
  /* Configure LIS3DH device: enable X, Y, Z axis, se 100Hz sampling rate */
  valu = CTRL_REG1_XEN | CTRL_REG1_YEN | CTRL_REG1_ZEN | CTRL_REG1_ODR_100;
  BSP_LIS3DH_SetReg(TEMP_CFG_REG, 0xC0); // temp configuration
  BSP_LIS3DH_SetReg(CTRL_REG4, 0x88); // sensubility +/- 2  
  BSP_LIS3DH_SetReg(CTRL_REG1, valu);
  return (errorValue);
}

/*********************** Getters Function *************************************/

/**
* @brief  Get Register Buffer
* @param  Register: address of register
* @param  value: pointer of buffer
* @retval BSP_LIS3DH_StatusEnum.
*/ 
BSP_LIS3DH_StatusEnum BSP_LIS3DH_GetReg(uint8_t AddrRegister, uint8_t *Datavalue)
{
  //uint8_t buffer;
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  
  i2cError = DRV_I2C_Read(LIS3DH_R_ADDRESS, AddrRegister, Datavalue);
  
  if (i2cError != I2C_OK) 
  {
    errorValue = LIS3DH_ERROR; 
  }
  return errorValue;
}

/************************ Setters functions ***********************************/

/**
* @brief  Set Buffer in Register.
* @param  Register: address of register
* @param  DataBuffer: Buffer to send
* @retval BSP_LIS3DH_StatusEnum
*/ 
BSP_LIS3DH_StatusEnum BSP_LIS3DH_SetReg(uint8_t AddrRegister, uint8_t DataBuffer)
{
  DRV_I2C_StatusTypeDef i2cError;
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  
  i2cError = DRV_I2C_Write(LIS3DH_W_ADDRESS, AddrRegister, DataBuffer);
  
  if (i2cError == I2C_ERROR)
  {
    errorValue = LIS3DH_ERROR;
  }
  return errorValue;
}

/**
* @brief Read the data of Accelerometre.
* @param uint16_t* AccData.
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadAccData(BSP_LIS3DH_Value *AccData, float LIS3DH_Sense)
{
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  uint8_t  DataBuffer[6] ;
  
  errorValue = BSP_LIS3DH_GetBuffer((OUT_X_L|0x80), 6, DataBuffer);
  
  AccData->X = (int16_t)((int16_t)(((int16_t)DataBuffer[1] << 8) | DataBuffer[0]) * LIS3DH_Sense) ;// Turn the MSB and LSB into a signed 16-bit value 
  AccData->Y = (int16_t)((int16_t)(((int16_t)DataBuffer[3] << 8) | DataBuffer[2]) * LIS3DH_Sense) ;  
  AccData->Z = (int16_t)((int16_t)(((int16_t)DataBuffer[5] << 8) | DataBuffer[4]) * LIS3DH_Sense) ; 
  
  return (errorValue);
}

/**
* @brief  Get Buffer Value
* @param  Register: Address of register to be read.
* @param  size: size of buffer to read
* @param  value: data register to be read.
* @retval  BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_GetBuffer(uint8_t AddrRegister , uint8_t size, uint8_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  
  i2cError = DRV_I2C_ReadBuffer(LIS3DH_R_ADDRESS, AddrRegister , 1, value, size);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = LIS3DH_ERROR; 
  }
  
  return errorValue ;
}

/**
* @brief  Check the ID 
* @param  None
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_Detect(void) {
  
  uint8_t id = 0;
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_ERROR;
  /* Get ID */
  BSP_LIS3DH_GetReg(WHO_AM_I, &id);
  /* Check device */
  if (id == WHO_AM_I_VALUE) 
  { 
    errorValue = LIS3DH_OK;
  }
  /* Return Error */
  return errorValue;
  
}

/**
* @brief  ReadTempData. 
* @param  LIS3DH_Sense: LIS3DH sensibility.
* @retval BSP_LIS3DH_StatusEnum
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadTempData(int8_t* TempData)
{
  uint8_t  DataBuffer[2];
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  
  errorValue |= BSP_LIS3DH_GetReg(OUT_ADC3_L, &DataBuffer[0]);
  errorValue |= BSP_LIS3DH_GetReg(OUT_ADC3_H, &DataBuffer[1]);
  *TempData = (((int16_t)DataBuffer[0] << 8) + (int16_t)DataBuffer[1]);
  
  return (errorValue);
}

/**
* @brief  Data Rate command. 
* @param  DataRate: Data rate value.
*   This parameter can be one of the following values:
*     @arg CTRL_REG1_ODR_0: 0 Hz output data rate - Power Down mode
*     @arg CTRL_REG1_ODR_1: 3.125 Hz output data rate 
*     @arg CTRL_REG1_ODR_10: 6.25 Hz output data rate
*     @arg CTRL_REG1_ODR_25: 12.5  Hz output data rate
*     @arg CTRL_REG1_ODR_50: 50 Hz output data rate 
*     @arg CTRL_REG1_ODR_100: 100 Hz output data rate
*     @arg CTRL_REG1_ODR_200: 400 Hz output data rate 
*     @arg CTRL_REG1_ODR_1600: 1.6 kHz output data rate
*     @arg CTRL_REG1_ODR_5000: 5 kHz output data rate
* @retval BSP_LIS3DH_StatusEnum
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_SetDataRate(uint8_t dataRate)
{
  BSP_LIS3DH_StatusEnum errorValue = LIS3DH_OK;
  uint8_t value;
  
  BSP_LIS3DH_GetReg(CTRL_REG1, &value);
  value &= ~(0xF0); // mask off bits
  value |= dataRate ;
  BSP_LIS3DH_SetReg(CTRL_REG1, value);  
  
  return (errorValue);
}

/**
* @brief  Enable the FIFO mode
* @param  None
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_EnableFIFO(void)
{
  BSP_LIS3DH_StatusEnum ErrorValue = LIS3DH_OK;
  
  ErrorValue = BSP_LIS3DH_SetReg(CTRL_REG5, 0x40);
  ErrorValue |= BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x40);
  return ErrorValue;
}

/**
* @brief  Read the FIFO
* @param  ACCE_DATA : value to be read.
* @param  samples : samples number
* @param  LIS3DH_Sense
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadFIFO(BSP_LIS3DH_Value ACCE_DATA [], uint16_t* sample, float LIS3DH_Sense)
{
  uint8_t value;
  BSP_LIS3DH_StatusEnum ErrorValue = LIS3DH_OK;
  
  // ErrorValue = BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x40);
  ErrorValue |= BSP_LIS3DH_GetReg(FIFO_SRC_REG, &value);
  *sample = value&0x1F;
  //__BSW_PRINTF("LIS3DH samples : %d \n",*sample );
  
  for(int i=0; i<*sample; i++)
  {
    ErrorValue |= BSP_LIS3DH_ReadAccData(&ACCE_DATA[i], LIS3DH_Sense);
    
    // __BSW_PRINTF("[ACCEL] X= %d, Y= %d, Z= %d \n", ACCE_DATA[i].X, ACCE_DATA[i].Y, ACCE_DATA[i].Z  );
  }
  
  // reset the FIFO
  // ErrorValue |= BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x0);
  // Re-activate the FIFO
  // ErrorValue = BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x40);
  
  return ErrorValue;
}

/**
* @brief  Enable the Stream mode
* @param  None
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_EnableStream(void)
{
  BSP_LIS3DH_StatusEnum ErrorValue = LIS3DH_OK;
  
  ErrorValue = BSP_LIS3DH_SetReg(CTRL_REG5, 0x40);
  ErrorValue |= BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x80);
  
  return ErrorValue;
}

/**
* @brief  Read in stream mode
* @param  ACCE_DATA : value to be read.
* @param  LIS3DH_Sense
* @retval BSP_LIS3DH_StatusEnum.
*/
BSP_LIS3DH_StatusEnum BSP_LIS3DH_ReadStream(BSP_LIS3DH_Value ACCE_DATA [], float LIS3DH_Sense)
{
  //uint8_t value;
  BSP_LIS3DH_StatusEnum ErrorValue = LIS3DH_OK; 
  /* 
  ErrorValue = BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x80);
  HAL_Delay(350);
  ErrorValue |= BSP_LIS3DH_GetReg(FIFO_SRC_REG, &value);
  if (value&0x40)
  {
  */  
  for(int i=0; i<25; i++)
    ErrorValue |= BSP_LIS3DH_ReadAccData(&ACCE_DATA[i],LIS3DH_Sense);
  /*
  
}
  ErrorValue |= BSP_LIS3DH_SetReg(FIFO_CTRL_REG, 0x0);
  */
  return ErrorValue;
}