/**
******************************************************************************
* $HeadURL: $
* $Author:  $
* $Revision:  $
* $Date: $
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_RTC.h"
#include <time.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
  RTC_HandleTypeDef RtcHandle; 
  RTC_DateTypeDef  sdate;
  RTC_TimeTypeDef  stime;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the RTC
  * @param  None
  * @retval None
  */
BSP_RTC_Status BSP_RTC_Init(void)
{
  BSP_RTC_Status Status= RTC_OK;
 /**** Configure the RTC peripheral ******************/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
      - Hour Format    = Format 12
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
  RtcHandle.Instance = RTC;

  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24; //RTC_HOURFORMAT_24
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  if(HAL_RTC_Init(&RtcHandle)!= HAL_OK)
  {
    Status = RTC_ERROR;
  }
  __HAL_RTC_WRITEPROTECTION_DISABLE(&RtcHandle);
  RtcHandle.Instance->CR |= RTC_CR_BYPSHAD;
  /* Enable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(&RtcHandle);
  
  /* Do the calibration with a value of CALM = 128 clock cycles */
  HAL_RTCEx_SetSmoothCalib(&RtcHandle, RTC_SMOOTHCALIB_PERIOD_32SEC, RTC_SMOOTHCALIB_PLUSPULSES_RESET, 0x00000080);
  
  /* Select RTC_CALIBOUTPUT_1HZ for output calibration */
  HAL_RTCEx_SetCalibrationOutPut(&RtcHandle, RTC_CALIBOUTPUT_1HZ);

  /* Enable the output calibration */
  RtcHandle.Instance->CR |= 0x00800000;
  /* Disable the output for ALARM */
  RtcHandle.Instance->CR &= 0xFF9FFFFF;
  /* Remap the RTC_OUT signal on PB2 pin */
  RtcHandle.Instance->OR = 0x00000002;
 
/*  
  // Configure the LSE clock to be outputted on MCO1 pin 
  HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_LSE,RCC_MCODIV_1);
  
  // Configure the PB2 GPIO in order to output the signal  
  CONF_BLE_PIN_GPIO_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = CONF_BLE_PIN_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(CONF_BLE_PIN_GPIO_GPIO, &GPIO_InitStruct);
*/
  
  return Status;
}

/**
  * @brief  Set the time and date.
  * @param  date 
  * @retval None
  */
BSP_RTC_Status BSP_RTC_SetTimeStamp(RTC_Date date)
{
  BSP_RTC_Status Status= RTC_OK;
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /***** Configure the Date ************************************************/
 
  sdatestructure.Year    = (uint8_t)(date.year -2000);
  sdatestructure.Month   = date.month;
  sdatestructure.Date    = date.day;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN) != HAL_OK)
  {
    Status = RTC_ERROR; 
  }
  /***** Configure the Time**************************************************/
  stimestructure.Hours          = date.hour;
  stimestructure.Minutes        = date.min;
  stimestructure.Seconds        = date.sec;
  stimestructure.SubSeconds     = 0x00;
  stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BIN) != HAL_OK)
  {
    Status = RTC_ERROR;  
  }
  
  return Status;
}

/**
  * @brief  Get the time and date.
  * @param  date to be read
  * @retval None
  */
BSP_RTC_Status BSP_RTC_GetTimeStamp(RTC_Date *date)
{
  BSP_RTC_Status Status= RTC_OK;
  
    /* Note : This configuration makes the RTC increments correctly but still not tested for a long
  period of time */
  
  if(HAL_RTC_GetDate(&RtcHandle, &sdate, RTC_FORMAT_BIN)!= HAL_OK) //RTC_FORMAT_BIN RTC_FORMAT_BCD
    Status = RTC_ERROR;
  if(HAL_RTC_GetTime(&RtcHandle, &stime, RTC_FORMAT_BIN)!= HAL_OK)
    Status = RTC_ERROR;
  
  date->day=sdate.Date;
  date->month=sdate.Month;
  date->year=sdate.Year+2000;
  date->hour=stime.Hours;
  date->min=stime.Minutes;
  date->sec=stime.Seconds;
  date->ssec = 999 - ((uint16_t) ((((float) (stime.SubSeconds)) * 1000.0f) / ((float) (stime.SecondFraction+1))));
  
  return Status;  
}


BSP_RTC_Status BSP_RTC_GetPosixMs(uint64_t *DatePosix)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  RTC_Date date;
  
  Status = BSP_RTC_GetTimeStamp(&date);
  
  tm2.tm_hour = date.hour;
  tm2.tm_min = date.min;
  tm2.tm_sec = date.sec;
  tm2.tm_mday = date.day;
  tm2.tm_mon = date.month-1;
  tm2.tm_year= date.year-1900; 
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DatePosix=mktime(&tm2);
  *DatePosix = (*DatePosix) * 1000 + 999 - ((uint16_t) ((((float) (stime.SubSeconds)) * 1000.0f) / ((float) (stime.SecondFraction+1))));
  return Status; 
}

BSP_RTC_Status BSP_RTC_GetPosixus(uint64_t *DatePosix)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  RTC_Date date;
  
  Status = BSP_RTC_GetTimeStamp(&date);
  
  tm2.tm_hour = date.hour;
  tm2.tm_min = date.min;
  tm2.tm_sec = date.sec;
  tm2.tm_mday = date.day;
  tm2.tm_mon = date.month-1;
  tm2.tm_year= date.year-1900; 
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DatePosix=mktime(&tm2);
  *DatePosix = (*DatePosix) * 1000 + 999 - ((uint16_t) ((((float) (stime.SubSeconds)) * 1000.0f) / ((float) (stime.SecondFraction+1))));
  *DatePosix *= 1000;
  return Status; 
}

BSP_RTC_Status BSP_RTC_GetCocoaMs(uint64_t *DateCocoa)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  RTC_Date date;
  
  Status = BSP_RTC_GetTimeStamp(&date);
  
  tm2.tm_hour = date.hour;
  tm2.tm_min = date.min;
  tm2.tm_sec = date.sec;
  tm2.tm_mday = date.day;
  tm2.tm_mon = date.month-1;
  tm2.tm_year= date.year-1900; 
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DateCocoa=mktime(&tm2);
  *DateCocoa = (*DateCocoa-978307200) * 1000 + 999 - ((uint16_t) ((((float) (stime.SubSeconds)) * 1000.0f) / ((float) (stime.SecondFraction+1))));
  return Status; 
}

BSP_RTC_Status BSP_RTC_GetCocoaus(uint64_t *DateCocoa)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  RTC_Date date;
  
  Status = BSP_RTC_GetTimeStamp(&date);
  
  tm2.tm_hour = date.hour;
  tm2.tm_min = date.min;
  tm2.tm_sec = date.sec;
  tm2.tm_mday = date.day;
  tm2.tm_mon = date.month-1;
  tm2.tm_year= date.year-1900; 
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DateCocoa=mktime(&tm2);
  *DateCocoa = (*DateCocoa-978307200) * 1000 + 999 - ((uint16_t) ((((float) (stime.SubSeconds)) * 1000.0f) / ((float) (stime.SecondFraction+1))));
  
  *DateCocoa *= 1000;
    
  return Status; 
}


/**
  * @brief  Get the time and date on Posix Format.
  * @param  Value to be read
  * @retval None
  */
BSP_RTC_Status BSP_RTC_GetPosix(uint64_t *DatePosix)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  if(HAL_RTC_GetDate(&RtcHandle, &sdate, RTC_FORMAT_BIN)!= HAL_OK)
    Status = RTC_ERROR;
  if(HAL_RTC_GetTime(&RtcHandle, &stime, RTC_FORMAT_BIN)!= HAL_OK)
    Status = RTC_ERROR;
  
  tm2.tm_hour = stime.Hours;
  tm2.tm_min = stime.Minutes;
  tm2.tm_sec = stime.Seconds;
  tm2.tm_mday = sdate.Date;
  tm2.tm_mon = sdate.Month-1;
  tm2.tm_year= sdate.Year+100;
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DatePosix=mktime(&tm2);
  
  return Status; 
}

BSP_RTC_Status BSP_RTC_GetCocoa(uint64_t *DateCocoa)
{
  BSP_RTC_Status Status= RTC_OK;
  struct tm tm2;
  
  if(HAL_RTC_GetDate(&RtcHandle, &sdate, RTC_FORMAT_BIN)!= HAL_OK)
    Status = RTC_ERROR;
  if(HAL_RTC_GetTime(&RtcHandle, &stime, RTC_FORMAT_BIN)!= HAL_OK)
    Status = RTC_ERROR;
  
  tm2.tm_hour = stime.Hours;
  tm2.tm_min = stime.Minutes;
  tm2.tm_sec = stime.Seconds;
  tm2.tm_mday = sdate.Date;
  tm2.tm_mon = sdate.Month-1;
  tm2.tm_year= sdate.Year+100;
  tm2.tm_yday= 0;
  tm2.tm_isdst=-1;
  tm2.__BSD_bug_filler1=0;
  tm2.__BSD_bug_filler2=0;
  
  *DateCocoa=mktime(&tm2);
  *DateCocoa -= 978307200;
  
  return Status; 
}
/**
  * @brief  Set the time and date on the Posix Format.
  * @param  None
  * @retval None
  */
BSP_RTC_Status BSP_RTC_SetPosix( uint64_t date)
{
  
  /* Note : This configuration makes the RTC increments correctly but still not tested for a long
  period of time */
  BSP_RTC_Status Status= RTC_OK;
  time_t t ;
  struct tm* tm1;
  
  t = (time_t) date;
  tm1 = localtime(&t);
  
  sdate.Year    = (uint8_t) tm1->tm_year -100; // 0x16
  sdate.Month   = (uint8_t) tm1->tm_mon + 1; //0x12
  sdate.Date    = (uint8_t) (tm1->tm_mday); //0x31
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdate,RTC_FORMAT_BIN) != HAL_OK) //RTC_FORMAT_BCD RTC_FORMAT_BIN
  {
    Status = RTC_ERROR; 
  } 

  stime.Hours          = (uint8_t) tm1->tm_hour; //0x23
  stime.Minutes        = (uint8_t) tm1->tm_min; //0x59
  stime.Seconds        = (uint8_t) tm1->tm_sec; //0x59
  stime.TimeFormat     = RTC_HOURFORMAT_24; //RTC_HOURFORMAT_24
  stime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stime.StoreOperation = RTC_STOREOPERATION_SET; 
  
  if(HAL_RTC_SetTime(&RtcHandle,&stime,RTC_FORMAT_BIN) != HAL_OK) //RTC_FORMAT_BCD RTC_FORMAT_BIN
  {
    Status = RTC_ERROR; 
  } 
  
  return Status;
}


BSP_RTC_Status BSP_RTC_SetPosixus(uint64_t value)
{
  uint64_t svalue; 
  BSP_RTC_Status Status= RTC_OK;
  
  svalue = (uint64_t)(value/1000000);
  Status = BSP_RTC_SetPosix(svalue);
  
  return Status ; 
}

/**
  * @brief  Set the Current Time.
  * @param  None
  * @retval None
  */
BSP_RTC_Status BSP_RTC_SetCurrentTime()
{
  BSP_RTC_Status Status= RTC_OK;
  time_t t ;
  struct tm* tmm;
  RTC_DateTypeDef  d1;
  RTC_TimeTypeDef  t1;
  
  time(&t);
  tmm = localtime(&t);
  
  d1.Year    = (uint8_t) (tmm->tm_year -100);
  d1.Month   = (uint8_t) tmm->tm_mon + 1;
  d1.Date    = (uint8_t) (tmm->tm_mday);
  
  if(HAL_RTC_SetDate(&RtcHandle,&d1,RTC_FORMAT_BIN) != HAL_OK)
  {
    Status = RTC_ERROR; 
  } 

  t1.Hours          = (uint8_t) tmm->tm_hour+1;
  t1.Minutes        = (uint8_t) tmm->tm_min;
  t1.Seconds        = (uint8_t) tmm->tm_sec;
  t1.TimeFormat     = RTC_HOURFORMAT12_AM;
  t1.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  t1.StoreOperation = RTC_STOREOPERATION_SET; 
  
  if(HAL_RTC_SetTime(&RtcHandle,&t1,RTC_FORMAT_BIN) != HAL_OK)
  {
    Status = RTC_ERROR; 
  } 
  
  return Status;
}
/**
  * @brief  MSP Init
  * @param  RTC_HandleTypeDef
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /******* Enables the PWR Clock and Enables access to the backup domain ****/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTC clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /******** Enable RTC peripheral Clocks ************************************/
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
  
  /****** Configure the NVIC for RTC TimeStamp ******************************/
  HAL_NVIC_SetPriority(TAMP_STAMP_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(TAMP_STAMP_IRQn);
}

/************************ (C) COPYRIGHT OPCMA *****END OF FILE****/