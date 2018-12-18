/**
  ******************************************************************************
  * $HeadURL:$
  * $Author:$
  * $Revision:$
  * $Date:$
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_amazon_S3_H
#define __BSP_amazon_S3_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "crypto.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/  
int Authorization_generation(void);
int chunked_authorization(uint8_t * Chunk_Authorization,uint8_t *signature, uint32_t content_length, uint32_t payload_length, uint8_t *my_signingkey);
void File_to_Digest(uint8_t * file_name);
int PUT_Authorization_calculation(void);
#ifdef __cplusplus
}
#endif

#endif /*__BSP_amazon_S3_H */


/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/
