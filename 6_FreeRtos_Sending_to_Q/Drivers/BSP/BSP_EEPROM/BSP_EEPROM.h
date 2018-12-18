/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_EEPROM/BSP_EEPROM.h $
* $Author: ghada.dhibi $
* $Revision: 486 $
* $Date: 2016-06-23 13:07:31 +0100 (jeu., 23 juin 2016) $
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Includes ------------------------------------------------------------------*/
#include "DRV_1Wire.h"
  
  /* Private define ------------------------------------------------------------*/
#define MAX_SLAVE_CONNECTED             0x04
#define SERIAL_NUMBER_SIZE              0x06    // 6 bytes serial number
#define WRITE_SCRATCHPAD_DATA_SIZE      0x08    // 8-byte sratchpad data 
#define RX_MEMORY_SIZE                  0x0090  // Total EEPROM Memory Size
#define DS2431_FAMILY_CODE              0x2D
  
  /*  1-Wire ROM FUNCTION COMMANDS Definition */
#define READ_ROM_CMD                    0x33    // READ ROM COMMAND Definition
#define MATCH_ROM_CMD                   0x55    // Match ROM COMMAND Definition
#define SKIP_ROM_CMD                    0xCC    // Skip ROM COMMAND Definition
#define SEARCH_ROM_COMMAND              0xF0    // Search ROM COMMAND Definition 
#define RESUME_CMD                      0xA5    // Resume COMMAND Definition 
#define OVERDRIVE_SKIP_ROM              0x3C    // OVERDRIVE SKIP ROM COMMAND Definition 
#define OVERDRIVE_MATCH_ROM             0x69    // OVERDRIVE MATCH ROM COMMAND Definition 
  /*  DS2431-SPECIFIC MEMORY FUNCTION COMMANDS Definition */
#define READ_SCRATCHPAD_CMD             0xAA    // Read SCRATCHPAD COMMAND Definition
#define COPY_SCRATCHPAD_CMD             0x55    // Copy SCRATCHPAD COMMAND Definition
#define WRITE_SCRATCHPAD_CMD            0x0F    // Write SCRATCHPAD COMMAND Definition
#define READ_MEMORY_CMD                 0xF0    // Read Memory COMMAND Definition
  
  /* Exported types ------------------------------------------------------------*/ 
//  typedef struct 
//  {
//    uint8_t FamilyCode;
//    uint8_t SerialNumber[SERIAL_NUMBER_SIZE];
//    uint8_t CRCDS2431;
//  } ReadRomCmd; // Read Rom Data tructure
  
  typedef struct 
  {
    uint8_t Addr[2];
    uint8_t Data[8];
    uint8_t CRCDS[2];
  } WriteScratchpadCmd; // Write scratchpad Data tructure
  
  
  typedef struct
  {
    uint8_t Addr[2];
    uint8_t RxMemory[RX_MEMORY_SIZE];
  } RxMemoryData; // Read Memory Data tructure
  
  typedef enum
  {
    DS2431_OK,
    DS2431_ERROR
  } DS2431_Status; // DS2431 Status
  
  
  /* Exported function prototypes ----------------------------------------------*/
  
#define BSP_EEPROM_Init()       DRV_1Wire_Init()
#define BSP_EEPROM_Reset()      DRV_1Wire_Reset()
  
  DS2431_Status BSP_EEPROM_ReadRom(ReadRomCmd *RxRomCmd);
  void BSP_EEPROM_SkipRom(void);
  void BSP_EEPROM_MatchRom(ReadRomCmd RxRomCmd);
  
  DS2431_Status BSP_EEPROM_WriteScratchpad(WriteScratchpadCmd Scratch, uint8_t  CRCDS[]);
  DS2431_Status BSP_EEPROM_ReadScratchpad(uint8_t RxScrat[]);
  DS2431_Status BSP_EEPROM_CopyScratchpad(uint8_t RxScrat[]);
  void BSP_EEPROM_ReadMemory(uint8_t AddrMemory[],RxMemoryData *readData);
  
  int BSP_EEPROM_First(void);
  int BSP_EEPROM_Next(void);
  int BSP_EEPROM_Search(void);
  int BSP_EEPROM_Verify(void);
  void BSP_EEPROM_FindAllDevices(uint8_t* count, ReadRomCmd *RxRomCmd);
  void BSP_EEPROM_TargetSetup(unsigned char family_code);
  void BSP_EEPROM_FamilySkipSetup(void); 
  void BSP_EEPROM_Resume(void);
  /* Exported function  --------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT OPCMA *****END OF FILE*****************/ 