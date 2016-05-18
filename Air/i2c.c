/*
   i2c.c
   
   This code is based on the BMP085 library by Stefan Sicklinger published on
   http://www.sicklinger.com.
   
   I am redistributing this file under the GNU General Public License.
*/

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <avr/io.h>
#include <util/delay.h> 
#include <util/twi.h>
#include "i2c.h"

void i2cSetup()
{    
    // Set the Serial Clock Line prescalar
    TWSR = ((0 << TWPS1) | (0 << TWPS0));
    
    // Set the frequency of the Serial Clock Line to 200 kHz
    TWBR = 32;
}

void i2cStart(void)
{
    // Send start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
}

uint8_t i2cStop(void)
{
    uint8_t i = 0; // Time out variable
    uint8_t errorCode = 0;
    
    // Transmit stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    // Wait until stop condition is executed and bus released
    while ((TWCR & (1 << TWSTO)) && (i < 90))
    {
        _delay_us(1);
        i++;
    }
    
    if (i > 89)
    {
        _delay_us(1);
        errorCode = 100;
    }
    
    return errorCode;
}

void i2cSendByte(uint8_t data)
{
    // Save data to the TWDR
    TWDR = data;
    
    // Begin send
    TWCR = (1 << TWINT) | (1 << TWEN);
}

void i2cReadByteACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
}

void i2cReadByteNOACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
}

uint8_t i2cWaitForComplete(void)
{
    uint8_t i = 0; // Time out variable
    uint8_t errorCode = 0;

    // Wait for the i2c interface to complete the operation
    while ((!(TWCR & (1 << TWINT))) && (i < 90))
    {
        _delay_us(1);
        i++;
    }
    
    if (i > 89)
    {
        _delay_us(1);
        errorCode = 100;
    }
    
    return errorCode;
}

uint8_t i2cGetReceivedByte(void)
{
    return (uint8_t) TWDR;
}

uint8_t i2cCheckReturnCode(uint8_t expectedErrorCode)
{
    uint8_t errorCode = 0;
    
    if ((TWSR & 0xF8) != expectedErrorCode)
    {
        errorCode = 10;
    }
        
    return errorCode;
}
