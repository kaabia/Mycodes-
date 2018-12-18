/**
******************************************************************************
* $HeadURL$
* $Author$
* $Revision$
* $Date$
******************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "BSP_ADPD103.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADPD103_ADDRESS_W     0xC8
#define ADPD103_ADDRESS_R     0xC9
#define ADPD103_SAMPLE_CLK    0x2692
#define ADPD103_ProgramMode   0x01
#define ADPD103_NormalMode    0x02
#define UPDATE_DATA_REGISTER  0x06
#define HOLD_DATA_REGISTER    0x00
#define ADPD103_SW_RESET      0x01
#define AFE_PWR_CFG1M_MASK    0xFE07
#define NUM_AVGM_MASK         0xF88F
#define PD_LED_SELECTM_MASK   0xF000     
#define ILED_COARSEM_MASK     0xDF80   
#define SLOT_ENM_MASK         0xEE02
#define SLOT_LED_PULSEM_MASK  0xE000
#define CLEAR_FIFO            0x8000
#define IN2_SHIFT              2
#define IN3_SHIFT              3
#define IN4_SHIFT              4
#define IN5_SHIFT              5
#define IN6_SHIFT              6
#define IN8_SHIFT              8
#define IN11_SHIFT             11
#define IN12_SHIFT             12
#define IN13_SHIFT             13

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void BSP_ADPD103_GetConfiguration();

/**
* @brief  Initialize the Oximeter Sensor.
* @param  None
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_Init(void)
{
  BSP_ADPD103_Status status;
  BSP_ADPD103_HandleStruct SPO2;
  
  status = BSP_ADPD103_SetRegValue(SAMPLE_CLK, ADPD103_SAMPLE_CLK);
  status |= BSP_ADPD103_SetProgramMode();
  
  BSP_ADPD103_GetConfiguration(&SPO2);
  status |= BSP_ADPD103_SetGeneralConfiguration(SPO2);
  status |= BSP_ADPD103_SetNormalMode();
  
  return status;
}

/**
* @brief  Set the normal mode.
* @param  None
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_SetNormalMode()
{  
  return (BSP_ADPD103_SetRegValue(ModeR, ADPD103_NormalMode));
}

/**
* @brief  Set the program mode.
* @param  None
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_SetProgramMode()
{
  return (BSP_ADPD103_SetRegValue(ModeR, ADPD103_ProgramMode));  
}

/**
* @brief  Set the register value
* @param  Register
* @param  value
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_SetRegValue(uint8_t Register, uint16_t value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_ADPD103_Status status = ADPD103_OK;
  
  uint8_t WriteData[2];
  
  WriteData[0] = (uint8_t)(value >> 8);
  WriteData[1] = (uint8_t)value;
  
  i2cError = DRV_I2C_WriteBuffer( ADPD103_ADDRESS_W, Register, 1, WriteData, 2);
  
  if (i2cError != I2C_OK) 
  { 
    status = ADPD103_ERROR; 
  }
  
  return status ;
}

/**
* @brief  Set the register MSB value
* @param  Register
* @param  value
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_SetRegMSBValue(uint8_t Register, uint8_t value)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_ADPD103_Status status = ADPD103_OK;
  
  uint8_t WriteData[2];
  
  WriteData[0] = value ;
  WriteData[1] = 0;
  
  i2cError = DRV_I2C_WriteBuffer( ADPD103_ADDRESS_W, Register,  1, WriteData, 2);
  
  if (i2cError != I2C_OK) 
  { 
    status = ADPD103_ERROR; 
  }
  
  return status ;
}

/**
* @brief  Read the register value
* @param  Register
* @param  data
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_ReadReg(uint8_t Register, uint16_t* data)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_ADPD103_Status status = ADPD103_OK;
  uint8_t value[2];
  *data=0;
  
  i2cError = DRV_I2C_ReadBuffer( ADPD103_ADDRESS_R, Register, 1, value, 2);
  *data = ((uint16_t)value[0] << 8) | value[1];
  
  if (i2cError != I2C_OK) 
  { 
    status = ADPD103_ERROR; 
  }
  
  return status ;
}

/**
* @brief  Read the register MSB value
* @param  Register
* @param  data
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_ReadMSBReg(uint8_t Register, uint8_t* data)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_ADPD103_Status status = ADPD103_OK;
  *data=0;
  
  i2cError = DRV_I2C_Read( ADPD103_ADDRESS_R, Register, data);
  
  if (i2cError != I2C_OK) 
  { 
    status = ADPD103_ERROR; 
  }
  
  return status ;
}

/**
* @brief  Read the Buffer value
* @param  Register
* @param  data
* @param  number of bytes
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_ReadBuffer(uint8_t Register, uint8_t* data, uint8_t number)
{
  DRV_I2C_StatusTypeDef i2cError = I2C_OK;  
  BSP_ADPD103_Status status = ADPD103_OK;
  
  i2cError = DRV_I2C_ReadBuffer( ADPD103_ADDRESS_R, Register, 1, data, number);
  
  if (i2cError != I2C_OK) 
  { 
    status = ADPD103_ERROR; 
  }
  
  return status ;
}

/**
* @brief  Read data from registers
* @param  Data to be read 
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_ReadDataRegister (BSP_ADPD103_LEDCH LED[])
{
  uint8_t data[16];
  BSP_ADPD103_Status status;
  
  /*  Write a 1 to SLOTA_DATA_HOLD or SLOTB_DATA_HOLD 
  (Register 0x5F, Bit 1 and Bit 2, respectively) for the
  time slot requiring access (both time slots can be accessed).
  This prevents sample updates.*/
  status = BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, UPDATE_DATA_REGISTER);
  
  /*  Read the registers as desired. */
  BSP_ADPD103_ReadBuffer(SLOTA_PD1_16BIT, data, 16);
  LED[0].CH1 = (data[0] << 8) | data[1];
  LED[0].CH2 = (data[2] << 8) | data[3];
  LED[0].CH3 = (data[4] << 8) | data[5];
  LED[0].CH4 = (data[6] << 8) | data[7];
  LED[1].CH1 = (data[8] << 8) | data[9];
  LED[1].CH2 = (data[10] << 8) | data[11];
  LED[1].CH3 = (data[12] << 8) | data[13];
  LED[1].CH4 = (data[14] << 8) | data[15];
  
  /*  Write a 0 to the SLOTA_DATA_HOLD or SLOTB_DATA_
  HOLD bits (Register 0x5F, Bit 1 and Bit 2, respectively)
  previously set. Sample updates are allowed again. */
  status |= BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, HOLD_DATA_REGISTER);
  
  return status;
}

/**
* @brief  Read data from FIFO
* @param  Data to be read
* @param  Number of bytes
* @retval BSP ADPD103 Status Value.
*/
uint8_t data[16];
uint8_t val=0;
BSP_ADPD103_Status BSP_ADPD103_ReadDataFIFO (BSP_ADPD103_LEDsCH DATA[], uint8_t* number)
{
  uint16_t i=0;
  uint16_t value = 0;
  BSP_ADPD103_Status status;
  
  for(int j=0; j<8; j++)
  {
    DATA[j].led[0].CH1 = 0;
    DATA[j].led[0].CH2 = 0;
    DATA[j].led[0].CH3 = 0;
    DATA[j].led[0].CH4 = 0;
    
    DATA[j].led[1].CH1 = 0;
    DATA[j].led[1].CH2 = 0;
    DATA[j].led[1].CH3 = 0;
    DATA[j].led[1].CH4 = 0;
  }
  /* Read the FIFO_SAMPLES bits (Register 0x00, Bits[15:8]). */
  status = BSP_ADPD103_ReadReg(Status, &value);
  
  val =(uint8_t)(value >> 8);
  *number = val;
  
  /* If FIFO_SAMPLES = the packet size, read a packet using */
  while(val>=16)
  {
    if((val % 16)==0)
    {
      val -= 16;
      /* Write 1 to the DIGITAL_CLOCK_ENA bit (Register 0x5F,
      Bit 0) twice in two consecutive write operations.*/
      status |= BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, 0x1);
      status |= BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, 0x1);
      
      /* Read a complete packet using one or more multiword
      accesses using Register 0x60.  */
      BSP_ADPD103_ReadBuffer(FIFO_ACCESS, data, 16);
      
      DATA[i].led[0].CH1 = (data[0] << 8) | data[1];
      DATA[i].led[0].CH2 = (data[2] << 8) | data[3];
      DATA[i].led[0].CH3 = (data[4] << 8) | data[5];
      DATA[i].led[0].CH4 = (data[6] << 8) | data[7];
      
      DATA[i].led[1].CH1 = (data[8] << 8) | data[9];
      DATA[i].led[1].CH2 = (data[10] << 8)| data[11];
      DATA[i].led[1].CH3 = (data[12] << 8)| data[13];
      DATA[i].led[1].CH4 = (data[14] << 8)| data[15];
      
      i++;
      
      /* Write 0 to the DIGITAL_CLOCK_ENA bit (Register 0x5F,Bit 0). */
      status |= BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, 0x0);
    }
  }
  return status;
}

/**
* @brief  Clear FIFO
* @param  None
* @retval BSP ADPD103 Status Value.
*/
uint16_t StatusvalBSP = 0x0000, DATA_ACCESS_CTLvalBSP = 0x0000;
BSP_ADPD103_Status BSP_ADPD103_ClearFIFO (void)
{
  BSP_ADPD103_Status status;
  
  /* Enter program mode by setting Register 0x10 to 0x1. */
  BSP_ADPD103_SetProgramMode();
  
  /*Write 1 to the DIGITAL_CLOCK_ENA bit (Register 0x5F,
  Bit 0) twice in two consecutive write operations*/
  status = BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, 0x0007);
  
  /* Write 1 to Register 0x00, Bit 15. */
  status |= BSP_ADPD103_SetRegValue(Status, CLEAR_FIFO);
  
  /* Write 0 to the DIGITAL_CLOCK_ENA bit (Register 0x5F,Bit 0). */
  status |= BSP_ADPD103_SetRegValue(DATA_ACCESS_CTL, HOLD_DATA_REGISTER);
  
  status |= BSP_ADPD103_SetNormalMode();
  
  return status;
}

/**
* @brief  Reset 
* @param  None
* @retval BSP ADPD103 Status Value.
*/
BSP_ADPD103_Status BSP_ADPD103_SoftwareReset()
{ 
  return (BSP_ADPD103_SetRegValue(SW_RESETR, ADPD103_SW_RESET));
}

/**
* @brief  Get the default configuration
* @param  BSP_ADPD103_HandleStruct
* @retval None.
*/
void BSP_ADPD103_GetConfiguration(BSP_ADPD103_HandleStruct* SPO2)
{
  /* Configure Time slot A */
  SPO2->SLOTA_LED_SEL  = ConfGet(ADPD103,SLOTALED); 
  SPO2->SLOTA_NUM_AVG  = AveragingFactor_64;
  SPO2->SLOTA_PD_SEL   = ConfGet(ADPD103,SLOTAPD);
  SPO2->SLOTA_AFE_MODE = AFE_NormalMode;
  SPO2->SLOTA_LED_WIDTH      = ConfGet(ADPD103,SLOTAWIDTH);
  SPO2->SLOTA_LED_PERIOD     = ConfGet(ADPD103,SLOTAPERIOD);
  SPO2->SLOTA_LED_OFFSET     = ConfGet(ADPD103,SLOTAOFFSET);
  SPO2->SLOTA_LED_NUMBER     = ConfGet(ADPD103,SLOTANUMBER);
  /* configure Time slot B */
  SPO2->SLOTB_LED_SEL  = ConfGet(ADPD103,SLOTBLED);  
  SPO2->SLOTB_NUM_AVG  = AveragingFactor_64;
  SPO2->SLOTB_PD_SEL   = ConfGet(ADPD103,SLOTBPD);
  SPO2->SLOTB_AFE_MODE = AFE_NormalMode;
  SPO2->SLOTB_LED_WIDTH      = ConfGet(ADPD103,SLOTBWIDTH); 
  SPO2->SLOTB_LED_PERIOD     = ConfGet(ADPD103,SLOTBPERIOD); 
  SPO2->SLOTB_LED_OFFSET     = ConfGet(ADPD103,SLOTBOFFSET); 
  SPO2->SLOTB_LED_NUMBER     = ConfGet(ADPD103,SLOTBNUMBER);
  
  /* Configure the 3 LEDs */
  SPO2->LED1_SCALE           = ConfGet(ADPD103,LED1Scale);
  SPO2->LED2_SCALE           = ConfGet(ADPD103,LED2Scale);
  SPO2->LED3_SCALE           = ConfGet(ADPD103,LED3Scale);
  SPO2->LED1_COARSE          = ConfGet(ADPD103,LED1Coarse);   
  SPO2->LED2_COARSE          = ConfGet(ADPD103,LED2Coarse); 
  SPO2->LED3_COARSE          = ConfGet(ADPD103,LED3Coarse); 
  SPO2->LED1_SLEW            = ConfGet(ADPD103,LED1Slew);
  SPO2->LED2_SLEW            = ConfGet(ADPD103,LED2Slew);
  SPO2->LED3_SLEW            = ConfGet(ADPD103,LED3Slew);
  
  /* Configure the channels and the adc timing */
  SPO2->AFE_POWERDOWN        = ConfGet(ADPD103,ChannelPwrDown);
  SPO2->ADC_Timing           = ConfGet(ADPD103,ADCTiming);
}

/**
* @brief  Set the default configuration
* @param  None
* @retval BSP ADPD103 Status Value.
*/
//uint8_t SLOTA_AFE_WIDTH = 0, SLOTB_AFE_WIDTH = 0;
BSP_ADPD103_Status BSP_ADPD103_SetGeneralConfiguration(BSP_ADPD103_HandleStruct SPO2)
{
  BSP_ADPD103_Status status;
  uint16_t value;
  // uint16_t FSAMPLE100HZ = 0x0050;
  
  // BSP_ADPD103_GetConfiguration(&SPO2);
  
  // 26B5
  
  //  SLOTA_AFE_WIDTH = (uint8_t)APP_SLOTx_AFE_WIDTH; //0x9, // OK
  //  SLOTB_AFE_WIDTH = (uint8_t)APP_SLOTx_AFE_WIDTH;// 0x9, // OK
  
  
  
  status = BSP_ADPD103_SetRegValue(FSAMPLE, 0x0050); // FSAMPLE100HZ
  status |= BSP_ADPD103_SetRegValue(ADC_CLOCK, SPO2.ADC_Timing);
  
  value = ((uint16_t)AFE_PWR_CFG1M & AFE_PWR_CFG1M_MASK)| (SPO2.AFE_POWERDOWN << IN3_SHIFT);
  status |= BSP_ADPD103_SetRegValue(AFE_PWR_CFG1, value);
  
  value = (NUM_AVGM & NUM_AVGM_MASK)| ((SPO2.SLOTB_NUM_AVG << IN8_SHIFT) | (SPO2.SLOTA_NUM_AVG << IN4_SHIFT));
  status |= BSP_ADPD103_SetRegValue(NUM_AVG, ((SPO2.SLOTB_NUM_AVG << IN8_SHIFT) | (SPO2.SLOTA_NUM_AVG << IN4_SHIFT)));
  
  value = (PD_LED_SELECTM & PD_LED_SELECTM_MASK) | (SPO2.SLOTB_PD_SEL << IN8_SHIFT) | (SPO2.SLOTA_PD_SEL << IN4_SHIFT) \
    | (SPO2.SLOTB_LED_SEL << IN2_SHIFT) | SPO2.SLOTA_LED_SEL;
  status |= BSP_ADPD103_SetRegValue(PD_LED_SELECT, value);
  
  value = (ILED_COARSEM & ILED_COARSEM_MASK)| (SPO2.LED1_SCALE << IN13_SHIFT) | (SPO2.LED1_SLEW << IN4_SHIFT) | SPO2.LED1_COARSE;
  status |= BSP_ADPD103_SetRegValue(ILED1_COARSE, value);
  value = (ILED_COARSEM & ILED_COARSEM_MASK)| (SPO2.LED2_SCALE << IN13_SHIFT) | (SPO2.LED2_SLEW << IN4_SHIFT) | SPO2.LED2_COARSE;
  status |= BSP_ADPD103_SetRegValue(ILED2_COARSE, value);  
  
  value = (SLOT_ENM & SLOT_ENM_MASK) | ((FIFO_OVRN_PREVENT << IN12_SHIFT) | (SLOTB_FIFO_MODE << IN6_SHIFT) \
    | (SLOTB_EN << IN5_SHIFT) | (SLOTA_FIFO_MODE << IN2_SHIFT) | SLOTB_EN );
  status |= BSP_ADPD103_SetRegValue(SLOT_EN, value);
  
  status |= BSP_ADPD103_SetRegValue(FIFO_THRESH, (FIFO_THRESHV << IN8_SHIFT));
  
  /**************************** SLOTA ********************************/
  value = ((uint16_t)SLOT_LED_PULSEM & SLOT_LED_PULSEM_MASK) | SPO2.SLOTA_LED_OFFSET | (SPO2.SLOTA_LED_WIDTH << IN8_SHIFT);
  status |= BSP_ADPD103_SetRegValue(SLOTA_LED_PULSE, value);
  
  value = (SPO2.SLOTA_LED_NUMBER << IN8_SHIFT) | SPO2.SLOTA_LED_PERIOD ;
  status |= BSP_ADPD103_SetRegValue(SLOTA_NUMPULSES, value);
  
  value = (SLOTA_AFE_WIDTH << IN11_SHIFT) | (SLOTA_AFE_OFFSET << IN5_SHIFT) | SLOTA_AFE_FOFFSET;
  status |= BSP_ADPD103_SetRegValue(SLOTA_AFE_WINDOW, value);
  
  /**************************** SLOTB ********************************/
  value = ((uint16_t)SLOT_LED_PULSEM & SLOT_LED_PULSEM_MASK) | SPO2.SLOTA_LED_OFFSET | (SPO2.SLOTB_LED_WIDTH << IN8_SHIFT);
  status |= BSP_ADPD103_SetRegValue(SLOTB_LED_PULSE, value);
  
  value = (SPO2.SLOTB_LED_NUMBER << IN8_SHIFT) | SPO2.SLOTB_LED_PERIOD ;
  status |= BSP_ADPD103_SetRegValue(SLOTB_NUMPULSES, value);
  
  value = (SLOTB_AFE_WIDTH << IN11_SHIFT) | (SLOTB_AFE_OFFSET << IN5_SHIFT) | SLOTB_AFE_FOFFSET;
  status |= BSP_ADPD103_SetRegValue(SLOTB_AFE_WINDOW, value);
  
  return status;
}

/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/