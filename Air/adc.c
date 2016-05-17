/*
 * adc.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of avr-telemetry which is released under the MIT license. 
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "serial.h"

#define SAMPLES 100

void selectADCChannel(uint8_t channel);

uint32_t accumulatorADC0 = 0;
uint32_t accumulatorADC1 = 0;

uint16_t samplesADC0 = 0;
uint16_t samplesADC1 = 0;

uint16_t voltage;
uint16_t current;

void setupADC(void)
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
    startConversion();
}

// Conversion will be started manually to control the sampling rate
void startConversion(void)
{
    // ADC start conversion
    ADCSRA |= (1 << ADSC);
}

// Update the voltage if enough samples have been accumulated and start a new conversion
void updateADC(void)
{
    if (samplesADC0 == SAMPLES)
    {
        voltage = accumulatorADC0 / samplesADC0;
        accumulatorADC0 = 0;
        samplesADC0 = 0;
    }
    
    if (samplesADC1 == SAMPLES)
    {
        current = accumulatorADC1 / samplesADC1;
        accumulatorADC1 = 0;
        samplesADC1 = 0;
    }

    startConversion();
}

// ADC Conversion Complete Interrupt
ISR(ADC_vect)
{
    // Get current channel
    uint8_t channel = ADMUX & 0b00000111;
    
    switch (channel) {
        case 0:
            accumulatorADC0 += ADC;
            samplesADC0++;
            break;
        case 1:
            accumulatorADC1 += ADC;
            samplesADC1++;
            break;
//         case 2:
//             ;
//             break;
//         case 3:
//             ;
//             break;
//         case 4:
//             ;
//             break;
//         case 5:
//             ;
//             break;
    }
    
    // Select the next channel
    if (channel == 1)
        selectADCChannel(0);
    else
        selectADCChannel(channel + 1);
}

void selectADCChannel(uint8_t channel)
{
    ADMUX = (ADMUX & 0b11111000) | (channel & 0b00000111);
}
