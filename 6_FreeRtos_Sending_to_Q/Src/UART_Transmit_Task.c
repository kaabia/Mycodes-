/**
  ******************************************************************************
  * @file           : UART_Transmit_Task.c
  * @brief          : Main program body
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"
#include "UART_Transmit_Task.h"

/* Externals */
  extern UART_HandleTypeDef UartHandle;
  extern osThreadId Transmit_Task_Handle;
  extern SemaphoreHandle_t xSemaphoreTX;
  extern QueueHandle_t xQueue1 ;
  
/* Private variables ---------------------------------------------------------*/
  uint8_t Buffer[RXBUFFERSIZE];
  uint8_t aTxBuffer[RXBUFFERSIZE] ;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE; ;
  
/* Private function prototypes -----------------------------------------------*/
  void Thread2(void const * argument);
  
  void  UART_Transmit_Task(){
    
  /* definition and creation of myTask02 */

   osThreadDef(myTask02, Thread2, osPriorityNormal, 0, 128);
   Transmit_Task_Handle = osThreadCreate(osThread(myTask02), NULL); 
   /* Suspend Uart_Transmit */
   vTaskSuspend(Transmit_Task_Handle);

}

/* USER CODE END Header_Thread2 */
void Thread2(void const * argument)
{
  
   /* Infinite loop */
  for(;;)
  {  
        
   if( xQueueReceive( xQueue1,( void * )&Buffer, ( TickType_t ) portMAX_DELAY ) != pdPASS )
	{
    Error_Handler();

	}
   
   if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)Buffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
 
    xSemaphoreTake(xSemaphoreTX,portMAX_DELAY); 
     
  }
  
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE because it
will get set to pdTRUE inside the interrupt-safe API function if a context switch is
required. */
xHigherPriorityTaskWoken = pdFALSE;

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
