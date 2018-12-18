/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IAP_H
#define __IAP_H

#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_exti.h"
#include "DRV_FLASH.h"

/* Private define ------------------------------------------------------------*/
#ifdef CARDIONAUTE_BOARD
#define NRF_GPIO_2_PIN                                  GPIO_PIN_2
#define NRF_GPIO_2_PORT                                 GPIOB
#define NRF_GPIO_2_CLK_ENABLE()                         __HAL_RCC_GPIOB_CLK_ENABLE()
#endif

#define NRF_PIN_INT                                     GPIO_PIN_2
#define NRF_PIN_INT_GPIO                                GPIOA
#define NRF_PIN_INT_GPIO_CLK_ENABLE()                   __GPIOA_CLK_ENABLE()

#define IAP_FLAG_VALUE                                  (0xCDFFFFFFFFFFFFAB)
#define ADDR_FLASH_PAGE_27                              ((uint32_t)0x0800D800)  /* Base @ of Page 27, 2 Kbytes */
#define IAP_FLAG_ADDR                                   ADDR_FLASH_PAGE_27      /* Base @ of Page 27, 2 Kbytes */
#define IAP_FLAG_PAGE                                   (27U)                   /* Page 27 */

#define FLASH_END_ADDR                                  (FLASH_BASE + FLASH_SIZE) - 1
#define IAP_ADDRESS                                     ((uint32_t)0x08000000)

/* Exported functions ------------------------------------------------------------*/
extern void IAP_init(void);

#endif  /* __IAP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
