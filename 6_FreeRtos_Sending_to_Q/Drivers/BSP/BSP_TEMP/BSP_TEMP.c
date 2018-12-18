/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/**
******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "BSP_TEMP.h"

/* Private function prototypes -----------------------------------------------*/
BSP_TEMP_ConfigurationStruct BSP_TEMP_GetDefaultconfig(void);

/* Exported functions --------------------------------------------------------*/

/**
* @brief  Initialization of the Temperature Sensor
* @param  None.
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_Init(void)
{  
  BSP_TEMP_StatusEnum errorValue;
  BSP_TEMP_ConfigurationStruct config_init;
  
  errorValue = BSP_TEMP_ResetGeneralCall();
  config_init = BSP_TEMP_GetDefaultconfig();
  config_init.Conversion_rate = ConfGet(TEMP,ConvRate); 
  
  errorValue |= BSP_TEMP_SetConfiguration(&config_init);
  
  return errorValue;
}

/**
* @brief  Set Temperature Sensor in Shutdown mode
* @param  None
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_Shutdown()
{ 
  BSP_TEMP_StatusEnum errorValue;
  uint8_t* configValueMSB = 0x00;
  
  errorValue = BSP_TEMP_GetMSBConfig(configValueMSB);
  *configValueMSB |= MASK_SHUTDOWN;
  errorValue |= BSP_TEMP_SetRegBuffer(MODE_CONF, *configValueMSB);  
  
  return errorValue;
}

/**
* @brief  WakeUp Temperature Sensor
* @param  None
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_WakeUp()
{  
  BSP_TEMP_StatusEnum errorValue;
  uint8_t* configValueMSB = 0x00;
  
  errorValue = BSP_TEMP_GetMSBConfig(configValueMSB);
  *configValueMSB &= ~MASK_SHUTDOWN;
  errorValue |= BSP_TEMP_SetRegMSB(MODE_CONF, *configValueMSB);
  
  return errorValue;
}

/**
* @brief  One Shot Temperature Sensor
* @note   This function shall be called only when Sensor is in Shutdown mode
* @param  None
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_OneShot()
{ 
  BSP_TEMP_StatusEnum errorValue;
  uint8_t* configValue = 0x00;
  
  errorValue = BSP_TEMP_GetMSBConfig(configValue);
  *configValue |= MASK_ONESHOT;
  errorValue |= BSP_TEMP_SetRegBuffer(MODE_CONF, *configValue);  
  
  return errorValue;
}

/**
* @brief  Reset Temerature Sensor
* @param  None
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_ResetGeneralCall()
{
  DRV_I2C_StatusTypeDef i2cError;
  BSP_TEMP_StatusEnum errorValue = TEMP_OK;
  uint8_t* Data = (uint8_t*) TEMP_RESET;
  
  i2cError = DRV_I2C_MasterTransmitBuffer(TEMP_ID_GENRAL_CALL, Data, 1);
  
  if (i2cError == I2C_ERROR)
  {
    errorValue = TEMP_ERROR;
  } 
  return errorValue;
}

/************************ Setters functions ***********************************/

/**
* @brief  Set Buffer in Register.
* @param  Register: address of register
* @param  DataBuffer: Buffer to send
* @retval BSP TEMP status value.
*/ 
BSP_TEMP_StatusEnum BSP_TEMP_SetRegBuffer(BSP_TEMP_AddressEnum Register, uint16_t DataBuffer)
{
  DRV_I2C_StatusTypeDef i2cError;
  BSP_TEMP_StatusEnum errorValue = TEMP_OK;
  uint8_t WriteData[2], LSB, MSB;
  
  LSB=(uint8_t)DataBuffer;
  MSB=(uint8_t)(DataBuffer >> 8);
  WriteData[0] = MSB;
  WriteData[1] = LSB;
  
  i2cError = DRV_I2C_WriteBuffer(TEMP_ID_W, Register, 1, WriteData, 2);
  
  if (i2cError == I2C_ERROR)
  {
    errorValue = TEMP_ERROR;
  }
  return errorValue;
}

/**
* @brief  Set MSB in Register.
* @param  Register: address of register.
* @param  MSB: Most significant Byte of register to send
* @retval BSP TEMP status value.
*/ 
BSP_TEMP_StatusEnum BSP_TEMP_SetRegMSB(BSP_TEMP_AddressEnum Register, uint8_t MSB)
{
  DRV_I2C_StatusTypeDef i2cError;
  BSP_TEMP_StatusEnum errorValue = TEMP_OK;
  
  i2cError = DRV_I2C_Write(TEMP_ID_W, Register, MSB);
  
  if (i2cError == I2C_ERROR)
  {
    errorValue = TEMP_ERROR;
  }
  return errorValue;
}

/**
* @brief  Set Module Configuration
* @param  config: Module configuration structure
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_SetConfiguration(BSP_TEMP_ConfigurationStruct* config)
{
  BSP_TEMP_StatusEnum errorValue;
  uint16_t dataToSend = 0;
  
  /* Check the TEMP configuration allocation */
  if(config == NULL)
  {
    return TEMP_ERROR;
  }
  dataToSend = config->Extended_mode;
  dataToSend |= config->Conversion_rate<<6;
  dataToSend |= config->Shutdown;
  dataToSend |= config->Thermostat;
  dataToSend |= config->Polarity;
  dataToSend |= config->Consecutive_faults<<11;
  dataToSend |= config->One_shot;
  
  errorValue = BSP_TEMP_SetRegBuffer(MODE_CONF, dataToSend);
  
  return errorValue;
}

/*********************** Getters Function *************************************/

/**
* @brief  Get Register Buffer
* @param  Register: address of register
* @param  value: pointer of buffer
* @retval BSP TEMP status value.
*/ 
BSP_TEMP_StatusEnum BSP_TEMP_GetRegBuffer(BSP_TEMP_AddressEnum Register, uint16_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_TEMP_StatusEnum errorValue = TEMP_OK;
  uint8_t DataBuffer[2];
  
  i2cError = DRV_I2C_ReadBuffer(TEMP_ID_R, Register, 1, DataBuffer, 2);
  
  *value = ((uint16_t)DataBuffer[0] << 8) | DataBuffer[1];
  
  if (i2cError != I2C_OK) 
  {
    errorValue = TEMP_ERROR; 
  }
  return errorValue;
}

/**
* @brief  Get MSB from Register
* @param  Register: address of register
* @param  MSBvalue: pointer of data 
* @retval BSP TEMP status value.
*/ 
BSP_TEMP_StatusEnum BSP_TEMP_GetRegMSB(BSP_TEMP_AddressEnum Register, uint8_t* MSBvalue)
{
  DRV_I2C_StatusTypeDef i2cError;
  BSP_TEMP_StatusEnum errorValue = TEMP_OK;
  
  i2cError = DRV_I2C_Read(TEMP_ID_R, Register, MSBvalue);
  
  if (i2cError == I2C_ERROR)
  {
    errorValue = TEMP_ERROR;
  } 
  return errorValue;
}

/**
* @brief  Get the module configuration
* @param  configValue: pointer of configuration buffer
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_GetModuleConfig(uint16_t* configValue)
{
  BSP_TEMP_StatusEnum errorValue;
  
  errorValue = BSP_TEMP_GetRegBuffer(MODE_CONF, configValue);
  
  return errorValue;
}

/**
* @brief  Get MSB of module configuration
* @param  configValueMSB: pointer of configuration buffer MSB
* @retval BSP TEMP status value.
*/
BSP_TEMP_StatusEnum BSP_TEMP_GetMSBConfig(uint8_t* configValueMSB)
{
  BSP_TEMP_StatusEnum errorValue;
  
  errorValue = BSP_TEMP_GetRegMSB(MODE_CONF, configValueMSB);
  
  return errorValue;
}

/**
* @brief  Read Temperature Fahrenheit value
* @param  None.
* @retval Fahrenheit value.
*/
float BSP_TEMP_ReadTemperatureF()
{
  float celsius, fahrenheit = 0;
  celsius = BSP_TEMP_ReadTemperatureC();
  fahrenheit = (1.8 * celsius) + 32;
  return fahrenheit;
}

/**
* @brief  Read Temperature Celsius value
* @param  None.
* @retval Celsius value
*/
float BSP_TEMP_ReadTemperatureC()
{
  uint8_t LSB ,MSB;
  uint16_t tempValue[1] = {0}; 
  uint16_t TempSum;
  float celsius;
  uint8_t extended = 0;
  
  BSP_TEMP_GetRegBuffer(TEMP_REG, tempValue);

  LSB = (uint8_t) tempValue[0]; 
  MSB = (uint8_t) (tempValue[0]>>8);
  
  /*Check extended mode*/
  if (LSB & 0x01 != 0)
  {
    extended = 1;
    TempSum = (((uint16_t) MSB << 8) | LSB) >> 3;
  }
  else
  {
    TempSum = (((uint16_t) MSB << 8) | LSB) >> 4;
  }
  
  /*Check negative value*/
  if ((MSB & 0x80) != 0)
  {
    TempSum = ~TempSum;
    if (extended == 1)
    {
      TempSum &= 0x1FFF;
    }
    else
    {
      TempSum &= 0xFFF;
    }
    TempSum++;
    celsius = (-0.0625) * TempSum;
  }
  else
  {
    celsius = 0.0625 * TempSum;
  }
  return celsius;
}

/**
* @brief  Get the default configuration 
* @param  None.
* @retval BSP TEMP ConfigurationStruct.
*/
BSP_TEMP_ConfigurationStruct BSP_TEMP_GetDefaultconfig(void)
{
  BSP_TEMP_ConfigurationStruct defaultConfig;
  
  defaultConfig.Extended_mode = EXTENDED_MODE;
  defaultConfig.Conversion_rate = RATE3;
  defaultConfig.Shutdown = WAKE_MODE;
  defaultConfig.Thermostat = COMPARATOR_MODE;
  defaultConfig.Polarity = INACTIVE_POLARITY;
  defaultConfig.Consecutive_faults = CF1;
  defaultConfig.One_shot = INACTIVE_OS;
  
  return defaultConfig;
}

/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/