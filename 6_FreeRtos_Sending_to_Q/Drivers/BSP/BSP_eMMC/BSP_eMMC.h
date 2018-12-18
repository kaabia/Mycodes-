/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_eMMC_H
#define __BSP_eMMC_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "DRV_eMMC.h"

/** 
  * @brief eMMC Card information structure 
  */
#define eMMC_CardInfo DRV_eMMC_CardInfoTypedef
   
/** 
  * @brief  eMMC status structure definition  
  */     
#define Emmc_OK         0x00
#define Emmc_ERROR      0x01


#define eMMC_InitTypeDef      SDMMC_InitTypeDef 
#define eMMC_TypeDef          SDMMC_TypeDef

/* DMA definitions for eMMC DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define eMMC_DMAx_Tx_STREAM                 DMA2_Channel4  
#define eMMC_DMAx_Rx_STREAM                 DMA2_Channel5  
#define eMMC_DMAx_Tx_IRQn                   DMA2_Channel4_IRQn
#define eMMC_DMAx_Rx_IRQn                   DMA2_Channel5_IRQn
#define eMMC_DMAx_Tx_IRQHandler             DMA2_Channel4_IRQHandler
#define eMMC_DMAx_Rx_IRQHandler             DMA2_Channel5_IRQHandler


uint8_t BSP_eMMC_Init(void);
uint8_t BSP_eMMC_SleepAwake(DRV_eMMC_Sleep eMMCMode);
uint8_t BSP_eMMC_ITConfig(void);
uint8_t BSP_eMMC_ReadBlocks(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_eMMC_WriteBlocks(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_eMMC_ReadBlocks_DMA(uint8_t *pData, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_eMMC_WriteBlocks_DMA(uint8_t *pData, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumOfBlocks);
uint8_t BSP_eMMC_Erase(uint64_t StartAddr, uint64_t EndAddr);
void BSP_eMMC_IRQHandler(void);
void BSP_eMMC_DMA_Tx_IRQHandler(void);
void BSP_eMMC_DMA_Rx_IRQHandler(void);
uint8_t BSP_eMMCType(void);
DRV_eMMC_TransferStateTypedef BSP_eMMC_GetStatus(void);
void BSP_eMMC_GetCardInfo(DRV_eMMC_CardInfoTypedef *CardInfo);
uint8_t BSP_SD_IsDetected(void);











#endif /*__BSP_eMMC_H */
  /************************ (C) COPYRIGHT OPCMA ****************END OF FILE****/