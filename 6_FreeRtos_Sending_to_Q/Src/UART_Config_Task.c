/**
  ******************************************************************************
  * @file           : UART_Config_Task.c
  * @brief          : Main program body
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"
#include "UART_Config_Task.h"
#include "UART_Receive_Task.h"

 /* Private variables --------------------------------------------------------*/
 
/* external handlers */
 extern UART_HandleTypeDef UartHandle; 
 extern osThreadId Receive_Task_Handle;
 
 /* config uart _Task_Handle */
 osThreadId Uart_config_TaskHandle;
 
 /* A Message shows at the beginning to verify UART configuration */
 uint8_t mess[10] = "start \r\n" ;

/* Private function prototypes -----------------------------------------------*/
 void Uart_Config_Thread(void const * argument);
  
 /*------------ Task Creation  -----------------------------------------------*/
void Create_Uart_ConfigTask()
{
  /* definition and creation of myTask01 */
  osThreadDef(Config_Task, Uart_Config_Thread, osPriorityNormal, 0, 128);
  /* Start thread */
  Uart_config_TaskHandle = osThreadCreate(osThread(Config_Task), NULL);
}

 /*------------ Uart_Config_Thread  ------------------------------------------*/

void Uart_Config_Thread(void const * argument)
{
 
    /*----------------1- Configure the UART peripheral -----------------------*/
  
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART configured as follows:
        - Word Length = 8 Bits
        - Stop Bit    = One Stop bit
        - Parity      = ODD parity
        - BaudRate    = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
  
    UartHandle.Instance        = USARTx;
    UartHandle.Init.BaudRate   = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;

    if (HAL_UART_Init(&UartHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

/* message de test "start" */ 
/* Uncomment these next 7 lines to test Uart functionnality */ 
    
//     if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)mess, RXBUFFERSIZE) != HAL_OK)
// {
//    Error_Handler();
//  } 
//  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
//  {
//  }
//  
  
    /* Resume UART_Receive_Task */
    vTaskResume(Receive_Task_Handle);
    /* Delete UART_Config_Task */
    vTaskDelete(Uart_config_TaskHandle);

}

