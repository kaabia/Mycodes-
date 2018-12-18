/**
******************************************************************************
* $HeadURL: svn://10.0.0.7/Bioserenity_BBNaute/Project%20Folder/Technical/OPCMA/Integration/BSW_Project/Drivers/BSP/BSP_ACC/BSP_LIS3DH_Conf.h $
* $Author: ghada.dhibi $
* $Revision: 451 $
* $Date: 2016-06-13 11:34:27 +0100 (lun., 13 juin 2016) $
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LIS3DH_CONF_H
#define __BSP_LIS3DH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif
  
#define X_AXIS  0
#define Y_AXIS  1
#define Z_AXIS  2

typedef struct 
{
  int16_t X;
  int16_t Y;
  int16_t Z; 
}LIS3DH_TypeDef;

//extern LIS3DH_TypeDef AcceData;

/*================I2C ADDRESS/BITS of Device SDO/SA0 is GND ===============*/ 
 #define LIS3DH_R_ADDRESS  (0x31) 
 #define LIS3DH_W_ADDRESS  (0x30)                      
/*=========================================================================*/
  
#define STATUS_REG_AUX   0x07
#define OUT_ADC1_L       0x08
#define OUT_ADC1_H       0x09
#define OUT_ADC2_L       0x0A
#define OUT_ADC2_H       0x0B
#define OUT_ADC3_L       0x0C
#define OUT_ADC3_H       0x0D
#define INT_COUNTER_REG  0x0E

#define WHO_AM_I         0x0F
#define WHO_AM_I_VALUE   0x33

#define TEMP_CFG_REG     0x1F

#define CTRL_REG1        0x20
#define CTRL_REG1_DEF    0x07
#define CTRL_REG1_XEN    (0x01<<0)
#define CTRL_REG1_YEN    (0x01<<1)
#define CTRL_REG1_ZEN    (0x01<<2)
#define CTRL_REG1_LPEN   (0x01<<3)
#define CTRL_REG1_ODR_MASK  0xF0

#define CTRL_REG1_ODR_0     (0<<4)
#define CTRL_REG1_ODR_1     (1<<4)
#define CTRL_REG1_ODR_10    (2<<4)
#define CTRL_REG1_ODR_25    (3<<4)
#define CTRL_REG1_ODR_50    (4<<4)
#define CTRL_REG1_ODR_100   (5<<4)
#define CTRL_REG1_ODR_200   (6<<4)
#define CTRL_REG1_ODR_400   (7<<4)
#define CTRL_REG1_ODR_1_6K  (8<<4)
#define CTRL_REG1_ODR_5_0K  (9<<4)

#define CTRL_REG2        0x21

#define CTRL_REG2_HPM_0         (1<<6)  // Add for Test bench 
#define CTRL_REG2_IFB           (1<<3)  // Internal filter bypassed

#define CTRL_REG3        0x22
#define CTRL_REG3_DEF    0
#define CTRL_REG3_I1_CLICK   (1<<7)
#define CTRL_REG3_I1_AOI1    (1<<6)
#define CTRL_REG3_I1_AOI2    (1<<5)
#define CTRL_REG3_I1_DRDY1   (1<<4)
#define CTRL_REG3_I1_DRDY2   (1<<3)
#define CTRL_REG3_I1_WTM     (1<<2)
#define CTRL_REG3_I1_OVERRUN (1<<1)

#define CTRL_REG4             0x23

#define CTRL_REG4_DEF         0x00
#define CTRL_REG_BDU         (1<<7)
#define CTRL_REG_BLE         (1<<6)

#define CTRL_REG4_FS2G       (0<<4)
#define CTRL_REG4_FS4G       (1<<4)
#define CTRL_REG4_FS8G       (2<<4)
#define CTRL_REG4_FS16G      (3<<4)

#define CTRL_REG5        0x24

#define CTRL_REG5_FIFO_BOOT     0x80    // Add for Test bench  
#define CTRL_REG5_FIFO_EN       0x40    // Add for Test bench 
#define CTRL_REG5_LIR_INT1      0x08    // Add for Test bench 
#define CTRL_REG5_D4D_INT1      0x04    // Add for Test bench 

#define CTRL_REG6        0x25
#define CTRL_REG6_DEF    0
#define CTRL_REG6_I2_CLICKEN  (1<<7) 8

#define FIFO_CTRL_REG    0x2E
#define FIFO_CTRL_FMx_BM    0x00
#define FIFO_CTRL_FMx_FM    (1<<6)
#define FIFO_CTRL_FMx_SM    (1<<7)
#define FIFO_CTRL_FMx_TM    0xC0

#define REFERENCE        0x26
#define STATUS_REG2      0x27
#define OUT_X_L          0x28
#define OUT_X_H          0x29
#define OUT_Y_L          0x2A
#define OUT_Y_H          0x2B
#define OUT_Z_L          0x2C
#define OUT_Z_H          0x2D
#define FIFO_SRC_REG     0x2F
#define INT1_CFG         0x30
#define INT1_SOURCE      0x31
#define INT1_THS         0x32
#define INT1_DURATION    0x33

#define CLICK_CFG        0x38
#define CLICK_CFG_DEF    0
#define CLICK_CFG_ZD     (1<<5)
#define CLICK_CFG_ZS     (1<<4)
#define CLICK_CFG_YD     (1<<3)
#define CLICK_CFG_YS     (1<<2)
#define CLICK_CFG_XD     (1<<1)
#define CLICK_CFG_XS     (1<<0)

#define CLICK_SRC        0x39
#define CLICK_THS        0x3A
#define TIME_LIMIT       0x3B
#define TIME_LATENCY     0x3C
#define TIME_WINDOW      0x3D

/* Range definitions. Map them to actual bitmasks. */
#define RANGE_2G   CTRL_REG4_FS2G
#define RANGE_4G   CTRL_REG4_FS4G
#define RANGE_8G   CTRL_REG4_FS8G
#define RANGE_16G  CTRL_REG4_FS16G


#define LIS3DH_Sense_2g   0.06
#define LIS3DH_Sense_4g   0.12
#define LIS3DH_Sense_8g   0.24
#define LIS3DH_Sense_16g  0.73

#ifdef __cplusplus
}
#endif

#endif /*__BSP_LIS3DH_CONF_H */
/************************ (C) COPYRIGHT OPCMA ******************END OF FILE****/