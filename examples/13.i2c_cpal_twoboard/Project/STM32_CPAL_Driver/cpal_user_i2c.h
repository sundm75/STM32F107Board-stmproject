
#ifndef __USER_I2C_H
#define __USER_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "cpal_i2c.h"

#define I2C_DevStructure        I2C1_DevStructure


#define countof(a) (sizeof(a) / sizeof(*(a)))


#define MAX_BUFF_SIZE           256
#define BUFFER_SIZE             (countof(tTxBuffer)-1)

#define OWN_ADDRESS             0x78

/* Select clock Speed */
/* To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral
   input clock) must be a multiple of 10 MHz */
   
#define I2C_SPEED                        300000


void CPAL_I2c_Config(void);
void User_I2c_Write(void);
void User_I2c_Read(void);

#endif /* __USER_I2C_H */

