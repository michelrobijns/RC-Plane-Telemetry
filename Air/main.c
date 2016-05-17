/*
 * main.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of avr-telemetry which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include "timers.h"
#include "serial.h"
#include "adc.h"
#include "i2c.h"
#include "bmp.h"

// Declare external global variables
//extern char rxBuffer[MESSAGE_SIZE];
extern char txBuffer[MESSAGE_SIZE];
extern uint16_t voltage;

// Declare external functions
extern void doAt1000Hz(void);
extern void doAt10Hz(void);

uint16_t counter1 = 0;
uint16_t counter2 = 0;

int32_t temperature = 0;
int32_t pressure = 0;
uint8_t errorCode = 0;

int main(void)
{
    // Call setup functions
    i2cSetup();
    bmpCalibrate(&errorCode);
    setupSerial();
    setupTimers();
    setupADC();
    
    // Set external interrupts
    sei();
    
    while (1)
    {
        // Infinite loop
    }
    
    return 0;
}

// This function is called 1000 times per second
void doAt1000Hz(void)
{
    counter1++;
    counter2++;

    // Triggered 1000 times per second
    if (counter1 == 1)
    {
        updateADC();
        counter1 = 0;
    }

    // Triggered 10 times per second
    if (counter2 == 100)
    {
        txBuffer[0] = voltage >> 2 | 0b00000111;
        txBuffer[1] = ((voltage << 3) & 0xFF) | 0b00000111;
                        
        txBuffer[2] = current >> 2 | 0b00000111;
        txBuffer[3] = ((current << 3) & 0xFF) | 0b00000111;
        
        txBuffer[4] = (pressure >> 24) | 0b00000011;
        txBuffer[5] = (pressure >> 18) | 0b00000011;
        txBuffer[6] = (pressure >> 12) | 0b00000011;
        txBuffer[7] = (pressure >> 6) | 0b00000011;
        txBuffer[8] = (pressure >> 0) | 0b00000011;
        txBuffer[9] = ((pressure << 6) & 0xFF) | 0b00111111;

        // Send whatever is in the TX buffer
        sendTxBuffer();

        counter2 = 0;
    }
}

// This function is called 10 times per second
void doAt10Hz(void)
{
    bmpComputePressureAndTemperature(&temperature, &pressure, &errorCode);
}
