
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "stm32l476g_discovery.h"

/* Private variables ---------------------------------------------------------*/
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
void Thread3(void const * argument);

void CreateTask3(){
  
  /* definition and creation of myTask02 */
  osThreadDef(myTask03, Thread3, osPriorityNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL); 
}


/* USER CODE BEGIN Header_Thread3 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/

/* USER CODE END Header_Thread3 */
void Thread3(void const * argument)
{
  /* USER CODE BEGIN Thread3 */
  /* Infinite loop */
  while(1)
  {
    BSP_LED_Off(LED5);
    BSP_LED_Off(LED4);
    osDelay(200);
    
  }
  /* USER CODE END Thread3 */
}