/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_FRAM.h"
#include "DRV_SPI.h"

uint8_t command[9] = {OPCODE_WREN, 
OPCODE_WRDI, 
OPCODE_RDSR, 
OPCODE_WRSR, 
OPCODE_READ, 
OPCODE_WRITE, 
OPCODE_RDID, 
OPCODE_FSTRD, 
OPCODE_SLEEP};

/**
* @brief  Send Enable or Disable write command
* @param  hSpi: SPI handle pointer
* @param  write: choose whether to set the Write Enable Latch or to reset the Write Enable Latch
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_Write_EnableDisable(SPI_HandleTypeDef* hSpi, Write_EnableDisable write)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* If user wants to set Write Enable Latch*/
  if(write == WRITE_ENABLE)
  {
    /* Transmit the WREN command */
    tmp = DRV_SPI_Transmit(&hspi,&command[0], 1);
  }		
  /* If user wants to reset Write Enable Latch*/
  else
  {
    /* Transmit the WRDI command */
    tmp = DRV_SPI_Transmit(&hspi,&command[1], 1);
  }
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  
  
  return retval;
}

/**
* @brief  Write data in the FRAM memory at the specified address
* @param  hSpi: SPI handle pointer
* @param  addr: address from where we start writing data 
* @param  data: the buffer to be written in the FRAM memory 
* @param  size: amount of data to write
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_WriteData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size)
{ 
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  uint8_t addrtmp[3] ;
  
  /* Check if address is not out of range */
  if(addr > 0x0001FFFF)
  {
     return BSP_FRAM_ERROR;
  }
  else
  {
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  /* Transmit the WREN command */
  tmp = DRV_SPI_Transmit(&hspi,&command[0], 1);
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  __NOP();
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the WRITE command */
  tmp = DRV_SPI_Transmit(&hspi,&command[5], 1);
  
  addrtmp[0] = (uint8_t)(addr >> 16);
  addrtmp[1] = (uint8_t)(addr >> 8);
  addrtmp[2] = (uint8_t)(addr & 0x000000FF); 
  
  if(tmp == SPI_OK)
  {		
        tmp  = DRV_SPI_Transmit(&hspi,&addrtmp[0], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[1], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[2], 1);
    
        /* Write data in the FRAM memory */
        for( int i=0;i<size;i++)
        {
         tmp |= DRV_SPI_Transmit(&hspi,&data[i], 1);
        }
  }
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  }
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}

/**
* @brief  Read data from the FRAM memory starting from the specified address
* @param  hSpi: SPI handle pointer
* @param  addr: address from where we start reading data 
* @param  data: read buffer 
* @param  size: amount of data to read
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_ReadData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  uint8_t addrtmp[3] ;
  
  /* Check if address is not out of range */
  if(addr > 0x0001FFFF)
  {
     return BSP_FRAM_ERROR;
  }
  else
  {
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the READ command */
  tmp = DRV_SPI_Transmit(&hspi,&command[4], 1);
  
  addrtmp[0] = (uint8_t)(addr >> 16);
  addrtmp[1] = (uint8_t)(addr >> 8);
  addrtmp[2] = (uint8_t)(addr & 0x000000FF); 
  
  /* Send the address from where we start reading data */
  if(tmp == SPI_OK)
  {
      /* Send the address from where we start writing data into the FRAM memory */
        tmp  = DRV_SPI_Transmit(&hspi,&addrtmp[0], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[1], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[2], 1);
        
        for( int i=0;i<size;i++)
        {
        /* Read data from the FRAM memory */
         tmp |= DRV_SPI_Receive(&hspi,&data[i], 1);
        }
    

  }

  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  }
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}

/**
* @brief  Write value to the status register
* @param  hSpi: SPI handle pointer
* @param  value : value to write in the status register
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_SetStatusRegister(SPI_HandleTypeDef* hSpi,uint8_t value)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the WREN command */
  tmp = DRV_SPI_Transmit(&hspi,&command[0], 1);		
  
  __NOP();
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  __NOP();
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the WRSR command */
  tmp = DRV_SPI_Transmit(&hspi,&command[3], 1);
  
  /* Transmit the value to the status register */
  if(tmp == SPI_OK)
  {
    tmp = DRV_SPI_Transmit(&hspi,&value, 1);
  }
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;
}

/**
* @brief  Get the value of the status register
* @param  hSpi: SPI handle pointer
* @param  data: pointer to store the status register value
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_GetStatusRegister(SPI_HandleTypeDef* hSpi, uint8_t *data)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
 
  /* Transmit the RDSR command */
  tmp = DRV_SPI_Transmit(&hspi,&command[2], 1);
  
  /* Receive the value of the status register */
  if(tmp == SPI_OK)
  {
    tmp = DRV_SPI_Receive(&hspi,data, 1);
  }	
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}

/**
* @brief  Enter Sleep Mode
* @param  hSpi: SPI handle pointer
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_EnterSleepMode(SPI_HandleTypeDef* hSpi)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the SLEEP command */
  tmp = DRV_SPI_Transmit(&hspi,&command[8], 1);
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}

/**
* @brief  Get the device ID value
* @param  hSpi: SPI handle pointer
* @param  data: pointer to store the device ID register
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_ReadDeviceID(SPI_HandleTypeDef* hSpi, uint8_t *ID)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the RDSR command */
  tmp = DRV_SPI_Transmit(&hspi,&command[6], 1);
  HAL_Delay(10);
  
//  while(SPI2->DR == 0);
  
  /* Receive the value of the status register */
  if(tmp == SPI_OK)
  {
    /* Manufacturer ID */ 
    tmp  = DRV_SPI_Receive(&hspi,&ID[0], 1);
    /* Continuation code */
    tmp |= DRV_SPI_Receive(&hspi,&ID[1], 1);
    /* Product ID 1st Byte */
    tmp |= DRV_SPI_Receive(&hspi,&ID[2], 1);
    /* Product ID 2nd Byte */
    tmp |= DRV_SPI_Receive(&hspi,&ID[3], 1);
  }
  
  __NOP();
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}

/**
* @brief  Read data from the FRAM memory starting from the specified address
* @param  hSpi: SPI handle pointer
* @param  addr: address from where we start reading data 
* @param  data: read buffer 
* @param  size: amount of data to read
* @retval BSP_FRAM_STATUS.
*/
BSP_FRAM_STATUS BSP_FRAM_FastReadData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size)
{
  uint32_t tmp ;
  BSP_FRAM_STATUS retval;
  uint8_t addrtmp[4] ;
  
    /* Check if address is not out of range */
  if(addr > 0x0001FFFF)
  {
     return BSP_FRAM_ERROR;
  }
  else
  {
  /* Set Chip Select Pin CS to low level before sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_RESET);
  
  __NOP();
  
  /* Transmit the READ command */
  tmp = DRV_SPI_Transmit(&hspi,&command[7], 1);
  
  addrtmp[0] = (uint8_t)(addr >> 16);
  addrtmp[1] = (uint8_t)(addr >> 8);
  addrtmp[2] = (uint8_t)(addr & 0x000000FF); 
  addrtmp[3] = (uint8_t)(0x33); 
  
  /* Send the address from where we start reading data */
  if(tmp == SPI_OK)
  {
      /* Send the address from where we start writing data into the FRAM memory */
        tmp  = DRV_SPI_Transmit(&hspi,&addrtmp[0], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[1], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[2], 1);
        tmp |= DRV_SPI_Transmit(&hspi,&addrtmp[3], 1);
        
        for( int i=0;i<size;i++)
        {
        /* Read data from the FRAM memory */
         tmp |= DRV_SPI_Receive(&hspi,&data[i], 1);
        }
  }
  __NOP();
  }
  
  /* Set Chip Select Pin CS to high level after sending the command */
  HAL_GPIO_WritePin(CONF_FRAM_PIN_CHIP_SELECT_GPIO,CONF_FRAM_PIN_CHIP_SELECT,GPIO_PIN_SET);
  
  switch (tmp)
  {
  case SPI_OK        : retval= BSP_FRAM_OK; break; 
  case SPI_ERROR     : retval= BSP_FRAM_ERROR; break; 
  case SPI_BUSY      : retval= BSP_FRAM_BUSY; break; 
  case SPI_TIMEOUT   : retval= BSP_FRAM_TIMEOUT; break; 
  }
  
  return retval;	
}