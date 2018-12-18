/**
  ******************************************************************************
  * $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_BLE/BSP_BLE.h $
  * $Author: ghada.dhibi $
  * $Revision: 559 $
  * $Date: 2016-09-22 15:55:20 +0100 (jeu., 22 sept. 2016) $
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_BLE_H
#define __BSP_BLE_H

#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "DRV_UART.h"
    
/**@brief To convert milliseconds to ticks.
 * @param[in] TIME          Number of milliseconds to convert.
 * @param[in] RESOLUTION    Unit to be converted to in [us/ticks].
 */
#define MSEC_TO_UNITS(TIME, RESOLUTION) (((TIME) * 1000) / (RESOLUTION))
  

#define BSP_BLE_ERROR_HANDLER(ERR_STRING, ERR_CODE)             \
  do                                                            \
  {                                                             \
    BSP_BLE_ErrorHandler((ERR_CODE), 0, ERR_STRING);            \
  }                                                             \
  while (0)

#define BSP_BLE_ERROR_CHECK(ERR_STRING, ERR_CODE)               \
  do                                                            \
  {                                                             \
    const uint8_t* LOCAL_ERR_STRING     = ERR_STRING;           \
    const uint8_t LOCAL_ERR_CODE        = (ERR_CODE);           \
    if (LOCAL_ERR_CODE != BLE_CMD_OK)                           \
    {                                                           \
      BSP_BLE_ERROR_HANDLER(LOCAL_ERR_STRING, LOCAL_ERR_CODE);  \
    }                                                           \
  }                                                             \
  while(0)
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_TIMEOUT            1000U
#define UART_RX_TIMEOUT         UART_TIMEOUT
#define UART_TX_TIMEOUT         UART_TIMEOUT        
    
#define BLE_PACKET_SIZE         20U
      
#define HAL_UART_ERROR_LAST_NUM USART_INVALID_PARAM
#define BLE_ERROR_BASE_NUM      (HAL_UART_ERROR_LAST_NUM + 1)
  
typedef enum BLE_Status_Enum BLE_Status_TypeDef;
enum BLE_Status_Enum
{
  BLE_USART_OK                  = USART_OK,                     // 0x0
  BLE_USART_ERROR               = USART_ERROR,                  // 0x1
  BLE_USART_BUSY                = USART_BUSY,                   // 0x2
  BLE_USART_TIMEOUT             = USART_TIMEOUT,                // 0x3
  BLE_USART_INVALID_PARAM       = USART_INVALID_PARAM,          // 0x4
  BLE_CMD_RDY                   = (BLE_ERROR_BASE_NUM + 0),     // 0x5
  BLE_CMD_OK                    = (BLE_ERROR_BASE_NUM + 1),     // 0x6
  BLE_CMD_ERROR                 = (BLE_ERROR_BASE_NUM + 2),     // 0x7
  BLE_CMD_INVALID_STATE         = (BLE_ERROR_BASE_NUM + 3),     // 0x8
  BLE_CMD_TX_FIFO_NO_MEM        = (BLE_ERROR_BASE_NUM + 4),	// 0x9  TX FIFO is full.
  BLE_CMD_TX_FIFO_NOT_FOUND  	= (BLE_ERROR_BASE_NUM + 5),	// 0xA  TX FIFO is empty.
  BLE_CMD_TX_FIFO_ERROR		= (BLE_ERROR_BASE_NUM + 6),	// 0xB  TX FIFO error
  BLE_CMD_UNKNOWN_ERROR         = (BLE_ERROR_BASE_NUM + 7),     // 0xC 
  BLE_INVALID_PARAM             = (BLE_ERROR_BASE_NUM + 8),     // 0xD
};

typedef enum BLE_Stack_Status_Enum BLE_Stack_Status_TypeDef;
enum BLE_Stack_Status_Enum
{
  BLE_STACK_STATUS_IDLE = 0x00,         // BLE Stack is not advertising yet
  BLE_STACK_STATUS_ADV = 0x01,          // BLE is advertising in connectable mode
  BLE_STACK_STATUS_CONNECTED = 0x02,    // BLE Peripheral is now connected to a Central
  BLE_STACK_STATUS_DISCONNECTED = 0x03, // BLE Peripheral has disconnected
  BLE_STACK_STATUS_OTHER = 0xFF, // BLE Peripheral has disconnected
};

typedef enum BLE_TimeRes_Enum BLE_TimeRes_TypeDef;
enum BLE_TimeRes_Enum 
{
  UNIT_0_625_MS = 625,          // Number of microseconds in 0.625 milliseconds
  UNIT_1_25_MS  = 1250,         // Number of microseconds in 1.25 milliseconds
  UNIT_10_MS    = 10000,        // Number of microseconds in 10 milliseconds
};

typedef struct BLE_FW_Version_Struct BLE_FW_Version_TypeDef; // nRF Firmware informations
struct BLE_FW_Version_Struct
 {
  uint8_t BLE_DEV_ID[8];
  uint8_t BLE_FW_MajorVersion;
  uint8_t BLE_FW_MinorVersion;
  uint8_t BLE_FW_BuildVersion;
  };

typedef struct BLE_UUID128_Struct BLE_UUID128_TypeDef; // 128 bit UUID values
struct BLE_UUID128_Struct
{ 
  const uint8_t *uuid128;
};

typedef struct BLE_BeaconInit_Struct BLE_BeaconInit_TypeDef;
struct BLE_BeaconInit_Struct
{
  BLE_UUID128_TypeDef uuid;
  uint32_t adv_interval;
  uint16_t major;
  uint16_t minor;
  uint16_t manuf_id;
  uint8_t rssi; /** measured RSSI at 1 meter distance in dBm*/  
};

typedef struct BLE_AdvParam_Struct BLE_AdvParam_TypeDef;
struct BLE_AdvParam_Struct
{
  uint8_t dev_name_len;                 // Device Name string lenght
  const uint8_t *dev_name;              // Device Name string
  const uint8_t *dev_session;              // Device Name string
  uint32_t ble_adv_fast_interval;       // Advertising interval for fast advertising
  uint32_t ble_adv_fast_timeout;        // Time-out (in seconds) for fast advertising
};

typedef struct BLE_ConnParam_Struct BLE_ConnParam_TypeDef;
struct BLE_ConnParam_Struct
{
  float min_conn_interval; // Minimum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS
  float max_conn_interval; // Maximum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS
  float slave_latency; // Slave Latency in number of connection events, see @ref BLE_GAP_CP_LIMITS.
  float conn_sup_timeout; // Connection Supervision Timeout in 10 ms units, see @ref BLE_GAP_CP_LIMITS
  
};

typedef enum BLE_Cmd_Enum BLE_Cmd_TypeDef;
enum BLE_Cmd_Enum
{
  BLE_CMD_NULL = 0x00,
  BLE_CMD_ADV_START = 0x01,                             // Start BLE Connectable adverting mode
  BLE_CMD_ADV_STOP = 0x02,                              // Stop BLE Connectable adverting mode
  BLE_CMD_BEACON_ADV_START = 0x03,                      // Start BLE ibeacon adverting mode
  BLE_CMD_BEACON_ADV_STOP = 0x04,                       // Stop BLE ibeacon adverting mode
  BLE_CMD_DISCONNECT = 0x05,                            // BLE disconnection command
  BLE_CMD_GET_STACK_STATUS = 0x06,                      // Requesting current BLE status (Advertising, connected, disconnected,...)
  BLE_CMD_GET_RX_BUFF_LEN = 0x07,                    // Send Config. data
  BLE_CMD_GET_RX_BUFF = 0x08,                           // Asking for Config. data availability on the nRF51 module
  BLE_CMD_DTT_START = 0x09,                             // Starting data Throughput Test
  BLE_CMD_DTT_STOP = 0x0A,                              // Stoping data Throughput Test
  BLE_CMD_SEND_PACKET = 0x0B,                              // Notification to the nRF51 module of starting data transmission to a central device 
  BLE_CMD_SOFT_RESET = 0x0C,                            // Request a Soft System Reset of the nRF51 module
//  BLE_CMD_INT = 0x0D,                                 // used for testing purpose of the INT GPIO [STM32 <------INT------ nRF51]
  BLE_CMD_BEACON_INIT = 0x0E,
  BLE_CMD_ADV_PARAM_INIT = 0x0F, 			// Init. adv. parameters
  BLE_CMD_CONN_PARAM_INIT = 0x10,			// Init. conn. parameters
  BLE_CMD_DEV_ID = 0x11,                                // Get device id
  BLE_CMD_SLEEP = 0x12,                                 // Put device into System-Off sleep mode
  BLE_CMD_SEND_BUFFER = 0x13, 		                // Init FIFO buffer
  BLE_CMD_GET_NOTIF_STATUS  = 0x14,                     // Get Notifcation status (Enabled/Disabled)
   BLE_CMD_DEV_FIRMWARE_VER = 0x15,                      // Get device firmware version
};

typedef struct CONF_BLE_Struct BLE_Conf_Typedef;

extern void BSP_BLE_Init(void);
extern BLE_Status_TypeDef BSP_BLE_CmdSend(BLE_Cmd_TypeDef command);
extern BLE_Status_TypeDef BSP_BLE_AdvStart(void);
extern BLE_Status_TypeDef BSP_BLE_AdvStop(void);
extern BLE_Status_TypeDef BSP_BLE_BeaconInit(BLE_BeaconInit_TypeDef *beacon_init);
extern BLE_Status_TypeDef BSP_BLE_BeaconAdvStart(void);
extern BLE_Status_TypeDef BSP_BLE_BeaconAdvStop(void);
extern BLE_Status_TypeDef BSP_BLE_GetStackStatus(uint8_t *status);
extern BLE_Status_TypeDef BSP_BLE_GetNotifStatus(uint8_t *status);
extern BLE_Status_TypeDef BSP_BLE_GetRXBufferLength(uint8_t *rx_buffer_status);
extern BLE_Status_TypeDef BSP_BLE_GetRXBuffer(uint8_t *rx_buffer, uint8_t *data_len);
extern BLE_Status_TypeDef BSP_BLE_DataThroughputTest(uint8_t enable);
extern BLE_Status_TypeDef BSP_BLE_Disconnect(void);
extern BLE_Status_TypeDef BSP_BLE_SendPacket(uint8_t *data, uint8_t size);
extern BLE_Status_TypeDef BSP_BLE_SendDataByPacket(uint8_t *data, uint16_t size);
extern BLE_Status_TypeDef BSP_BLE_SendBuffer(uint8_t *buffer, uint16_t buffer_size);
extern BLE_Status_TypeDef BSP_BLE_SoftReset(void);
extern void BSP_BLE_INTPinInit(void);
extern void BSP_BLE_GPIO2PinInit(void);
extern void BSP_BLE_RSTNPinInit(void);
extern BLE_Status_TypeDef BSP_BLE_AdvParamInit(BLE_AdvParam_TypeDef *adv_param);
extern BLE_Status_TypeDef BSP_BLE_ConnParamInit(BLE_ConnParam_TypeDef *conn_param);
extern BLE_Status_TypeDef BSP_BLE_Get_FW_params(BLE_FW_Version_TypeDef* BLE_FW);
extern void BSP_BLE_HardReset(void);
extern BLE_Status_TypeDef BSP_BLE_Sleep(void);
extern void BSP_BLE_Wackup(void);
extern void BSP_BLE_GetConfig(BLE_Conf_Typedef *config);
extern BLE_Status_TypeDef BSP_BLE_SetGeneralConfig(void);
extern void BSP_BLE_ErrorHandler(uint8_t error_code, uint32_t line_num, const uint8_t * p_file_name);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_BLE_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
