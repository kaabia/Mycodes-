/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "nm_bsp.h"
#include "nm_common.h"
#include "stm32l4xx_hal.h"
#include "conf_winc.h"
//#include "pinout.h"
//#include "freeRTOS.h"
//#include "task.h"

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  HAL_GPIO_DeInit(CONF_WIFI_PIN_CHIP_ENABLE_GPIO,CONF_WIFI_PIN_CHIP_ENABLE);
  HAL_GPIO_DeInit(CONF_WIFI_PIN_RESET_GPIO,CONF_WIFI_PIN_RESET);
  
  /* GPIO Ports Clock Enable */
  CONF_WIFI_PIN_RESET_GPIO_CLK_ENABLE();
  CONF_WIFI_PIN_CHIP_ENABLE_GPIO_CLK_ENABLE(); 
  CONF_WIFI_PIN_CHIP_SELECT_GPIO_CLK_ENABLE();

  /* Configure WINC1500-RESET GPIO pin*/
  GPIO_InitStruct.Pin = CONF_WIFI_PIN_RESET;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(CONF_WIFI_PIN_RESET_GPIO, &GPIO_InitStruct);
  
  /* Configure WINC1500-CHIP-ENABLE GPIO pin*/
  GPIO_InitStruct.Pin = CONF_WIFI_PIN_CHIP_ENABLE;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(CONF_WIFI_PIN_CHIP_ENABLE_GPIO, &GPIO_InitStruct);
  
  /* Configure WINC1500-CHIP-SELECT GPIO pin*/
  GPIO_InitStruct.Pin = CONF_WIFI_PIN_CHIP_SELECT;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(CONF_WIFI_PIN_CHIP_SELECT_GPIO, &GPIO_InitStruct);	
}

/*
 *	@fn		nm_bsp_init
 *	@brief	Initialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_init(void)
{

  /* Initialize chip IOs. */
  init_chip_pins();

  /* Perform chip reset. */
  nm_bsp_reset();

  return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_deinit
 *	@brief	De-iInitialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{

  HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_ENABLE_GPIO,CONF_WIFI_PIN_CHIP_ENABLE,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(CONF_WIFI_PIN_RESET_GPIO,CONF_WIFI_PIN_RESET,GPIO_PIN_RESET);
  
  HAL_GPIO_DeInit(CONF_WIFI_PIN_IRQN_GPIO, CONF_WIFI_PIN_IRQN); 
  HAL_GPIO_DeInit(CONF_WIFI_PIN_CHIP_SELECT_GPIO, CONF_WIFI_PIN_CHIP_SELECT);

  HAL_SPI_DeInit(&hspi);
  __SPI2_CLK_DISABLE();
  hif_deinit(NULL);
  
  return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
  HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_ENABLE_GPIO,CONF_WIFI_PIN_CHIP_ENABLE,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(CONF_WIFI_PIN_RESET_GPIO,CONF_WIFI_PIN_RESET,GPIO_PIN_RESET);
  nm_bsp_sleep(100);
  HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_ENABLE_GPIO,CONF_WIFI_PIN_CHIP_ENABLE,GPIO_PIN_SET);
  nm_bsp_sleep(100);
  HAL_GPIO_WritePin(CONF_WIFI_PIN_RESET_GPIO,CONF_WIFI_PIN_RESET,GPIO_PIN_SET);
  nm_bsp_sleep(100);
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
  HAL_Delay(u32TimeMsec);	
}

/*
 *	@fn		nm_bsp_register_isr
 *	@brief	Register interrupt service routine
 *	@param[IN]	pfIsr
 *				Pointer to ISR handler
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
  HAL_GPIO_DeInit(CONF_WIFI_PIN_IRQN_GPIO,CONF_WIFI_PIN_IRQN);
  
  CONF_WIFI_PIN_IRQN_GPIO_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = CONF_WIFI_PIN_IRQN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(CONF_WIFI_PIN_IRQN_GPIO, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, /*tskIDLE_PRIORITY*/+2, 1);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		
}

/*
 *	@fn		nm_bsp_interrupt_ctrl
 *	@brief	Enable/Disable interrupts
 *	@param[IN]	u8Enable
*				'0' disable interrupts. '1' enable interrupts
*/
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
  if (u8Enable) 
  {
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, /*tskIDLE_PRIORITY*/+2, 1);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  }
  else 
  {
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
  }
}
