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

// Declare external global variables
//extern char rxBuffer[MESSAGE_SIZE];
extern char txBuffer[MESSAGE_SIZE];
extern uint16_t voltage;

// Declare external functions
extern void doAt500Hz(void);

uint16_t counter1 = 0;
uint16_t counter2 = 0;

int main(void)
{
    // Call setup functions
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

// This function is called 10000 times per second
void doAt10kHz(void)
{
    counter1++;
    counter2++;

    // Triggered 2500 times per second
    if (counter1 == 4)
    {
        updateADC();
        counter1 = 0;
    }

    // Triggered 10 times per second
    if (counter2 == 1000)
    {
        txBuffer[3] = voltage >> 2 | 0b00000111;
        txBuffer[4] = ((voltage << 3) & 0xFF) | 0b00000111;
        
        txBuffer[5] = current >> 2 | 0b00000111;
        txBuffer[6] = ((current << 3) & 0xFF) | 0b00000111;

        // Send whatever is in the TX buffer
        sendTxBuffer();

        counter2 = 0;
    }
}
