/*
Enabling/disabling the Pressure and tepm measurement and oversampling
settings are selected through the osrs_p[2:0] bits in control register 0xF4.
In order to find a suitable setting for osrs_p, please consult chapter 3.4.

Power 
The BMP280 offers three power modes: sleep mode, forced mode and normal mode. These
can be selected using the mode[1:0] bits in control register 0xF4.
00 Sleep mode ..  01 and 10 Forced mode .. 11 Normal mode

Data readout
To read out data after a conversion, it is strongly recommended to use a burst read and not
address every register individually.This will prevent a possible mix-up of bytes belonging to
different measurements and reduce interface traffic. Data readout is done by starting a burst
read from 0xF7 to 0xFC . For details on memory map and interfaces, please
consult chapters 3.12 and 5 respectively.After the values of ‘ut’ and ‘up’ have
been read, the actual pressure and temperature need to be calculated using the compensation
parameters stored in the device. The procedure is elaborated in chapter 3.11.

Trimming parameter readout The mapping is shown in Table 17. page 21
Compensation formula page 22 


I2c
slave address 1110110 (0x76) Connecting SDO to GND.The SDO pin cannot be left floating.
To be able to read registers, first the register address must be sent in write mode (slave address 111011X0).
Compensation formula in 32 bit fixed point page 45 
*/

// Datasheet page 25 
 
#define ID 0xD0 // contains the chip identification number chip which is 0x58.
#define RESET 0xE0 // register contains the soft reset word reset The readout value is always 0x00.
#define STATUS 0xF3 // The “status” register contains two bits which indicate the status of the device..
#define ctrl_meas 0xF4 // The “ctrl_meas” register sets the data acquisition options of the device.
#define config 0xF5 // The “config” register sets the rate, filter and interface options of the device. 
//Writes to the “config” register in normal mode may be ignored..

#define press_msb 0xF7 // Contains the MSB part up[19:12]
#define press_lsb 0xF8   // Contains the LSB part up[11:4]
#define press_xlsb 0xF9 // Contains the XLSB part up[3:0]       

#define temp_msb 0xFA // Contains the MSB part uT[19:12]
#define temp_lsb 0xFB   // Contains the LSB part uT[11:4]
#define temp_xlsb 0xFC // Contains the XLSB part uT[3:0] 
#define BMP280_S32_t int32_t

uint16_t dig_T1 ;
int16_t dig_T2 ;
int16_t dig_T3 ;


