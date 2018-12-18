/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_FRAM_SPI_H
#define __BSP_FRAM_SPI_H

#include "DRV_SPI.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private define ------------------------------------------------------------*/
/* Define FRAM commands ------------------------------------------------------*/
/* These commands are used to set the mode and operation required ------------*/

#define OPCODE_WREN  ((uint8_t) 0x06)  //0b00000110         /* Set Write Enable Latch : required before WRITE and WRSR commands         */
#define OPCODE_WRDI  ((uint8_t) 0x04)  //0b00000100         /* Reset Write Enable Latch : if performed WRITE operation could not occur  */
#define OPCODE_RDSR  ((uint8_t) 0x05)  //0b00000101         /* Read Status Register                                                     */ 
#define OPCODE_WRSR  ((uint8_t) 0x01)  //0b00000001         /* Write Status Register                                                    */ 
#define OPCODE_READ  ((uint8_t) 0x03)  //0b00000011         /* Read Memory Code                                                         */ 
#define OPCODE_WRITE ((uint8_t) 0x02)  //0b00000010         /* Write Memory Code                                                        */ 
#define OPCODE_RDID  ((uint8_t) 0x9F)  //0b10011111         /* Read Device ID                                                           */
#define OPCODE_FSTRD ((uint8_t) 0x0B)  //0b00001011         /* Fast Read Memory Code                                                    */
#define OPCODE_SLEEP ((uint8_t) 0xB9)  //0b10111001         /* Sleep Mode                                                               */

typedef enum
{
 BSP_FRAM_OK = 0x00,
 BSP_FRAM_ERROR = 0x01,
 BSP_FRAM_BUSY  = 0x02, 
 BSP_FRAM_TIMEOUT = 0x03 
}
BSP_FRAM_STATUS;


typedef enum
{
 WRITE_DISABLE = 0,
 WRITE_ENABLE	
}
Write_EnableDisable;


BSP_FRAM_STATUS BSP_FRAM_Write_EnableDisable(SPI_HandleTypeDef* hSpi, Write_EnableDisable write);
BSP_FRAM_STATUS BSP_FRAM_WriteData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size);
BSP_FRAM_STATUS BSP_FRAM_ReadData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size);
BSP_FRAM_STATUS BSP_FRAM_FastReadData(SPI_HandleTypeDef* hSpi,uint32_t addr, uint8_t *data, uint32_t size);
BSP_FRAM_STATUS BSP_FRAM_SetStatusRegister(SPI_HandleTypeDef* hSpi,uint8_t value);
BSP_FRAM_STATUS BSP_FRAM_GetStatusRegister(SPI_HandleTypeDef* hSpi, uint8_t *data);
BSP_FRAM_STATUS BSP_FRAM_EnterSleepMode(SPI_HandleTypeDef* hSpi);
BSP_FRAM_STATUS BSP_FRAM_ReadDeviceID(SPI_HandleTypeDef* hSpi, uint8_t *ID);

#ifdef __cplusplus
}
#endif

#endif /*__BSP_FRAM_SPI_H */