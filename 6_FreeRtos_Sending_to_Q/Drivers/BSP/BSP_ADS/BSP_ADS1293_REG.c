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

extern ADS1293_ADSSel_TypeDef selected_ads; // Default selected ADS


/********************************************************************************************************************************************************************************************/

ADS1293_Status_TypeDef BSP_ADS1293_LL_SPIRead(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count)
{
  
  uint8_t u8pTx[2];
    
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
    
  /* Send Address */
  u8pTx[0] = addr;
  BSP_ADS1293_LL_SPI_RW(u8pTx, NULL, 1 );  
  /* Get Data */
  BSP_ADS1293_LL_SPI_RW(NULL, value, count );
        
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
  
  return ADS1293_OK;
}


void BSP_ADS1293_LL_SPI_RW(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
  unsigned int index;

  if (pu8Mosi)
  {
    /* Transmit case : Write only bytes and wait in between 
     * that transmission is done: Transmit buffer Empty
     */
    __IO uint8_t temp;

    for (index=0; index < u16Sz; index ++)
    {
      /* Wait transmit buffer available */
      while (!LL_SPI_IsActiveFlag_TXE(CONF_ADS_SPIx));

      /* Write byte */
      LL_SPI_TransmitData8(CONF_ADS_SPIx, pu8Mosi[index]);
      /* Read byte received */
      while (!LL_SPI_IsActiveFlag_RXNE(CONF_ADS_SPIx));
      temp=LL_SPI_ReceiveData8(CONF_ADS_SPIx);
    }

  }
  else if (pu8Miso)
  {
    /* Receive case : We are master, so need to write 
     * something to generate the clock so that slave transmitter
     * can send its data */ 

    /* Check that SPI is ready to send */
    while (!LL_SPI_IsActiveFlag_TXE(CONF_ADS_SPIx));
    for (index=0; index < u16Sz; index ++)
    {
      /* Write trash to receive something */
      LL_SPI_TransmitData8(CONF_ADS_SPIx, 0);
  
      /* Wait for Received buffer Not Empty */
      while (!LL_SPI_IsActiveFlag_RXNE(CONF_ADS_SPIx));

      /* Read received 8 bits */
      pu8Miso[index]=LL_SPI_ReceiveData8(CONF_ADS_SPIx);
      
    }
  }

  
}



/**
* @brief  Select the ADS in use
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ADSSel(ADS1293_ADSSel_TypeDef ads_select)
{
  BSP_ADS1293_CSInit();
  
  selected_ads = ads_select;
  
  return ADS1293_OK;
}

/**
* @brief
* @param
* @param
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_SPIWrite(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count)
{
  ADS1293_Status_TypeDef err;
  
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
  
  err = HAL_SPI_Transmit(&SpiHandle, &addr, 1, 2000); 
  
  if(err == HAL_OK)
  {
    err = HAL_SPI_Transmit(&SpiHandle, value, count, 2000);
  }
  
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
  
  return err;
}

/**
* @brief  Writes "value" to the ADS1293 configuration register at address “addr”
* @param  addr: Specifies the address of the configuration register to be written
* @param  value: The value to be written to the configuration register
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_WriteReg(REG8 addr, REG8 *value)
{ 
  ADS1293_Status_TypeDef err; 
  
  err = BSP_ADS1293_SPIWrite((addr &= ADS1293_WR_BIT), value, GPIOC, BSP_ADS1293_GetCSPin(), 1);
  
  return err;
}

/**
* @brief Get CS Pin
* @param
* @param
* @retval ADS1293 Status
*/
GPIO_PIN BSP_ADS1293_GetCSPin(void)
{
  switch (selected_ads)
  {
  case AFE_CSn_FRONT:    
    return GPIO_PIN_1;  
    
  case AFE_CSn_PRECORD1:
    return GPIO_PIN_2;
    
  case AFE_CSn_PRECORD2:
    return GPIO_PIN_3;
    
  case AFE_CSn_PRECORD3:
    return GPIO_PIN_4;
    
  default:
    return GPIO_PIN_1; 
  }
}


/**
* @brief Spi Read n bytes
* @param
* @param
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_SPIRead(REG8 addr, REG8 *value, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t count)
{
 
  ADS1293_Status_TypeDef err; 
  
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
  
  err = HAL_SPI_Transmit(&SpiHandle, &addr, 1, 2000);
  
  if( err == ADS1293_OK )
  {
    err = HAL_SPI_Receive(&SpiHandle, value, count, 2000);
  }
  
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
  
  return ADS1293_OK;
  
  
}

/**
* @brief  Reads a single register at address “addr” and returns the 8-bit value read
* @param  addr: Specifies the address of the register to be read
* @param  value: 8-bit value read at “addr”
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ReadReg(REG8 addr, REG8 *value)
{      
  return (BSP_ADS1293_SPIRead((addr |= ADS1293_RD_BIT), value, GPIOC, BSP_ADS1293_GetCSPin(), 1));
}

ADS1293_Status_TypeDef BSP_ADS1293_AutoIncWriteReg(REG8 addr, REG8 *buffer, uint8_t count)
{   
  return(BSP_ADS1293_SPIWrite((addr &= ADS1293_WR_BIT), buffer, GPIOC, BSP_ADS1293_GetCSPin(), count));
}

ADS1293_Status_TypeDef BSP_ADS1293_AutoIncReadReg(REG8 addr, REG8 *buffer, uint8_t count)
{
  return (BSP_ADS1293_SPIRead((addr |= ADS1293_RD_BIT), buffer, GPIOC, BSP_ADS1293_GetCSPin(), count));
}

ADS1293_Status_TypeDef BSP_ADS1293_SPIStreamReadReg(REG8 *buffer, uint8_t count)
{
  return BSP_ADS1293_AutoIncReadReg(ADS1293_DATA_LOOP_REG, buffer, count);
}

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Register modification.
* @param  regSetMask: data to be written
* @param  regClrMask: mask to be used
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_ModifyReg(REG8 regAddr,  REG8 regSetMask, REG8 regClrMask)
{
  REG8 regVal = 0;
  
  if(BSP_ADS1293_ReadReg(regAddr, &regVal))
    return ADS1293_ERROR;
  
  if(isEqual(regVal, regSetMask, regClrMask))
    return ADS1293_OK;
  
  ADS1293_MODIFY_REG(regVal, regClrMask, regSetMask);
  
  return BSP_ADS1293_WriteReg(regAddr, &regVal);
}

/**
* @brief  Deactivate all CS pins
* @retval ADS1293 Status
*/
ADS1293_Status_TypeDef BSP_ADS1293_CSInit(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
  
  return ADS1293_OK;
}





