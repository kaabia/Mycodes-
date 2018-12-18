/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_eMMC.h"

/* Private variables ---------------------------------------------------------*/

eMMC_HandleTypeDef Emmchandle;
static eMMC_CardInfo eMMCCardInfo;

/* Private function prototypes -----------------------------------------------*/
static void eMMC_MspInit(void);
DRV_eMMC_ErrorTypedef eMMC_DMAConfigRx(eMMC_HandleTypeDef *hemmc);
DRV_eMMC_ErrorTypedef eMMC_DMAConfigTx(eMMC_HandleTypeDef *hemmc);

/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the eMMC device.
  * @retval eMMC status.
  */
uint8_t BSP_eMMC_Init(void)
{ 
  uint8_t state = Emmc_OK;
  
  /* eMMC device interface configuration */
  Emmchandle.Instance = SDMMC1;

  Emmchandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  Emmchandle.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_ENABLE;
  Emmchandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  Emmchandle.Init.BusWide             = SDMMC_BUS_WIDE_1B;
  Emmchandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  Emmchandle.Init.ClockDiv            = 10;
  
  /* Check if the SD card is plugged in the slot */
  eMMC_MspInit();

  /* HAL eMMC initialization */
  if(DRV_eMMC_Init(&Emmchandle, &eMMCCardInfo) != eMMC_OK)
  {
    state = Emmc_ERROR;
  }
  
  /* Configure MMC Bus width */
  if(state == Emmc_OK)
  {
    /* Enable wide operation */
    if(DRV_eMMC_WideBusOperation_Config(&Emmchandle, SDMMC_BUS_WIDE_4B) != eMMC_OK)
    {
      state = Emmc_ERROR;
    }
    else
    {
      state = Emmc_OK;
    }
  }
  HAL_Delay(20);
  return  state;
  

}

/**
* @brief  Initializes the eMMC device.
* @retval eMMC status.
*/
uint8_t BSP_eMMC_SleepAwake(DRV_eMMC_Sleep eMMCMode)
{ 
  uint8_t state = Emmc_ERROR;
  uint32_t eMMC_State = 0;
  //uint32_t tickstart = 0;
  //uint32_t tickend = 0;
  //uint32_t elapsedtime = 0;
  
  switch(eMMCMode)
  {
    
  case eMMC_Transfer:
    
    eMMC_State = eMMC_GetState(&Emmchandle);
    
    switch(eMMC_State)
    {
    case eMMC_TRANSFER:
      state = Emmc_OK;
      break;
      
    case eMMC_STANDBY:
      if(eMMC_Select_Deselect(&Emmchandle, (uint32_t)(((uint32_t)&eMMCCardInfo.RCA) << 16))!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
      eMMC_State = eMMC_GetState(&Emmchandle);
      
      if( eMMC_State == eMMC_TRANSFER )
      {
          HAL_Delay(20);
        state = Emmc_OK;
      }
      else
      {
        state = Emmc_ERROR;
      }
    }
      break;
      
    default:
//      tickstart = HAL_GetTick();
      if(eMMC_SetAwakeMode(&Emmchandle)!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
        HAL_Delay(3);
      
      eMMC_State = eMMC_GetState(&Emmchandle);
      
      if ( eMMC_State == eMMC_STANDBY )
      {
        if(eMMC_Select_Deselect(&Emmchandle, (uint32_t)(((uint32_t)&eMMCCardInfo.RCA) << 16))!= eMMC_OK)
        {
          state = Emmc_ERROR;
        }
          else
          {
        eMMC_State = eMMC_GetState(&Emmchandle);
        if( eMMC_State == eMMC_TRANSFER )
        {
          state = Emmc_OK;
        }
        else
        {
          state = Emmc_ERROR;
            }
        }

      }
      else
      {
        state = Emmc_ERROR;
      }
    }
      
      break;
  }
    break;
  
  case eMMC_StdByOnly:
    eMMC_State = eMMC_GetState(&Emmchandle);
    
    if( eMMC_State == eMMC_STANDBY )
    {
      state = Emmc_OK;
    }
    else
    {
      if(eMMC_Deselect(&Emmchandle)!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
      eMMC_State = eMMC_GetState(&Emmchandle);
      
      if( eMMC_State == eMMC_STANDBY )
      {
          HAL_Delay(20);
        state = Emmc_OK;
      }
      else
      {
        state = Emmc_ERROR;
      }
    }
  }
    break;
    
  case eMMC_Sleep:
    
    eMMC_State = eMMC_GetState(&Emmchandle);
    
    switch(eMMC_State)
    {
    case eMMC_STANDBY:
      if(eMMC_SetSleepMode(&Emmchandle)!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
        HAL_Delay(20);
        state = Emmc_OK;
    }
      break;
      
    case eMMC_TRANSFER:
      if(eMMC_Deselect(&Emmchandle)!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
      eMMC_State = eMMC_GetState(&Emmchandle);
      
      if( eMMC_State == eMMC_STANDBY )
      {
        if(eMMC_SetSleepMode(&Emmchandle)!= eMMC_OK)
        {
          state = Emmc_ERROR;
          }
          else
          {
            HAL_Delay(20);
            state = Emmc_OK;
        }
      }
      else
      {
        state = Emmc_ERROR;
        return  state;
      }
    }
      break;
      
    default:
      if(eMMC_SetAwakeMode(&Emmchandle)!= eMMC_OK)
      {
        state = Emmc_ERROR;
      }
      else
      {
      eMMC_State = eMMC_GetState(&Emmchandle);
      
      if( eMMC_State == eMMC_STANDBY )
      {
        if(eMMC_SetSleepMode(&Emmchandle)!= eMMC_OK)
        {
          state = Emmc_ERROR;
          }
          else
          {
            state = Emmc_OK;
        }
      }
      else
      {
        state = Emmc_ERROR;
      }
    }
      break;
      
  }
    
    break;
    
  default:
    state = Emmc_OK;
    break;
    
  }
  
  HAL_Delay(5);
  
    return  state;
  
}


/**
  * @brief  Reads block(s) from a specified address in an MMC card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: eMMC data block size, that should be 512
  * @param  NumOfBlocks: Number of eMMC blocks to read 
  * @retval eMMC status
  */
uint8_t BSP_eMMC_ReadBlocks(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  if(DRV_eMMC_ReadBlocks(&Emmchandle, pData, ReadAddr, BlockSize, NumOfBlocks) != eMMC_OK)
  {
    return Emmc_ERROR;
  }
  else
  {
    return Emmc_OK;
  }
}

uint8_t BSP_eMMCType(void)
{
  uint8_t eMMCType;
   
  eMMCType = DRV_eMMC_GetManufacturerID(&Emmchandle) ;
  return eMMCType;
}


/**
  * @brief  Writes block(s) to a specified address in an eMMC, in polling mode. 
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: eMMC data block size, that should be 512
  * @param  NumOfBlocks: Number of eMMC blocks to write
  * @retval eMMC status
  */
uint8_t BSP_eMMC_WriteBlocks(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{

  if( DRV_eMMC_WriteBlocks(&Emmchandle, pData, WriteAddr, BlockSize, NumOfBlocks) != eMMC_OK)
  {
    return Emmc_ERROR;
  }
  else
  {
    return Emmc_OK;
  }
}

uint32_t BSP_eMMC_GetState()
{
  return (eMMC_GetState(&Emmchandle));
}

  void BSP_GET_eMMCState()
  {

    switch(BSP_eMMC_GetState()) {

     case eMMC_CARD_IDLE :
        __BSW_PRINTF( "-----> eMMC_CARD_IDLE \n" );
        break; /* optional */

     case eMMC_CARD_READY :
        __BSW_PRINTF( "-----> eMMC_CARD_READY \n" );
        break; /* optional */

     case eMMC_IDENTIFICATION :
        __BSW_PRINTF( "-----> eMMC_IDENTIFICATION \n" );
        break; /* optional */

     case eMMC_STANDBY :
        __BSW_PRINTF( "-----> eMMC_STANDBY \n" );
        break; /* optional */

     case eMMC_TRANSFER :
        __BSW_PRINTF( "-----> eMMC_TRANSFER \n" );
        break; /* optional */

     case eMMC_SENDING :
        __BSW_PRINTF( "-----> eMMC_SENDING \n" );
        break; /* optional */

     case eMMC_RECEIVING :
        __BSW_PRINTF( "-----> eMMC_RECEIVING \n" );
        break; /* optional */

     case eMMC_PROGRAMMING :
        __BSW_PRINTF( "-----> eMMC_PROGRAMMING \n" );
        break; /* optional */

     case eMMC_DISCONNECTED :
        __BSW_PRINTF( "-----> eMMC_DISCONNECTED \n" );
        break; /* optional */

     case eMMC_BOOT :
        __BSW_PRINTF( "-----> eMMC_BOOT \n" );
        break; /* optional */

     case eMMC_SLEEP :
        __BSW_PRINTF( "-----> eMMC_SLEEP \n" );
        break; /* optional */

     case eMMC_PROB :
        __BSW_PRINTF( "-----> eMMC_PROB \n" );
        break; /* optional */
    
     /* you can have any number of case statements */
     default : /* Optional */
     __BSW_PRINTF( "-----> eMMC_CARD_IDLE \n" );
  }

}


/**
  * @brief  Reads block(s) from a specified address in an eMMC, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read  
  * @param  BlockSize: eMMC data block size, that should be 512
  * @param  NumOfBlocks: Number of eMMC blocks to read 
  * @retval eMMC status
  */
uint8_t BSP_eMMC_ReadBlocks_DMA(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t state = Emmc_OK;
  
  /* Invalidate the dma tx handle*/
  Emmchandle.hdmatx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((eMMC_DMAConfigRx(&Emmchandle) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
  
  if(state == eMMC_OK)
  {
    /* Read block(s) in DMA transfer mode */
    state = ((DRV_eMMC_ReadBlocks_DMA(&Emmchandle, pData, ReadAddr, BlockSize, NumOfBlocks) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
    /* Wait until transfer is complete */
    if(state == eMMC_OK)
    {
      state = ((DRV_eMMC_CheckReadOperation(&Emmchandle, (uint32_t)eMMC_DATATIMEOUT) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
    }    
  }
  
  return state; 
}

/**
  * @brief  Writes block(s) to a specified address in an eMMC, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written  
  * @param  BlockSize: eMMC data block size, that should be 512
  * @param  NumOfBlocks: Number of eMMC blocks to write 
  * @retval eMMC status
  */
uint8_t BSP_eMMC_WriteBlocks_DMA(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks)
{
  uint8_t state = Emmc_OK;
  
  /* Invalidate the dma rx handle*/
  Emmchandle.hdmarx = NULL;
    
  /* Prepare the dma channel for a read operation */
  state = ((eMMC_DMAConfigTx(&Emmchandle) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
  
  if(state == Emmc_OK)
  { 
    /* Write block(s) in DMA transfer mode */
    state = ((DRV_eMMC_WriteBlocks_DMA(&Emmchandle, pData, WriteAddr, BlockSize, NumOfBlocks) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
    /* Wait until transfer is complete */
    if(state == Emmc_OK)
    {
      state = ((DRV_eMMC_CheckWriteOperation(&Emmchandle, (uint32_t)eMMC_DATATIMEOUT) == eMMC_OK) ? Emmc_OK : Emmc_ERROR);
    }
  }
  
  return state;  
}

/**
  * @brief  Erases the specified memory area of the given eMMC. 
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval eMMC status
  */
uint8_t BSP_eMMC_Erase(uint64_t StartAddr, uint64_t EndAddr)
{
  if(DRV_eMMC_Erase(&Emmchandle, StartAddr, EndAddr) != eMMC_OK)
  {
    return Emmc_ERROR;
  }
  else
  {
    return Emmc_OK;
  }
}

/**
  * @brief  Handles eMMC interrupt request.
  * @retval None
  */
void BSP_eMMC_IRQHandler(void)
{
  DRV_eMMC_IRQHandler(&Emmchandle);
}

/**
  * @brief  Handles eMMC DMA Tx transfer interrupt request.
  * @retval None
  */
void BSP_eMMC_DMA_Tx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Emmchandle.hdmatx); 
}

/**
  * @brief  Handles eMMC DMA Rx transfer interrupt request.
  * @retval None
  */
void BSP_eMMC_DMA_Rx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Emmchandle.hdmarx);
}

/**
  * @brief  Gets the current eMMC data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  eMMC_TRANSFER_OK: No data transfer is acting
  *            @arg  eMMC_TRANSFER_BUSY: Data transfer is acting
  *            @arg  eMMC_TRANSFER_ERROR: Data transfer error 
  */
DRV_eMMC_TransferStateTypedef BSP_eMMC_GetStatus(void)
{
  return(DRV_eMMC_GetStatus(&Emmchandle));
}

/**
  * @brief  Get eMMC information about specific eMMC.
  * @param  CardInfo: Pointer to DRV_eMMC_CardInfoTypedef structure
  * @retval None 
  */
void BSP_eMMC_GetCardInfo(DRV_eMMC_CardInfoTypedef *CardInfo)
{
  /* Get eMMC Information */
  DRV_eMMC_Get_CardInfo(&Emmchandle, CardInfo);
}



/**
  * @brief  Initializes the eMMC MSP.
  * @retval None
  */
static void eMMC_MspInit(void)
{
  GPIO_InitTypeDef gpioinitstruct = {0};
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;
  uint32_t SDMMC_Freq;
  
   DMA_HandleTypeDef hdma_rx;
   DMA_HandleTypeDef hdma_tx;

  /* Configure the Eval SDMMC1 clock source. The clock is derived from the PLLSAI1 */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
  RCC_PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLL;
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    while(1) {}
  }
  
  /* Enable SDMMC1 clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  
  /* Enable DMA2 clocks */
  __DMAx_TxRx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_PULLUP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = GPIO_AF12_SDMMC1;
  
  /* GPIOC configuration */
  gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &gpioinitstruct);

  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
   
  /* NVIC configuration for SDMMC1 interrupts */
  HAL_NVIC_SetPriority(SDMMC1_IRQn,6, 0);
  HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  
  SDMMC_Freq=  HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SDMMC1);
  __BSW_PRINTF( "SDMMC_Freq is = %d, \n", SDMMC_Freq);
  
  /* DMA initialization should be done here but , as there is only one channel for RX and TX it is configured and done directly when required*/
}

/**
  * @brief Configure the DMA to receive data from the eMMC
  * @retval eMMC_ERROR or eMMC_OK
  */
DRV_eMMC_ErrorTypedef eMMC_DMAConfigRx(eMMC_HandleTypeDef *hemmc)
{
  static DMA_HandleTypeDef hdma_rx;
  HAL_StatusTypeDef status = HAL_ERROR;
  
  /* Configure DMA Rx parameters */
  hdma_rx.Init.Request             = DMA_REQUEST_7;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

  hdma_rx.Instance = DMA2_Channel4;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hemmc, hdmarx, hdma_rx);

  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_rx);
  
  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_rx);

  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_rx);
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
  
  return (status != HAL_OK ? eMMC_ERROR : eMMC_OK);
}

/**
  * @brief Configure the DMA to transmit data to the eMMC
  * @retval eMMC_ERROR or eMMC_OK
  */
DRV_eMMC_ErrorTypedef eMMC_DMAConfigTx(eMMC_HandleTypeDef *hemmc)
{
  static DMA_HandleTypeDef hdma_tx;
  HAL_StatusTypeDef status;
  
  /* Configure DMA Tx parameters */
  hdma_tx.Init.Request             = DMA_REQUEST_7;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  
  hdma_tx.Instance = DMA2_Channel4;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hemmc, hdmatx, hdma_tx);
  
  /* Stop any ongoing transfer and reset the state*/
  HAL_DMA_Abort(&hdma_tx);
  
  /* Deinitialize the Channel for new transfer */
  HAL_DMA_DeInit(&hdma_tx);
  
  /* Configure the DMA Channel */
  status = HAL_DMA_Init(&hdma_tx); 
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);

  return (status != HAL_OK ? eMMC_ERROR : eMMC_OK);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


