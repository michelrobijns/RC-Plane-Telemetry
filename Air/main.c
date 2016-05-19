/*
 * main.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"
#include "uart.h"
#include "adc.h"
#include "i2c.h"
#include "bmp.h"

#define ADC_PIN_VOLTAGE 0
#define ADC_PIN_CURRENT 1

// Declare external functions
extern void timers1000Hz(void);
extern void timers5Hz(void);

static volatile uint16_t voltage;
static volatile uint16_t current;
static int32_t temperature = 0;
static int32_t pressure = 0;
static volatile uint32_t adcTotal[6] = {0, 0, 0, 0, 0, 0};
static volatile uint8_t samples;
static uint8_t errorCode = 0;

int main(void)
{
    // Call setup functions
    bmpSetup(&errorCode);
    uartSetup();
    timersSetup();
    adcSetup();
    
    // Set external interrupts
    sei();
    
    while (1)
    {
        // Infinite loop
    }
    
    return 0;
}

// This function is called 1000 times per second
void timers1000Hz(void)
{
    adcTotal[0] += adcReadFromPin(ADC_PIN_VOLTAGE);
    adcTotal[1] += adcReadFromPin(ADC_PIN_CURRENT);
    
    samples++;
    
    if (samples == 200)
    {
        voltage = adcTotal[0] / samples;
        current = adcTotal[1] / samples;
        
        adcTotal[0] = 0;
        adcTotal[1] = 0;
        
        samples = 0;
    }
    
    adcStartConversion();
}

// This function is called 10 times per second
void timers5Hz(void)
{
    bmpComputePressureAndTemperature(&temperature, &pressure, &errorCode);
    
    uartWriteTxBuffer(0, voltage >> 2 | 0b00000111);
    uartWriteTxBuffer(1, ((voltage << 3) & 0xFF) | 0b00000111);

    uartWriteTxBuffer(2, current >> 2 | 0b00000111);
    uartWriteTxBuffer(3, ((current << 3) & 0xFF) | 0b00000111);

    uartWriteTxBuffer(4, (pressure >> 24) | 0b00000011);
    uartWriteTxBuffer(5, (pressure >> 18) | 0b00000011);
    uartWriteTxBuffer(6, (pressure >> 12) | 0b00000011);
    uartWriteTxBuffer(7, (pressure >> 6) | 0b00000011);
    uartWriteTxBuffer(8, (pressure >> 0) | 0b00000011);
    uartWriteTxBuffer(9, ((pressure << 6) & 0xFF) | 0b00111111);

    // Send whatever is in the TX buffer
    uartSendTxBuffer();
}
