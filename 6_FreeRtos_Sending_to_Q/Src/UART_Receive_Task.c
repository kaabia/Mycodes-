/**
******************************************************************************
* @file           : UART_Receive_Task.c
* @brief          : Main program body
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"
#include "UART_Receive_Task.h"
#include "string.h"
#include "stdio.h"

/*OS Thread Declaration*/
extern osThreadId Receive_Task_Handle;

/*Externals*/
extern UART_HandleTypeDef UartHandle;
extern SemaphoreHandle_t xSemaphoreRX;
extern SemaphoreHandle_t xSemaphoreTX;
extern QueueHandle_t xQueue1 ;
extern UBaseType_t  spaceAvailable ;
/*aRxBuffer*/
uint8_t aRxBuffer[RXBUFFERSIZE] ;
uint8_t Buffer[RXBUFFERSIZE];

/*setting xHigherPriorityTaskWoken2 */
BaseType_t xHigherPriorityTaskWoken2 = pdFALSE;
BaseType_t pHigherPriorityTaskWoken2 = pdFALSE;;

BaseType_t xHigherPriorityTaskWoken = pdFALSE;

void Thread1(void const * argument);

void UART_Receive_Task()
{
  /* definition and creation of myTask01 */
  osThreadDef(myTask01, Thread1, osPriorityNormal, 0, 128);
  /* Start thread */
  Receive_Task_Handle = osThreadCreate(osThread(myTask01), NULL);
  /* Suspend Uart_Transmit_Task */
  vTaskSuspend(Receive_Task_Handle);
}

/* USER CODE END Header_Thread1 */
void Thread1(void const * argument)
{         
  
  /* Infinite loop */
  for(;;)
  {
    if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
      Error_Handler();
    }
    xSemaphoreTake(xSemaphoreRX,portMAX_DELAY);
    
    if ( spaceAvailable > 0) /* ques is not full */
    {  
      char buff[10] = "vide\r\n" ;
      
      if( xQueueSend( xQueue1, ( void * ) &aRxBuffer, ( TickType_t ) portMAX_DELAY ) != pdPASS )
      {
        Error_Handler();// Failed to post the message
      }    
      spaceAvailable = uxQueueSpacesAvailable(  xQueue1 );
            if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)buff, 6) != HAL_OK)
      {     
        Error_Handler();
      }
      xSemaphoreTake(xSemaphoreTX,portMAX_DELAY);
      
    } 
    
    
    if ( spaceAvailable == 0) 
    {
      char buffr[10] = "full\r\n" ; 
      
      if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)buffr, 6) != HAL_OK)
      {     
        Error_Handler();
      }
      xSemaphoreTake(xSemaphoreTX,portMAX_DELAY);
 
    } 
    
  }
  
  
  
}




/* USER CODE END 5 */ 

/**
* @brief  Rx Transfer completed callback
* @param  huart: UART handle
* @note   This example shows a simple way to report end of DMA Rx transfer, and
*         you can add your own implementation.
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE because it
  will get set to pdTRUE inside the interrupt-safe API function if a context switch is
  required. */
  xHigherPriorityTaskWoken2 = pdFALSE;
  /* Give the semaphore to unblock the task, passing in the address of
  xHigherPriorityTaskWoken as the interrupt-safe API function's pxHigherPriorityTaskWoken
  parameter. */
  
  xSemaphoreGiveFromISR(xSemaphoreRX,&xHigherPriorityTaskWoken2);
  
  /* Pass the xHigherPriorityTaskWoken value into portYIELD_FROM_ISR(). If
  xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR(), then calling
  portYIELD_FROM_ISR() will request a context switch. If xHigherPriorityTaskWoken is still
  pdFALSE, then calling portYIELD_FROM_ISR() will have no effect. Unlike most FreeRTOS
  ports, the Windows port requires the ISR to return a value. The return statement is inside
  the Windows version of portYIELD_FROM_ISR(). */
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken2 );
  
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  
  xHigherPriorityTaskWoken = pdFALSE;
  /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE because it
will get set to pdTRUE inside the interrupt-safe API function if a context switch is
required. */

/* Give the semaphore to unblock the task, passing in the address of
xHigherPriorityTaskWoken as the interrupt-safe API function's pxHigherPriorityTaskWoken
parameter. */
xSemaphoreGiveFromISR(xSemaphoreTX,&xHigherPriorityTaskWoken);

/* Pass the xHigherPriorityTaskWoken value into portYIELD_FROM_ISR(). If
xHigherPriorityTaskWoken was set to pdTRUE inside xSemaphoreGiveFromISR(), then calling
portYIELD_FROM_ISR() will request a context switch. If xHigherPriorityTaskWoken is still
pdFALSE, then calling portYIELD_FROM_ISR() will have no effect. Unlike most FreeRTOS
ports, the Windows port requires the ISR to return a value. The return statement is inside
the Windows version of portYIELD_FROM_ISR(). */
portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}


