/**
******************************************************************************
* $HeadURL: $
* $Author:  $
* $Revision:  $
* $Date: $
******************************************************************************
*/

  /* Includes ------------------------------------------------------------------*/
#include "BSP_EEPROM.h"

unsigned char ROM_NO[SERIAL_NUMBER_SIZE+2];
unsigned char ROM_List[MAX_SLAVE_CONNECTED][SERIAL_NUMBER_SIZE+2] ; 
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
unsigned char crc8;

extern DS2431_Status EEPROM_CRC8 (ReadRomCmd RxRomCmd);
extern DS2431_Status EEPROM_CRC16 ( uint8_t tab[], uint8_t size, uint16_t CRC16);

/**
  * @brief  Read ROM
  * @param  ReadRomCmd
  * @retval DS2431_Status
  */
DS2431_Status BSP_EEPROM_ReadRom(ReadRomCmd *RxRomCmd)
{
  uint8_t bit = 0;
  int i = 0 ;
  DS2431_Status status = DS2431_ERROR;
  
  /* Send Read Rom CMD */
  DRV_1Wire_WriteByte(READ_ROM_CMD);
  
  /* Read the Family Code */
  DRV_1Wire_ReadByte(&bit);
  RxRomCmd->FamilyCode=bit;
  
  /* Chek if Eqaul S2431 Family Code */
  if (RxRomCmd->FamilyCode == DS2431_FAMILY_CODE )
  {
    status = DS2431_OK;
  }
  else 
  {
    status = DS2431_ERROR;
  }
  
  /* Read the serial Number */
  for (i = 0; i<SERIAL_NUMBER_SIZE ; i++)
  {
    bit =0;
    DRV_1Wire_ReadByte(&bit);
    RxRomCmd->SerialNumber[i]= bit;
  }
  
  /* Read CRC Byte */
  bit =0;
  DRV_1Wire_ReadByte(&bit);
  RxRomCmd->CRCDS2431= bit;
  
  /* Check the received CRC */
  status |= EEPROM_CRC8 (*RxRomCmd);
  
  return status;
}

/**
  * @brief  Skip ROM
  * @param  None
  * @retval None
  */
void BSP_EEPROM_SkipRom(void)
{
  DRV_1Wire_WriteByte(SKIP_ROM_CMD);
}

/**
  * @brief  Match ROM
  * @param  RxRomCmd
  * @retval None
  */
void BSP_EEPROM_MatchRom(ReadRomCmd RxRomCmd)
{
  int i = 0 ;
  
  /* Send Match Rom CMD */
  DRV_1Wire_WriteByte(MATCH_ROM_CMD);
  
  /* Send the family Code */
  DRV_1Wire_WriteByte(RxRomCmd.FamilyCode);
  
  /* Send the serial Number */
  for (i = 0; i<SERIAL_NUMBER_SIZE ; i++)
  {
    DRV_1Wire_WriteByte(RxRomCmd.SerialNumber[i]);
  }
  
  /* Send the CRC8 */
  DRV_1Wire_WriteByte(RxRomCmd.CRCDS2431);
}          

/**
  * @brief  Write Scratchpad
  * @param  Values to write
  * @retval CRC_16bits
  */
DS2431_Status BSP_EEPROM_WriteScratchpad(WriteScratchpadCmd Scratch, uint8_t CRCDS[])
{  
  uint8_t byte=0;
  uint8_t carctab[WRITE_SCRATCHPAD_DATA_SIZE + 3 ] = {0}; 
  uint16_t crcValue = 0 ;
  DS2431_Status status = DS2431_ERROR;
  int i = 0 ;
  
  /* Send the write Scratchpad CMD */
  DRV_1Wire_WriteByte(WRITE_SCRATCHPAD_CMD);
  
  /* Send TA1 and TA2 */
  DRV_1Wire_WriteByte(Scratch.Addr[0]);
  DRV_1Wire_WriteByte(Scratch.Addr[1]);
  
  /* Write Scratchpad 8 bytes */
  for (i=0; i<WRITE_SCRATCHPAD_DATA_SIZE; i++)
  {
    DRV_1Wire_WriteByte(Scratch.Data[i]);
  }
  
  /* Read CRC first Byte */
  byte=0;
  DRV_1Wire_ReadByte(&byte);
  CRCDS[0] = byte;

  /* Read CRC second Byte */
  byte=0;
  DRV_1Wire_ReadByte(&byte);
  CRCDS[1] = byte;
  
  /* Prepare the CRC Tab for check */
  carctab[0] = WRITE_SCRATCHPAD_CMD;
  carctab[1] = Scratch.Addr[0];
  carctab[2] = Scratch.Addr[1];
  for(int i =0; i< WRITE_SCRATCHPAD_DATA_SIZE; i++)
  {
    carctab[i+3]= Scratch.Data[i];
  }
  
  /* CRC 16 Check */
  crcValue= (CRCDS[1]<<8 | CRCDS[0]);
  status = EEPROM_CRC16 ( carctab , WRITE_SCRATCHPAD_DATA_SIZE + 3 , crcValue);
  
  return status;
}

/**
  * @brief  Read Scratchpad
  * @param  Value to be read
  * @retval None
  */
DS2431_Status BSP_EEPROM_ReadScratchpad(uint8_t RxScrat[])
{
  uint8_t byte =0;
  uint8_t crctab[12] = {0};
  uint16_t crcValue=0;
  DS2431_Status status = DS2431_ERROR;
  int i=0;
  
  /* Send the read Scratchpad CMD */
  DRV_1Wire_WriteByte(READ_SCRATCHPAD_CMD);
  
  /* Read the TA1, TA2, E/S, the 8 data bytes and 2 data bytes of CRC*/
  for(i=0; i<WRITE_SCRATCHPAD_DATA_SIZE + 5; i++)
  {
    DRV_1Wire_ReadByte(&byte);
    RxScrat[i]=byte;
    byte=0;
  }

  /* Prepare the CRC Tab for check */
  crctab[0]= READ_SCRATCHPAD_CMD;
  for(i=0; i<WRITE_SCRATCHPAD_DATA_SIZE + 3 ; i++) // for TA1, TA2, E/S and the 8 data bytes
  {
    crctab[i+1]= RxScrat[i];
  }
  
  /* CRC 16 Check */
  crcValue = RxScrat[12]<<8 | RxScrat[11];
  status = EEPROM_CRC16 ( crctab, 12, crcValue);
  
  return status;
}

/**
  * @brief  Copy Scratchpad
  * @param  RxScratchpad
  * @retval status
  */
DS2431_Status BSP_EEPROM_CopyScratchpad(uint8_t RxScrat[])
{
  uint8_t bit=0;
  DS2431_Status status = DS2431_ERROR;

  /* Send the Copy Scratchpad CMD */
  DRV_1Wire_WriteByte(COPY_SCRATCHPAD_CMD);
  
  /* Send the TA1, TA2 and E/S bytes*/
  DRV_1Wire_WriteByte(RxScrat[0]);
  DRV_1Wire_WriteByte(RxScrat[1]);
  DRV_1Wire_WriteByte(RxScrat[2]);
  
  HAL_Delay(10);
  /* Read the received Bit */
  DRV_1Wire_ReadBit(&bit);
  
  if (bit == 0x00)
  {
    status = DS2431_OK;
  }
  else
  {
    status = DS2431_ERROR;
  }
  
  return status ;
} 

/**
  * @brief  Read memory
  * @param  Addess Memory TA1 and TA2
  * @param  pointer of Data to be read
  * @retval None
  */
void BSP_EEPROM_ReadMemory(uint8_t AddrMemory[],RxMemoryData *readData)
{
  uint8_t byte = 0;
  uint16_t i = 0;
  uint16_t memSize = 0 ;
  uint16_t targetAddress = 0 ;

  /* Send the Read Memory CMD */
  DRV_1Wire_WriteByte(READ_MEMORY_CMD);
  
  /* Send the TA1 and TA2 */
  readData->Addr[0]=AddrMemory[0];
  readData->Addr[1]=AddrMemory[1];
  DRV_1Wire_WriteByte(readData->Addr[0]);
  DRV_1Wire_WriteByte(readData->Addr[1]);

  targetAddress = (uint16_t)(AddrMemory[1]<<8);
  targetAddress |= (uint16_t)AddrMemory[0];
  
  memSize = RX_MEMORY_SIZE - targetAddress ;
  /*Read the entire memory starting from TA*/
  for(i=0; i<memSize; i++)
  {
    DRV_1Wire_ReadByte(&byte);
    readData->RxMemory[i]= byte;
    byte=0;
  }
}

/**
* @brief  Find the 'first' devices on the 1-Wire bus
* @param  None
* @retval TRUE : device found, ROM number in ROM_NO buffer
* @retval FALSE : no device present
*/
int BSP_EEPROM_First()
{
 // reset the search state
 LastDiscrepancy = 0;
 LastDeviceFlag = false;
 LastFamilyDiscrepancy = 0;
 
 return BSP_EEPROM_Search();
}

/**
* @brief  Find the 'next' devices on the 1-Wire bus
* @param  None
* @retval TRUE : device found, ROM number in ROM_NO buffer
* @retval FALSE : device not found, end of search
*/
int BSP_EEPROM_Next()
{
 // leave the search state alone
 return BSP_EEPROM_Search();
}

/**
* @brief  Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing search state.
* @param  None
* @retval TRUE : device found, ROM number in ROM_NO buffer
* @retval FALSE : device not found, end of search
*/
int BSP_EEPROM_Search()
{
  int id_bit_number;
  int last_zero, rom_byte_number, search_result;
  uint8_t id_bit, cmp_id_bit;
  unsigned char rom_byte_mask, search_direction;
  // initialize for search
  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;
  crc8 = 0;
  // if the last call was not the last one
  if (!LastDeviceFlag)
  {
    // 1-Wire reset
    if (DRV_1Wire_Reset() != StatusOK) //if (!OWReset())
    {
      // reset the search
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      return false;
    }
    // issue the search command
    DRV_1Wire_WriteByte(SEARCH_ROM_COMMAND); // OWWriteByte(0xF0);
    // loop to do the search
    do
    {
      // read a bit and its complement
      DRV_1Wire_ReadBit(&id_bit); // id_bit = OWReadBit();
      DRV_1Wire_ReadBit(&cmp_id_bit); // cmp_id_bit = OWReadBit();
        // check for no devices on 1-wire
        if ((id_bit == 1) && (cmp_id_bit == 1))
          break;
        else
        {
          // all devices coupled have 0 or 1
          if (id_bit != cmp_id_bit)
            search_direction = id_bit; // bit write value for search
          else
          {
            // if this discrepancy if before the Last Discrepancy
            // on a previous next then pick the same as last time
            if (id_bit_number < LastDiscrepancy)
              search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
            else
              // if equal to last pick 1, if not then pick 0
              search_direction = (id_bit_number == LastDiscrepancy);
            // if 0 was picked then record its position in LastZero
            if (search_direction == 0)
            {
              last_zero = id_bit_number;
              // check for Last discrepancy in family
              if (last_zero < 9)
                LastFamilyDiscrepancy = last_zero;
            }
          }
          // set or clear the bit in the ROM byte rom_byte_number
          // with mask rom_byte_mask
          if (search_direction == 1)
            ROM_NO[rom_byte_number] |= rom_byte_mask;
          else
            ROM_NO[rom_byte_number] &= ~rom_byte_mask;
          // serial number search direction write bit
          DRV_1Wire_WriteBit(search_direction); // OWWriteBit(search_direction);
          // increment the byte counter id_bit_number
          // and shift the mask rom_byte_mask
          id_bit_number++;
          rom_byte_mask <<= 1;
          // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
          if (rom_byte_mask == 0)
          {
            //docrc8(ROM_NO[rom_byte_number]); // accumulate the CRC
            rom_byte_number++;
            rom_byte_mask = 1;
          }
        }
    }
    while(rom_byte_number < 8); // loop until through all ROM bytes 0-7
    // if the search was successful then
    if (!((id_bit_number < 65) || (crc8 != 0)))
    {
      // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
      LastDiscrepancy = last_zero;
        // check for last device
        if (LastDiscrepancy == 0)
          LastDeviceFlag = true;
      
      search_result = true;
    }
  }
  // if no device found then reset counters so next 'search' will be like a first
  if (!search_result || !ROM_NO[0])
  {
    LastDiscrepancy = 0;
    LastDeviceFlag = false;
    LastFamilyDiscrepancy = 0;
    search_result = false;
  }
  return search_result;
}

/**
* @brief  Verify the device with the ROM number in ROM_NO buffer is present.
* @param  None
* @retval TRUE : device verified present
* @retval FALSE : device not present
*/
int BSP_EEPROM_Verify()
{
  unsigned char rom_backup[8];
  int i,rslt,ld_backup,ldf_backup,lfd_backup;
  // keep a backup copy of the current state
  for (i = 0; i < 8; i++)
    rom_backup[i] = ROM_NO[i];
  ld_backup = LastDiscrepancy;
  ldf_backup = LastDeviceFlag;
  lfd_backup = LastFamilyDiscrepancy;
  // set search to find the same device
  LastDiscrepancy = 64;
  LastDeviceFlag = false;
  if (BSP_EEPROM_Search())
  {
    // check if same device found
    rslt = true;
    for (i = 0; i < 8; i++)
    {
      if (rom_backup[i] != ROM_NO[i])
      {
        rslt = false;
        break;
      }
    }
  }
  else
    rslt = false;
  // restore the search state
  for (i = 0; i < 8; i++)
    ROM_NO[i] = rom_backup[i];
    LastDiscrepancy = ld_backup;
  LastDeviceFlag = ldf_backup;
  LastFamilyDiscrepancy = lfd_backup;
  // return the result of the verify
  return rslt;
}

/**
* @brief  Setup the search to find the device type 'family_code' on the next call
to OWNext() if it is present.
*/
void BSP_EEPROM_TargetSetup(unsigned char family_code)
{
  int i;
  // set the search state to find SearchFamily type devices
  ROM_NO[0] = family_code;
  for (i = 1; i < 8; i++)
    ROM_NO[i] = 0;
  LastDiscrepancy = 64;
  LastFamilyDiscrepancy = 0;
  LastDeviceFlag = false;
}

/**
* @brief  Setup the search to skip the current device type on the next call
to OWNext().
*/
void BSP_EEPROM_FamilySkipSetup()
{
  // set the Last discrepancy to last family discrepancy
  LastDiscrepancy = LastFamilyDiscrepancy;
  LastFamilyDiscrepancy = 0;
  // check for end of list
  if (LastDiscrepancy == 0)
    LastDeviceFlag = true;
}

/**
  * @brief  Resume
  * @param  None
  * @retval None
  */
void BSP_EEPROM_Resume(void)
{
  DRV_1Wire_WriteByte(RESUME_CMD);
}

void BSP_EEPROM_FindAllDevices(uint8_t* count, ReadRomCmd *RxRomCmd)
{
  int cnt = 0;
  int rslt,i ; 
  
  // find ALL devices
  __BSW_PRINTF("\nFIND ALL\n");
  
  rslt = BSP_EEPROM_First();
  while (rslt)
  {
    // print device found
    for (i= SERIAL_NUMBER_SIZE + 1 ; i >= 0; i--)
    {
      __BSW_PRINTF("%02X", ROM_NO[i]);
      ROM_List[cnt][i] = ROM_NO[i];
    }
    __BSW_PRINTF(" %d\n",++cnt);
    rslt = BSP_EEPROM_Next();
  }
  
  *count  = cnt;
  
  for (i=0; i<cnt; i++)
  {
    RxRomCmd[i].FamilyCode = ROM_List[i][0] ;
    RxRomCmd[i].SerialNumber[0] =  ROM_List[i][1];
    RxRomCmd[i].SerialNumber[1] =  ROM_List[i][2];
    RxRomCmd[i].SerialNumber[2] =  ROM_List[i][3];
    RxRomCmd[i].SerialNumber[3] =  ROM_List[i][4];
    RxRomCmd[i].SerialNumber[4] =  ROM_List[i][5];
    RxRomCmd[i].SerialNumber[5] =  ROM_List[i][6];  
    RxRomCmd[i].CRCDS2431 =  ROM_List[i][7];
  }
}


/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/  