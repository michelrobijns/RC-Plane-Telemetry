/*
   This code is based on the BMP085 library by Stefan Sicklinger published on
   http://www.sicklinger.com
   
   I am redistributing this modified version under the GNU General Public
   License version 3
*/

#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

void i2cSetup();
  /* Sets pullup and initializes bus speed to 200 kHz (at F_CPU=16000000UL) */

void i2cStart(void);
                                    /* Sends a start condition (sets TWSTA) */

uint8_t i2cStop(void);
                                     /* Sends a stop condition (sets TWSTO) */

void i2cSendByte(uint8_t data);
                                                       /* Sends a data byte */

void i2cReadByteACK(void);
                   /* Reads a data byte and sends ACK when done (sets TWEA) */

void i2cReadByteNOACK(void);
         /* Reads a data byte and sends NOACK when done (does not set TWEA) */

uint8_t i2cGetReceivedByte(void);
                   /* Returns the received data byte form the TWDR register */

uint8_t i2cWaitForComplete(void);
                            /* Waits until the hardware sets the TWINT flag */

uint8_t i2cCheckReturnCode(uint8_t expectedErrorCode);
                 /* Compares the i2c error code with an expected error code */

#endif
