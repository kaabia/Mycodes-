/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
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

#include <stdio.h>
#include "nm_bsp.h"
#include "nm_common.h"
#include "nm_bus_wrapper.h"
#include "conf_winc.h"
#include "DRV_SPI.h"
//#include "pinout.h"
#include "stm32l4xx_hal.h"

#define NM_BUS_MAX_TRX_SZ	256
SPI_HandleTypeDef hspi;
tstrNmBusCapabilities egstrNmBusCapabilities =
{
	NM_BUS_MAX_TRX_SZ
};

static sint8 spi_rw(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
  unsigned int index;
  
  HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_SELECT_GPIO,CONF_WIFI_PIN_CHIP_SELECT,GPIO_PIN_RESET);

  if (pu8Mosi)
  {
    /* Transmit case : Write only bytes and wait in between 
     * that transmission is done: Transmit buffer Empty
     */
    __IO uint8_t temp;

    for (index=0; index < u16Sz; index ++)
    {
      /* Wait transmit buffer available */
      while (!LL_SPI_IsActiveFlag_TXE(CONF_WIFI_SPIx));

      /* Write byte */
      LL_SPI_TransmitData8(CONF_WIFI_SPIx, pu8Mosi[index]);
      /* Read byte received */
      while (!LL_SPI_IsActiveFlag_RXNE(CONF_WIFI_SPIx));
      temp=LL_SPI_ReceiveData8(CONF_WIFI_SPIx);
    }

  }
  else if (pu8Miso)
  {
    /* Receive case : We are master, so need to write 
     * something to generate the clock so that slave transmitter
     * can send its data */ 

    /* Check that SPI is ready to send */
    while (!LL_SPI_IsActiveFlag_TXE(CONF_WIFI_SPIx));
    for (index=0; index < u16Sz; index ++)
    {
      /* Write trash to receive something */
      LL_SPI_TransmitData8(CONF_WIFI_SPIx, 0);
  
      /* Wait for Received buffer Not Empty */
      while (!LL_SPI_IsActiveFlag_RXNE(CONF_WIFI_SPIx));

      /* Read received 8 bits */
      pu8Miso[index]=LL_SPI_ReceiveData8(CONF_WIFI_SPIx);
    }
  }

  HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_SELECT_GPIO,CONF_WIFI_PIN_CHIP_SELECT,GPIO_PIN_SET);

  return SPI_OK; 
}
/*
static sint8 spi_rw_old(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
	uint8 u8Dummy = 0;
	uint8 u8SkipMosi = 0, u8SkipMiso = 0;
	uint8_t txd_data = 0;
	uint8_t rxd_data = 0;
        SPI_StatusTypeDef spiRetval = SPI_OK; 

	if (!pu8Mosi) {
		pu8Mosi = &u8Dummy;
		u8SkipMosi = 1;
	}
	else if(!pu8Miso) {
		pu8Miso = &u8Dummy;
		u8SkipMiso = 1;
	}
	else {
		return M2M_ERR_BUS_FAIL;
	}

	HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_SELECT_GPIO,CONF_WIFI_PIN_CHIP_SELECT,GPIO_PIN_RESET);

	while (u16Sz) {
		txd_data = *pu8Mosi;

                spiRetval = DRV_SPI_TransmitReceive(&hspi, &txd_data,&rxd_data, 1);
                
                if(spiRetval != SPI_OK){
                  return M2M_ERR_BUS_FAIL;
                }

		*pu8Miso = rxd_data;
		u16Sz--;
		if (!u8SkipMiso)
			pu8Miso++;
		if (!u8SkipMosi)
			pu8Mosi++;
	}

        HAL_GPIO_WritePin(CONF_WIFI_PIN_CHIP_SELECT_GPIO,CONF_WIFI_PIN_CHIP_SELECT,GPIO_PIN_SET);
        
	return M2M_SUCCESS;
}
*/

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*/
sint8 nm_bus_init(void *pvinit)
{
    sint8 result = M2M_SUCCESS;
    DRV_SPI2_Init(&hspi);
    nm_bsp_reset();
    nm_bsp_sleep(1);
          
    return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param[IN]	u8Cmd
*					IOCTL command for the operation
*	@param[IN]	pvParameter
*					Arbitrary parameter depenging on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@note	For SPI only, it's important to be able to send/receive at the same time
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
  sint8 s8Ret = 0;
  switch(u8Cmd)
  {
    case NM_BUS_IOCTL_RW: {
        tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
	s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
    }
    break;
    
    default:
	s8Ret = -1;
	M2M_ERR("invalide ioclt cmd\n");
    break;
  }

  return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*/
sint8 nm_bus_deinit(void)
{
	sint8 result = M2M_SUCCESS;
	return result;
}

/*
*	@fn			nm_bus_reinit
*	@brief		re-initialize the bus wrapper
*	@param [in]	void *config
*					re-init configuration data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
sint8 nm_bus_reinit(void* config)
{
	return M2M_SUCCESS;
}

