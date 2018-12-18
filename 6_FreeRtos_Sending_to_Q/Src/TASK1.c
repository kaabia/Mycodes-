
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"


/*OS Thread Declaration*/

osThreadId myTask01Handle;
void Thread1(void const * argument);
extern UART_HandleTypeDef UartHandle;
extern uint8_t aRxBuffer[RXBUFFERSIZE];
//extern osMutexId myMutex01Handle;
 extern QueueHandle_t xQueue1 ;



void CreateTask1()
{
  /* definition and creation of myTask01 */
  osThreadDef(myTask01, Thread1, osPriorityNormal, 0, 128);
  /* Start thread */
  myTask01Handle = osThreadCreate(osThread(myTask01), NULL);
}


/* USER CODE BEGIN Header_Thread1 */
/**
  * @brief  Function implementing the myTask01 thread.
  * @param  argument: Not used 
  * @retval None
  */

/* USER CODE END Header_Thread1 */
void Thread1(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
   for(;;)
  {

    
          
  if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
   //xSemaphoreTake(myMutex01Handle,portMAX_DELAY);

//    while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
//  {
//  }

	
   //xSemaphoreGive(myMutex01Handle);
  
   osDelay(1000);
  }
  /* USER CODE END 5 */ 
}
/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED5 on: Transfer in reception process is correct */
 
    if( xQueueSend( xQueue1, ( void * ) &aRxBuffer, ( TickType_t ) 10 ) != pdPASS )
	{
	      Error_Handler();// Failed to post the message, even after 10 ticks.
	}
}


