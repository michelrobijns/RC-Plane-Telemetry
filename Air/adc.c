/*
 * adc.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

void adcSelectChannel(uint8_t channel);

static volatile uint16_t adcReadings[6] = {0, 0, 0, 0, 0, 0};

uint16_t adcReadFromPin(uint8_t pin)
{
    return adcReadings[pin];
}

void adcSetup(void)
{
    // Select the reference voltage to VCC
    ADMUX = (1 << REFS0);
    
    // Set ADC5 to as input channel
    //ADMUX |= (1 << MUX0) | (1 << MUX2);
    
    // Set ADC0 to as input channel
    ADMUX |= (1 << MUX0);
    
    // Enable the ADC
    ADCSRA = (1 << ADEN);
    
    // Enable the ADC Conversion Complete Interrupt
    ADCSRA |= (1 << ADIE);
    
    // Set the division factor between the system clock frequency and the input clock to the ADC to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Disable the digital input buffer
    DIDR0 = (1 << ADC5D);
    
    // ADC start conversion
    adcStartConversion();
}

// Conversion will be started manually to control the sampling rate
void adcStartConversion(void)
{
    // ADC start conversion
    ADCSRA |= (1 << ADSC);
}

// ADC Conversion Complete Interrupt
ISR(ADC_vect)
{
    // Get current channel
    uint8_t channel = ADMUX & 0b00000111;
    
    adcReadings[channel] = ADC;
    
    // Select the next channel
    if (channel == 1) {
        adcSelectChannel(0);
    } else {
        adcSelectChannel(channel + 1);
    }
}

void adcSelectChannel(uint8_t channel)
{
    ADMUX = (ADMUX & 0b11111000) | (channel & 0b00000111);
}
