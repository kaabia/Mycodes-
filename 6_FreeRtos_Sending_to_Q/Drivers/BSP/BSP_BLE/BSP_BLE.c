/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_BLE.h"

/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define RETURN(STATUS) return((BLE_Status_TypeDef)STATUS)
  
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
/* Exported function  --------------------------------------------------------*/
#define BSP_ERROR_ENUM_LAST_VAL         BLE_INVALID_PARAM
#define BSP_ERROR_ENUM_NUM              (BSP_ERROR_ENUM_LAST_VAL + 1)

static const uint8_t* bsp_ble_err_strings[BSP_ERROR_ENUM_NUM] = {                                       \
                                                                  "BLE_USART_OK (0x0)",                 \
                                                                  "BLE_USART_ERROR (0x01)",             \
                                                                  "BLE_USART_BUSY(0x02)",               \
                                                                  "BLE_USART_TIMEOUT (0x03)",           \
                                                                  "BLE_USART_INVALID_PARAM (0x04)",     \
                                                                  "BLE_CMD_RDY (0x05)",                 \
                                                                  "BLE_CMD_OK (0x06)",                  \
                                                                  "BLE_CMD_ERROR (0x07)",               \
                                                                  "BLE_CMD_INVALID_STATE (0x08)",       \
                                                                  "BLE_CMD_TX_FIFO_NO_MEM (0x09)" ,             \
                                                                  "BLE_CMD_TX_FIFO_NOT_FOUND (0x0A)" ,          \
                                                                  "BLE_CMD_TX_FIFO_ERROR (0x0B)"    ,           \
                                                                  "BLE_CMD_UNKNOWN_ERROR (0x0C)"  ,             \
                                                                  "BLE_INVALID_PARAM (0x0D)"                  \
                                                                };

#define BSP_BLE_PRINT_ERROR(ERR_STRING, ERR_CODE)                               \
  do                                                                            \
  {                                                                             \
    __BSW_PRINTF("> %s: %s\n", ERR_STRING, bsp_ble_err_strings[ERR_CODE]);      \
  }                                                                             \
  while(0)
      

/** @brief Configure BLE relevant GPIO pins
  * @retval BLE Status
  */
void BSP_BLE_Init(void)
{
  BSP_BLE_GPIO2PinInit();       // Init 'BLE_GPIO_2' pin as gpio output, to be used for nRF51 module wackup
  BSP_BLE_INTPinInit();         // Init 'BLE_INT' pin as input with active pull-up to avoid any floating state  
  BSP_BLE_RSTNPinInit();        // Configure and Init BLE Reset Pin 'BLE_SWDIO_RSTN'
  BSP_BLE_HardReset();
}

/** 
  * @brief BLE Command send over the UART port
  * @param command: Command to send
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_CmdSend(BLE_Cmd_TypeDef command)
{
  DRV_USART_StatusTypeDef uerr; // UART Error
  BLE_Status_TypeDef ack;
  
  /* Generate a rising edge on the CONF_BLE_PIN_GPIO pin */
  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_SET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_RESET);
  HAL_Delay(2);
  
  /* Ensure that the nRF51 is ready to receive a command over UART */
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  if(ack != BLE_CMD_RDY)
  {
    return BLE_CMD_UNKNOWN_ERROR;
  }
  
  /* Transmit the command over UART */
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&command, 1, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }  
  return BLE_CMD_OK;
}

/** 
  * @brief Get BLE Configuration Data over BLE
  * @retval BLE Status
  */

BLE_Status_TypeDef BSP_BLE_Get_FW_params(BLE_FW_Version_TypeDef* BLE_FW)
{  
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  uint8_t nrf_id[8] = {0};
  uint8_t nrf_fw_version[3]={0};
  

  err = BSP_BLE_CmdSend(BLE_CMD_DEV_ID);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, nrf_id, 8, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  err = BSP_BLE_CmdSend(BLE_CMD_DEV_FIRMWARE_VER);
  if(err != BLE_CMD_OK)
  {
    return err;
  }

  uerr = DRV_UART_Receive(&huart1, nrf_fw_version, 3, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
   for(uint8_t i = 0; i < 8; i++) BLE_FW->BLE_DEV_ID[i] = nrf_id[i];
  
  BLE_FW->BLE_FW_MajorVersion = nrf_fw_version[0];
  BLE_FW->BLE_FW_MinorVersion = nrf_fw_version[1];
  BLE_FW->BLE_FW_BuildVersion = nrf_fw_version[2];

  return BLE_CMD_OK;
}

BLE_Status_TypeDef BSP_BLE_SendBuffer(uint8_t *buffer, uint16_t buffer_size)
{
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  
  if((buffer == NULL) || (buffer_size == 0))
  {
    return BLE_INVALID_PARAM;
  }
  
  err = BSP_BLE_CmdSend(BLE_CMD_SEND_BUFFER);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&err, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&buffer_size, 2, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Transmit(&huart1, buffer, buffer_size, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&err, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Start advertising in connectable mode
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_AdvStart(void)
{
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  
  err = BSP_BLE_CmdSend(BLE_CMD_ADV_START);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&err, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Start advertising in non connectable mode (iBeacon mode)
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_BeaconAdvStart(void)
//{
//  return BSP_BLE_CmdSend(BLE_CMD_BEACON_ADV_START);
//}

/** 
  * @brief Stop advertising in connectable mode
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_AdvStop(void)
//{  
//  return BSP_BLE_CmdSend(BLE_CMD_ADV_STOP);
//}

/** 
  * @brief Stop advertising in non connectable mode (iBeacon mode)
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_BeaconAdvStop(void)
//{  
//  return BSP_BLE_CmdSend(BLE_CMD_BEACON_ADV_STOP);
//}

/** 
  * @brief Get BLE status
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_GetStackStatus(uint8_t *status)
{
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  
  err = BSP_BLE_CmdSend(BLE_CMD_GET_STACK_STATUS);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, status, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Get BLE status
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_GetNotifStatus(uint8_t *status)
{
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  
  err = BSP_BLE_CmdSend(BLE_CMD_GET_NOTIF_STATUS);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, status, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Get BLE RX Buffer length
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_GetRXBufferLength(uint8_t *rx_buffer_len)
{
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;

  err = BSP_BLE_CmdSend(BLE_CMD_GET_RX_BUFF_LEN);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, rx_buffer_len, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Get BLE Received Data over BLE
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_GetRXBuffer(uint8_t *rx_buffer, uint8_t *data_len)
{  
  BLE_Status_TypeDef err;
  DRV_USART_StatusTypeDef uerr;
  
  if((rx_buffer == NULL) || (data_len == NULL))
  {
    return BLE_INVALID_PARAM;
  }
  
  err = BSP_BLE_CmdSend(BLE_CMD_GET_RX_BUFF);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  uerr = DRV_UART_Receive(&huart1, data_len, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  if(*data_len == 0)
  {
    return BLE_CMD_INVALID_STATE;
  }
  
  uerr = DRV_UART_Receive(&huart1, rx_buffer, *data_len, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  return err;
}

/** 
  * @brief Start/stop data throughput test
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_DataThroughputTest(uint8_t enable)
//{
//  return BSP_BLE_CmdSend(((enable) ? BLE_CMD_DTT_START : BLE_CMD_DTT_STOP));
//}

/** 
  * @brief Disconnection
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_Disconnect(void)
//{
//   return BSP_BLE_CmdSend(BLE_CMD_DISCONNECT);
//}

/** 
  * @brief Send data to the BLE Central device
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_SendPacket(uint8_t *data, uint8_t size)
//{
//  DRV_USART_StatusTypeDef uerr;
//  BLE_Status_TypeDef err, ack;
//  
//  if((data == NULL) || (size == NULL)) // || (size > BLE_PACKET_SIZE))
//  {
//    return BLE_INVALID_PARAM;
//  }
//  
//  if(size > BLE_PACKET_SIZE)
//  {
//    size = BLE_PACKET_SIZE;
//  }
//   
//  err = BSP_BLE_CmdSend(BLE_CMD_SEND_PACKET);
//  if(err != BLE_CMD_OK)
//  {
//    return err;
//  }
//  
//  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(err);
//  }
//  
//  if(ack != BLE_CMD_OK)
//  {
//    return ack;
//  }
//
//  uerr = DRV_UART_Transmit(&huart1, &size, 1, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
////  __BSW_PRINTF("> ");
////  for(uint8_t i = 0; i < size; i++)
////  {
////    __BSW_PRINTF("0x%X,", data[i]);
////  }
////   __BSW_PRINTF("\n");
//  
//  uerr = DRV_UART_Transmit(&huart1, data, size, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  return ack;
//}

//BLE_Status_TypeDef BSP_BLE_SendDataByPacket(uint8_t *data, uint16_t size)
//{
//  uint16_t i = 0;
//  BLE_Status_TypeDef err;
//  
//  if((data == NULL) || (size == NULL))
//  {
//    return BLE_INVALID_PARAM;
//  }
//  
//  for(i = 0; i < (size/BLE_PACKET_SIZE); i++)
//  {
//    err = BSP_BLE_SendPacket(data + (i * BLE_PACKET_SIZE), BLE_PACKET_SIZE);
//    if(err != BLE_CMD_OK)
//    {
//      return err;
//    }
//  }
//  
//  if(size%BLE_PACKET_SIZE)
//  {
//    err = BSP_BLE_SendPacket(data + (i * BLE_PACKET_SIZE), size%BLE_PACKET_SIZE);
//  }
//  
//  return err;
//}

/** 
  * @brief Initialize beacon advertisement data
  * @retval BLE Status
//  */
//BLE_Status_TypeDef BSP_BLE_BeaconInit(BLE_BeaconInit_TypeDef *beacon_init)
//{
//  BLE_Status_TypeDef ack, err;
//  DRV_USART_StatusTypeDef uerr;
//  uint8_t uuid128[16];
//  const char *p;
//
//  if((beacon_init == NULL) || (beacon_init->uuid.uuid128  == NULL))
//  {
//    return BLE_INVALID_PARAM;
//  }
//  
//  p = (const char *)beacon_init->uuid.uuid128;
//  
//  for(uint8_t i = 0; i < 16; i++)
//  {
//    sscanf(p,"%2x", (uint32_t *)(uuid128 + i));
//    p += 2;
//  }
//  
//  err = BSP_BLE_CmdSend(BLE_CMD_BEACON_INIT);
//  if(err != BLE_CMD_OK)
//  {
//    return err;
//  }
//
//  uerr = DRV_UART_Transmit(&huart1, uuid128, 16, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&beacon_init->adv_interval, 4, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&beacon_init->major, 2, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&beacon_init->minor, 2, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&beacon_init->manuf_id, 2, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Transmit(&huart1, &beacon_init->rssi, 1, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//  
//  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_TIMEOUT);
//  if(uerr != USART_OK)
//  {
//    RETURN(uerr);
//  }
//   
//  return ack;
//}

/** 
  * @brief Initialize advertising parameters
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_AdvParamInit(BLE_AdvParam_TypeDef *adv_param)
{
  BLE_Status_TypeDef err, ack;
  DRV_USART_StatusTypeDef uerr;
  
  if((adv_param  == NULL) || (adv_param->dev_name == NULL))
  {
    return BLE_INVALID_PARAM;
  }
  
  adv_param->dev_name_len = (uint8_t)strlen((const char *)adv_param->dev_name);
   
  err = BSP_BLE_CmdSend(BLE_CMD_ADV_PARAM_INIT);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
    
  HAL_Delay(1);
    
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&adv_param->dev_name_len, 1, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)adv_param->dev_name, adv_param->dev_name_len, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&adv_param->ble_adv_fast_interval, 4, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&adv_param->ble_adv_fast_timeout, 4, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
   
  return ack;
}

/** 
  * @brief Initialize connection parameters
  * @retval BLE Status
  */
BLE_Status_TypeDef BSP_BLE_ConnParamInit(BLE_ConnParam_TypeDef *conn_param)
{
  BLE_Status_TypeDef err, ack;
  DRV_USART_StatusTypeDef uerr;
  uint16_t min_conn_interval, max_conn_interval, slave_latency, conn_sup_timeout;
   
  if(conn_param == NULL)
  {
    return BLE_INVALID_PARAM;
  }
   
  err = BSP_BLE_CmdSend(BLE_CMD_CONN_PARAM_INIT);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  HAL_Delay(1);
  
  min_conn_interval = (uint16_t) MSEC_TO_UNITS(conn_param->min_conn_interval, UNIT_1_25_MS); 
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&min_conn_interval, 2, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  max_conn_interval = (uint16_t) MSEC_TO_UNITS(conn_param->max_conn_interval, UNIT_1_25_MS);
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&max_conn_interval, 2, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  slave_latency = (uint16_t) conn_param->slave_latency;
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&slave_latency, 2, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  conn_sup_timeout = (uint16_t) MSEC_TO_UNITS(conn_param->conn_sup_timeout, UNIT_10_MS);
  uerr = DRV_UART_Transmit(&huart1, (uint8_t *)&conn_sup_timeout, 2, UART_TX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
  
  uerr = DRV_UART_Receive(&huart1, (uint8_t *)&ack, 1, UART_RX_TIMEOUT);
  if(uerr != USART_OK)
  {
    RETURN(uerr);
  }
   
  return ack;
}

/** 
  * @brief BLE System Reset
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_SoftReset(void)
//{
//  BLE_Status_TypeDef err; 
//  
//  err =  BSP_BLE_CmdSend(BLE_CMD_SOFT_RESET);
//  if(err != BLE_CMD_OK)
//  {
//    return err;
//  }
//   
//  HAL_Delay(500); // This is a blocking state! osDelay is not recommanded here, for sychronisation purpose with the BLE module
//   
//  return err;
//}


/**
  * @brief  Configures EXTI lines 2 (connected to PC.2 pin) in interrupt mode SHOULD BE RECONFIGURED ON THE PINA.2!!!!! 
  * @param  None
  * @retval BLE Status
  */
void BSP_BLE_INTPinInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  CONF_BLE_PIN_INT_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin =  CONF_BLE_PIN_INT;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(CONF_BLE_PIN_INT_GPIO, &GPIO_InitStruct);
  
  return;		
}

void BSP_BLE_GPIO2PinInit()
{
  CONF_BLE_PIN_GPIO_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = CONF_BLE_PIN_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(CONF_BLE_PIN_GPIO_GPIO, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_SET);
  
  return;
}

void BSP_BLE_RSTNPinInit(void)
{  
#ifdef USE_SEPARATED_RESET

  GPIO_InitTypeDef GPIO_InitStruct;
   
  CONF_BLE_PIN_RSTN_GPIO_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = CONF_BLE_PIN_RSTN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(CONF_BLE_PIN_RSTN_GPIO, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CONF_BLE_PIN_RSTN_GPIO, CONF_BLE_PIN_RSTN, GPIO_PIN_SET);
#endif
  return;
}

/** 
  * @brief Put the BLE device in System-Off Sleep mode 
  * @retval BLE Status
  */
//BLE_Status_TypeDef BSP_BLE_Sleep(void)
//{  
//  return BSP_BLE_CmdSend(BLE_CMD_SLEEP);
//}

/** 
  * @brief Exit System-Off Sleep mode
  * @retval BLE Status
  */
//void BSP_BLE_Wackup(void)
//{  
//  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_SET); // Init the wackup pin to high to ensure the falling edge dectection on the nRF51 side
//  HAL_Delay(1);
//  
//  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_RESET); // Wackup nRF51 module from system-off sleep mode
//  HAL_Delay(1);
//  
//  HAL_GPIO_WritePin(CONF_BLE_PIN_GPIO_GPIO, CONF_BLE_PIN_GPIO, GPIO_PIN_SET); // Drive the wackup pin to low level 
//  HAL_Delay(500); // Wait for nRF51 chip wackup
//  
//  return;
//}

/**
* @brief  Get the default configuration
* @param  None
* @retval BLE Status
*/
void BSP_BLE_GetConfig(BLE_Conf_Typedef *config)
{
  config->beacon_param.uuid.uuid128 = ConfGet(BLE, BEACON_UUID);
  config->beacon_param.adv_interval = ConfGet(BLE, BEACON_ADV_INTERVAL);
  config->beacon_param.major = ConfGet(BLE, BEACON_MAJOR);
  config->beacon_param.minor = ConfGet(BLE, BEACON_MINOR);
  config->beacon_param.rssi = ConfGet(BLE, BEACON_RSSI);
  config->beacon_param.manuf_id = ConfGet(BLE, BEACON_MANUF_ID);
  config->adv_param.dev_name = ConfGet(BLE, DEV_NAME);
  config->adv_param.ble_adv_fast_interval = ConfGet(BLE, ADV_INTERVAL); 
  config->adv_param.ble_adv_fast_timeout = ConfGet(BLE, ADV_TIMEOUT);   
  config->conn_param.min_conn_interval = ConfGet(BLE, MIN_CONN_INTERVAL); 
  config->conn_param.max_conn_interval = ConfGet(BLE, MAX_CONN_INTERVAL); 
  config->conn_param.slave_latency = ConfGet(BLE, SLAVE_LATENCY); 
  config->conn_param.conn_sup_timeout = ConfGet(BLE, CONN_SUP_TIMEOUT);
  
  return;
}

BLE_Status_TypeDef BSP_BLE_SetGeneralConfig(void)
{
  BLE_Status_TypeDef err;
  BLE_Conf_Typedef config;
  
  BSP_BLE_GetConfig(&config);

  err = BSP_BLE_ConnParamInit(&config.conn_param);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
  
  err = BSP_BLE_AdvParamInit(&config.adv_param);
  if(err != BLE_CMD_OK)
  {
    return err;
  }
    
//  return BSP_BLE_BeaconInit(&config.beacon_param);
  return err;
}

void BSP_BLE_HardReset(void)
{
#ifdef USE_SEPARATED_RESET

  // Triggering a falling edge on nRF51 Reset Pin
  HAL_GPIO_WritePin(CONF_BLE_PIN_RSTN_GPIO, CONF_BLE_PIN_RSTN, GPIO_PIN_SET);
  HAL_Delay(1); // osDelay(1); // The BSP_BLE_HardReset is called at startup (FreeRTOS is not yet initailized!), indeed, osDelay() should not be called here!
  HAL_GPIO_WritePin(CONF_BLE_PIN_RSTN_GPIO, CONF_BLE_PIN_RSTN, GPIO_PIN_RESET);
  HAL_Delay(1); // osDelay(1); // Ensure reset detection
  HAL_GPIO_WritePin(CONF_BLE_PIN_RSTN_GPIO, CONF_BLE_PIN_RSTN, GPIO_PIN_SET);
  HAL_Delay(500); // Wait for nRF chip restart
#endif
}

void BSP_BLE_ErrorHandler(uint8_t error_code, uint32_t line_num, const uint8_t* p_file_name)
{
  // NVIC_SystemReset();
  BSP_BLE_PRINT_ERROR(p_file_name, error_code);
}

/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/
