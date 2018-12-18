/* Includes ------------------------------------------------------------------*/
#include "iap.h"

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private types ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint64_t iap_flag_value = IAP_FLAG_VALUE;
static FLASH_EraseInitTypeDef eraseInitStruct;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void nrf_inter_init(void);
__STATIC_INLINE void nrf_gpio_2_init(void);
__STATIC_INLINE void IAP_mode_enable(void);
__STATIC_INLINE void enter_IAP_mode(void);

static void IAP_flag_erase(void);
static DRV_FLASH_StatusTypeDef IAP_flash_write(uint32_t destination, uint32_t *p_source, uint32_t length);
static void jumpToIAP(void);

/* Private functions ---------------------------------------------------------*/
void EXTI2_IRQHandler(void)
{
  if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    
    nrf_gpio_2_init();
    DRV_FLASH_Init(NULL);
    IAP_mode_enable(); 
    enter_IAP_mode();     
  }
}

void IAP_init(void)
{  
  nrf_inter_init();
  return;
}

/**
  * @brief  Configures EXTI lines 2 (connected to PC.2 pin) in interrupt mode SHOULD BE RECONFIGURED ON THE PINA.2!!!!! 
  * @param  None
  * @retval BLE Status
  */
__STATIC_INLINE void nrf_inter_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  NRF_PIN_INT_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin =  NRF_PIN_INT;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(NRF_PIN_INT_GPIO, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0); // Enable and set EXTI line 2 Interrupt to the lowest priority
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  return;		
}

__STATIC_INLINE void nrf_gpio_2_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
  NRF_GPIO_2_CLK_ENABLE();
  
  /* -2- Configure IOs in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pin = NRF_GPIO_2_PIN;
    
  HAL_GPIO_DeInit(NRF_GPIO_2_PORT, NRF_GPIO_2_PIN);
  HAL_GPIO_Init(NRF_GPIO_2_PORT, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(NRF_GPIO_2_PORT, NRF_GPIO_2_PIN, GPIO_PIN_RESET);  
  return;
}

__STATIC_INLINE void enter_IAP_mode(void)
{  
  if(((*(__IO uint32_t*)IAP_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
  {
    jumpToIAP();
  }
}

static void jumpToIAP(void)
{
  /* Software reset */
  HAL_NVIC_SystemReset();
}
    
static void IAP_flag_erase(void)
{
  uint32_t pageError;
  
  HAL_FLASH_Unlock();
  
  eraseInitStruct.Banks = FLASH_BANK_1;
  eraseInitStruct.Page = IAP_FLAG_PAGE;
  eraseInitStruct.NbPages = 1U;
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
  
  HAL_FLASH_Lock();
  
  return;
}

__STATIC_INLINE void IAP_mode_enable(void)
{
  IAP_flag_erase();
  IAP_flash_write(IAP_FLAG_ADDR, (uint32_t *)&iap_flag_value, 2);
  return;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
static DRV_FLASH_StatusTypeDef IAP_flash_write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  DRV_FLASH_StatusTypeDef status = DRV_FLASH_OK;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* DataLength must be a multiple of 64 bit */
  for (uint32_t i = 0; (i < length/2) && (destination <= (FLASH_END_ADDR-8)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, destination, *((uint64_t *)(p_source+2*i))) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint64_t*)destination != *(uint64_t *)(p_source+2*i))
      {
        /* Flash content doesn't match SRAM content */
        status = DRV_FLASH_WRITINGCTRL_ERROR;
        break;
      }
      /* Increment FLASH destination address */
      destination += 8;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      status = DRV_FLASH_WRITING_ERROR;
      break;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return status;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
