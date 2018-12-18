/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BSP_MAX30101.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// SLAVE Address
#define MAX30101_ID_W       0xAE  
#define MAX30101_ID_R       0xAF
#define WAKE_UP_VALUE   0x07
#define SLEEP_VALUE     0x80
#define RESET_VALUE     0x40
#define MODE_MASK       0x07
#define SAMPLE_RATE_MASK 0x1C
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
BSP_MAX30101_HandleStruct BSP_MAX30101_GetDefaultConfig (void);
BSP_MAX30101_InterruptStruct BSP_MAX30101_GetDefaultInterrupt (void);

/*********************** Initialization functions *****************************/
/**
* @brief  Initialisation of the Oximeter Sensor.
* @param  Handle: pointer of the Handle structure to be set
* @param  Interrupt: pointer of interruption structure
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_Init(void)
{
  BSP_MAX30101_HandleStruct Handle;
  BSP_MAX30101_InterruptStruct Interrupt;
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  errorValue |= BSP_MAX30101_Reset();
  
  Handle = BSP_MAX30101_GetDefaultConfig();
  Interrupt = BSP_MAX30101_GetDefaultInterrupt();
  
  Handle.MODE = ConfGet(MAX30101,Mode);
  Handle.ADC_Range_Control = ConfGet(MAX30101,ADCRange);
  Handle.Sample_Rate_Control = ConfGet(MAX30101,SampleRate);
  Handle.LEDx_Current_Control.RED_LED_CurrentControl = ConfGet(MAX30101,LEDRedCurrent);
  Handle.LEDx_Current_Control.IR_LED_CurrentControl = ConfGet(MAX30101,LEDIRCurrent);
  Handle.LEDx_Current_Control.GREEN_LED_CurrentControl = ConfGet(MAX30101,LEDGreenCurrent);
  Handle.LEDx_Current_Control.LED_PROX_CurrentControl = ConfGet(MAX30101,LEDProxCurrent);
  Handle.Pulse_Width_Control = ConfGet(MAX30101,PulseWidth);
  
  errorValue |= BSP_MAX30101_SetConfiguration(&Handle);
  errorValue |= BSP_MAX30101_SetInterrupt(&Interrupt);
  
  errorValue |= BSP_MAX30101_SetRegValue(FIFO_WRITE_PTR, 0x00);
  errorValue |= BSP_MAX30101_SetRegValue(OVER_FLOW_CNT, 0x00);
  errorValue |= BSP_MAX30101_SetRegValue(FIFO_READ_PTR, 0x00);
  
  return errorValue ;
}

/**
* @brief  WakeUp Oximeter Sensor.
* @param  None
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_WakeUp()
{  
  
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t *regValue;
  
  regValue = 0x00;
  // to verify
  errorValue |= BSP_MAX30101_GetRegValue(MODE_CONFIG, regValue);
  regValue[0] &= WAKE_UP_VALUE;
  errorValue |= BSP_MAX30101_SetRegValue(MODE_CONFIG, *regValue);
  
  return errorValue ;
}

/**
* @brief  Enter Oximeter Sensor to sleep mode.
* @param  None
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_Sleep()
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t *regValue;
  
  regValue = 0x00;
  // to verify
  errorValue |= BSP_MAX30101_GetRegValue(MODE_CONFIG, regValue);
  regValue[0] &= WAKE_UP_VALUE;
  regValue[0] |= SLEEP_VALUE;
  errorValue |= BSP_MAX30101_SetRegValue(MODE_CONFIG, *regValue); 
  
  return errorValue ;
}

/**
* @brief  Reset Module .
* @param  None
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_Reset()
{  
  return BSP_MAX30101_SetRegValue(MODE_CONFIG, RESET_VALUE); 
}

/*********************** Setters functions ************************************/
/**
* @brief  Set Configuration Value
* @param  Handle: pointer of the Handle structure to be set
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_SetConfiguration(BSP_MAX30101_HandleStruct *Handle)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t dataToSend;
  
  /* Check the MAX30101 handle allocation */
  if(Handle == NULL)
  {
    return MAX30101_ERROR;
  }
  
  //FIFO Config
  dataToSend = 0;
  dataToSend = Handle->SMP_AVE; 
  dataToSend |= Handle->FIFO_ROLLOVER_EN;
  dataToSend |= Handle->FIFO_A_FULL; 
  
  errorValue |= BSP_MAX30101_SetRegValue(FIFO_CONFIG, dataToSend);
  
  //Mode Config 
  errorValue |= BSP_MAX30101_SetRegValue(MODE_CONFIG, Handle->MODE);
  
  //MAX30101 Config 
  dataToSend = 0;
  dataToSend = Handle->ADC_Range_Control; 
  dataToSend |= Handle->Sample_Rate_Control;
  dataToSend |= Handle->Pulse_Width_Control;
  
  errorValue |= BSP_MAX30101_SetRegValue(SPO2_CONFIG, dataToSend);
  
  //Set Proximity Threshold
  errorValue |= BSP_MAX30101_SetRegValue(INT_PROXIMITY, Handle->PROX_INT_THRESH_Control);
  
  // update param led 
  errorValue |= BSP_MAX30101_UpdateLEDCurrent(&(Handle->LEDx_Current_Control));
  
  // update multiled
  if (Handle->MODE == MultiMode)
  {
    errorValue |= BSP_MAX30101_MultiLedModeControl(&(Handle->SLOTx_Control));
  }
  
  return errorValue ;
}

/**
* @brief  update Configuration Value
* @param  Update: pointer of the Update structure to be set
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_UpdateConfiguration(BSP_MAX30101_UpdateStruct *Update)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t dataToSend;
  
  /* Check the MAX30101 handle allocation */
  if(Update == NULL)
  {
    return MAX30101_ERROR;
  }
  
  //MAX30101 Config 
  errorValue |= BSP_MAX30101_GetRegValue(SPO2_CONFIG, &dataToSend);
  
  dataToSend &= SAMPLE_RATE_MASK; //0b00011100 to save Sample rate value
  dataToSend |= Update->ADC_Range_Control;
  dataToSend |= Update->Pulse_Width_Control;
  
  errorValue |= BSP_MAX30101_SetRegValue(SPO2_CONFIG, dataToSend);
  
  // update param led 
  errorValue |= BSP_MAX30101_UpdateLEDCurrent(&(Update->LEDx_Current_Control));
  
  return errorValue ;
}

/**
* @brief  Set LED current Value
* @param  LEDx_Current: pointer of LED current structure
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_UpdateLEDCurrent(BSP_MAX30101_LEDCurrentStruct *LEDx_Current)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t modeValue;
  
  /* Check the structure allocation */
  if(LEDx_Current == NULL)
  {
    return MAX30101_ERROR;
  }
  
  errorValue |= BSP_MAX30101_GetRegValue(MODE_CONFIG, &modeValue);
  errorValue &= MODE_MASK;
  
  switch (modeValue){
    
  case HR_Mode: 
    errorValue |= BSP_MAX30101_SetRegValue(LED1_CONFIG, LEDx_Current->RED_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED_PROX_CONFIG, LEDx_Current->LED_PROX_CurrentControl);
    break;
    
  case SPO2_Mode:   
    errorValue |= BSP_MAX30101_SetRegValue(LED1_CONFIG, LEDx_Current->RED_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED2_CONFIG, LEDx_Current->IR_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED_PROX_CONFIG, LEDx_Current->LED_PROX_CurrentControl);
    break;
    
  case MultiMode:   
    errorValue |= BSP_MAX30101_SetRegValue(LED1_CONFIG, LEDx_Current->RED_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED2_CONFIG, LEDx_Current->IR_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED3_CONFIG, LEDx_Current->GREEN_LED_CurrentControl);
    errorValue |= BSP_MAX30101_SetRegValue(LED_PROX_CONFIG, LEDx_Current->LED_PROX_CurrentControl);
    break;
    
  default:
    errorValue = MAX30101_ERROR;
    break;
  }
  return errorValue ;
}

/**
* @brief  Set multi LED Value
* @note   this function should be used only in multiLed mode
* @param  SLOTx_Control: pointer of SLOTx Control structure
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_MultiLedModeControl(BSP_MAX30101_SlotControlStruct *SLOTx_Control)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t multiLedModeValue = 0x00;
  
  /* Check the structure allocation */
  if(SLOTx_Control == NULL)
  {
    return MAX30101_ERROR;
  }
  
  multiLedModeValue = (uint8_t)(SLOTx_Control->SLOT1_Control | (SLOTx_Control->SLOT2_Control<<4));
  errorValue |= BSP_MAX30101_SetRegValue(LED_MULTI12_CONFIG, multiLedModeValue);
  
  multiLedModeValue = 0x00;
  multiLedModeValue |= (uint8_t)(SLOTx_Control->SLOT3_Control | (SLOTx_Control->SLOT4_Control<<4));
  multiLedModeValue |= BSP_MAX30101_SetRegValue(LED_MULTI34_CONFIG, multiLedModeValue);
  
  return errorValue ;
}  

/**
* @brief  Set interruption Value
* @param  Interrupt: pointer of interruption structure
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_SetInterrupt(BSP_MAX30101_InterruptStruct *Interrupt)
{
  uint8_t dataToSend;
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  /* Check the MAX30101 interrupt allocation */
  if(Interrupt == NULL)
  {
    return MAX30101_ERROR;
  }
  
  dataToSend = 0;
  if (Interrupt->A_FULL == BIT_SET) 
  {
    dataToSend |= A_FULL_BitControl; 
  }
  if (Interrupt->PPG_DRY == BIT_SET) 
  {
    dataToSend |= PPG_DRY_BitControl; 
  }
  if (Interrupt->ALC_OVF == BIT_SET) 
  {
    dataToSend |= ALC_OVF_BitControl; 
  }
  if (Interrupt->PROX_INT == BIT_SET) 
  {
    dataToSend |= PROX_INT_BitControl; 
  }
  errorValue |= BSP_MAX30101_SetRegValue(INT_ENABLE1, dataToSend);
  
  dataToSend = 0;
  
  if (Interrupt->DIE_TEMP_DRY == BIT_SET) 
  {
    dataToSend = DIE_TEMP_DRY_BitControl; 
  }
  errorValue |= BSP_MAX30101_SetRegValue(INT_ENABLE2, dataToSend);
  
  return errorValue ;
}

/**
* @brief  Set Register Value
* @param  Registre: Address of register to be Set.
* @param  value: Pointer of data register to be Set.
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_SetRegValue(BSP_MAX30101_AddressEnum Registre, uint8_t value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  i2cError = DRV_I2C_Write( MAX30101_ID_W, Registre ,value);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MAX30101_ERROR; 
  }
  
  return errorValue ;
}

/*********************** Getters functions ************************************/
/**
* @brief  Read Register Value
* @param  Registre: Address of register to be read.
* @param  value: Pointer of data register to be read.
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_GetRegValue(BSP_MAX30101_AddressEnum Registre, uint8_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  i2cError = DRV_I2C_Read( MAX30101_ID_R, Registre ,value);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MAX30101_ERROR; 
  }
  
  return errorValue ;
}

BSP_MAX30101_StatusEnum BSP_MAX30101_GetBuffValue(BSP_MAX30101_AddressEnum Registre, uint16_t size, uint8_t *value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  i2cError = DRV_I2C_ReadBuffer(MAX30101_ID_R, Registre, 1, value, size);
  
  if (i2cError != I2C_OK) 
  { 
    errorValue = MAX30101_ERROR; 
  }
  
  return errorValue ;
}
/**
* @brief  Read interruption status
* @param  Interrupt: interruption to be read.
* @param  status: Pointer of data register to be read. it can be BIT_SET or BIT_RESET
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_GetInterruptStatus(BSP_MAX30101_InterruptEnum Interrupt, 
                                                BSP_MAX30101_BitStatusEnum *status)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t regValue;
  
  *status = BIT_RESET;
  
  switch (Interrupt){
  case A_FULL_BitControl: 
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS1 ,&regValue);
    regValue &= A_FULL_BitControl;
    break;
  case PPG_DRY_BitControl:  
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS1 ,&regValue);
    regValue &= PPG_DRY_BitControl;
    break;
  case ALC_OVF_BitControl: 
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS1 ,&regValue);
    regValue &= ALC_OVF_BitControl;
    break;
  case PROX_INT_BitControl: 
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS1 ,&regValue);
    regValue &= PROX_INT_BitControl;
    break;
  case PWR_RDY_BitControl: 
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS1 ,&regValue);
    regValue &= PWR_RDY_BitControl;
    break;
  case DIE_TEMP_DRY_BitControl: 
    errorValue |= BSP_MAX30101_GetRegValue(INT_STATUS2 ,&regValue);
    regValue &= DIE_TEMP_DRY_BitControl;
    break;
  }
  
  if (regValue != 0x00)
  { 
    *status = BIT_SET ; 
  }
  
  return errorValue ;
}

/**
* @brief  Read Temperature Value
* @param  Temp_Integer: Pointer of integer value to be read.
* @param  Temp_Fraction: Pointer of fraction value to be read.
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_ReadTemp(uint8_t *Temp_Integer, uint8_t *Temp_Fraction)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  
  errorValue = BSP_MAX30101_SetRegValue(TEMP_CONFIG , 0x01); // to verify
  
  errorValue |= BSP_MAX30101_GetRegValue(TEMP_INTEGER ,Temp_Integer);
  errorValue |= BSP_MAX30101_GetRegValue(TEMP_FRACTION ,Temp_Fraction);
  
  return errorValue ;
}

/**
* @brief  Read FIFO Value
* @param  LED_Struct: Pointer of LED_Struct value to be read.
* @retval BSP MAX30101 Error Value.
*/
BSP_MAX30101_StatusEnum BSP_MAX30101_ReadLEDValue(uint8_t *nSamples, BSP_MAX30101_LEDStruct * LED)
{
  BSP_MAX30101_StatusEnum errorValue = MAX30101_OK;
  uint8_t fifoValue[300];
  uint8_t fifoRegisters[3] = {0,0,0};
  uint8_t readPtr, writePtr, OvfCounter ;
  uint16_t i, fifoSize; 
  BSP_MAX30101_PulseWidthEnum PulseWidthValue;
  BSP_MAX30101_ModeEnum modeValue;
  
  for (i=0; i<sizeof(fifoValue); i++) fifoValue[i] = 0;
  
  modeValue = ConfGet(MAX30101,Mode);
  PulseWidthValue = ConfGet(MAX30101,PulseWidth);
  
  errorValue |= BSP_MAX30101_GetBuffValue(FIFO_WRITE_PTR, 3, fifoRegisters);
  
  writePtr = fifoRegisters[0];
  OvfCounter = fifoRegisters[1]&0x1F;
  readPtr = fifoRegisters[2];
  
  if (OvfCounter>0) 
  {
    *nSamples = OvfCounter;
  }
  else 
  {
    *nSamples = abs(writePtr - readPtr);
  }
  
  if (*nSamples>=1 )
  {
    switch (modeValue)
    {
    case HR_Mode    : // Only Red LED
      fifoSize = *nSamples *3 ;
      errorValue |= BSP_MAX30101_GetBuffValue(FIFO_DATA_REG, fifoSize, fifoValue);
      
      for (i=0; i<*nSamples; i++)
      {
        LED[i].Red_LED = ((uint32_t)fifoValue[3*i+0] << 16) | ((uint32_t)fifoValue[3*i+1] << 8) | fifoValue[3*i+2] ;
        
        LED[i].Red_LED =  LED[i].Red_LED & 0x3FFFF;
      }
      break ;
    case SPO2_Mode  : // Red LED + IR LED
      fifoSize = *nSamples *6 ;
      errorValue |= BSP_MAX30101_GetBuffValue(FIFO_DATA_REG, fifoSize, fifoValue);
      
      for (i=0; i<*nSamples; i++)
      {
        LED[i].Red_LED  = ((uint32_t)fifoValue[6*i+0] << 16) | ((uint32_t)fifoValue[6*i+1] << 8) | fifoValue[6*i+2] ;
        LED[i].IR_LED   = ((uint32_t)fifoValue[6*i+3] << 16) | ((uint32_t)fifoValue[6*i+4] << 8) | fifoValue[6*i+5] ;
        
        LED[i].Red_LED  =  LED[i].Red_LED & 0x3FFFF;
        LED[i].IR_LED   =  LED[i].IR_LED & 0x3FFFF;
      }
      break;
    case MultiMode  : // Red + IR + Green LED 
      fifoSize = *nSamples *9 ;
      errorValue |= BSP_MAX30101_GetBuffValue(FIFO_DATA_REG, fifoSize, fifoValue);
      for (i=0; i<*nSamples; i++)
      {
        LED[i].Red_LED       = ((uint32_t)fifoValue[9*i+0] << 16) | ((uint32_t)fifoValue[9*i+1] << 8) | fifoValue[9*i+2] ;
        LED[i].IR_LED        = ((uint32_t)fifoValue[9*i+3] << 16) | ((uint32_t)fifoValue[9*i+4] << 8) | fifoValue[9*i+5] ;
        LED[i].Green_LED     = ((uint32_t)fifoValue[9*i+6] << 16) | ((uint32_t)fifoValue[9*i+7] << 8) | fifoValue[9*i+8] ;
        
        LED[i].Red_LED       =  LED[i].Red_LED & 0x3FFFF;
        LED[i].IR_LED        =  LED[i].IR_LED & 0x3FFFF;
        LED[i].Green_LED     =  LED[i].Green_LED & 0x3FFFF;
      }
      break;
    }
    for (i=0; i<*nSamples; i++)
    {
      switch (PulseWidthValue)
      {
      case PW_69: // 15-bit ADC resolution
        LED[i].Red_LED       =  LED[i].Red_LED >> 3;
        LED[i].IR_LED        =  LED[i].IR_LED >> 3;
        LED[i].Green_LED     =  LED[i].Green_LED >> 3;
        break ; 
      case PW_118: // 16-bit ADC resolution
        LED[i].Red_LED       =  LED[i].Red_LED >> 2;
        LED[i].IR_LED        =  LED[i].IR_LED >> 2;
        LED[i].Green_LED     =  LED[i].Green_LED >> 2;
        break ;
      case PW_215: // 17-bit ADC resolution
        LED[i].Red_LED       =  LED[i].Red_LED >> 1;
        LED[i].IR_LED        =  LED[i].IR_LED >> 1;
        LED[i].Green_LED     =  LED[i].Green_LED >> 1;
        break ;
      case PW_411: // 18-bit ADC resolution
        break ;
      default : 
        break;
      }
    }
  }
   
  return errorValue ;
}

BSP_MAX30101_HandleStruct BSP_MAX30101_GetDefaultConfig (void)
{
  BSP_MAX30101_HandleStruct defaultConfig;
  
  //Mode Config
  defaultConfig.MODE = MultiMode ; 
  //FIFO Config
  defaultConfig.SMP_AVE = SA_1;
  defaultConfig.FIFO_ROLLOVER_EN = FIFO_ROLLOVER_EN; 
  defaultConfig.FIFO_A_FULL = A_FULL_1; 
  //MAX30101 Config
  defaultConfig.ADC_Range_Control = FS_8192;
  defaultConfig.Sample_Rate_Control = SR_100;
  defaultConfig.Pulse_Width_Control = PW_411;
  // LED Config
  defaultConfig.LEDx_Current_Control.RED_LED_CurrentControl = 0x3F;
  defaultConfig.LEDx_Current_Control.IR_LED_CurrentControl = 0x3F;
  defaultConfig.LEDx_Current_Control.GREEN_LED_CurrentControl = 0x3F;
  defaultConfig.LEDx_Current_Control.LED_PROX_CurrentControl = 0x02;
  defaultConfig.PROX_INT_THRESH_Control = 0x55;
  defaultConfig.SLOTx_Control.SLOT1_Control = RED_LED_PA;
  defaultConfig.SLOTx_Control.SLOT2_Control = IR_LED_PA;
  defaultConfig.SLOTx_Control.SLOT3_Control = GREEN_LED_PA;
  defaultConfig.SLOTx_Control.SLOT4_Control = DISABLED;
  
  return defaultConfig;
}

BSP_MAX30101_InterruptStruct BSP_MAX30101_GetDefaultInterrupt (void)
{
  BSP_MAX30101_InterruptStruct defaultInterrupt;
  
  defaultInterrupt.A_FULL = BIT_SET ; 
  defaultInterrupt.PPG_DRY = BIT_SET;
  defaultInterrupt.ALC_OVF = BIT_RESET ; 
  defaultInterrupt.PROX_INT = BIT_RESET ; 
  defaultInterrupt.DIE_TEMP_DRY = BIT_RESET ; 
  
  return defaultInterrupt;
}
/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/