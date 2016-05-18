/*
 * adc.h
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

uint16_t adcReadFromPin(uint8_t pin);
void adcSetup(void);
void adcStartConversion(void);

#endif
