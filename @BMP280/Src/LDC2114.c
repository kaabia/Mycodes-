//void LDC2114_Config(){
  
  // 1. read Register STATUS (Address 0x00) first to lock the data
  
    // 2. write sequence,
    // 2.a  Set CONFIG_MODE (Register RESET, Address 0x0A) bit = 1 to start the register write session
    // 2.b Poll for RDY_TO_WRITE (Register STATUS, Address 0x00) bit = 1
    // 2.c I2C write to configure registers
    // 2.d Set CONFIG_MODE (Register RESET, Address 0x0A) bit = 0 to terminate the register write session

  
  
//}