/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"
#include "TASK2.h"

/* Private variables ---------------------------------------------------------*/
osThreadId myTask02Handle;


/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef UartHandle;
//extern osMutexId myMutex01Handle;
 extern QueueHandle_t xQueue1 ;
 uint8_t Buffer[RXBUFFERSIZE];


/* Private function prototypes -----------------------------------------------*/
void Thread2(void const * argument);

void  CreateTask2(){
  /* definition and creation of myTask02 */

  osThreadDef(myTask02, Thread2, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL); 
}

/* USER CODE BEGIN Header_Thread2 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/

/* USER CODE END Header_Thread2 */
void Thread2(void const * argument)
{
  /* USER CODE BEGIN Thread2 */
  vTaskSuspend(myTask02Handle);
  
    if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)Buffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Infinite loop */
  for(;;)
  {
   //xSemaphoreTake(myMutex01Handle,portMAX_DELAY);    

 //   while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
  //{
    
 // }
   //xSemaphoreGive(myMutex01Handle);
  
   osDelay(1000);
  }
  /* USER CODE END Thread2 */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED4 on: Transfer in transmission process is correct */
  
    if( xQueueReceive( xQueue1,( void * )&Buffer, ( TickType_t ) 10 ) != pdPASS )
	{
	  // Failed to post the message, even after 10 ticks.
	}
}
