/*
 * adc.h
 *
 * Author: Michel Robijns
 * 
 * This file is part of avr-telemetry which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

extern uint16_t voltage;
extern uint16_t current;

void setupADC(void);
void startConversion(void);
void updateADC(void);

#endif
