/*
 * timers.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"

void timersSetup(void)
{
    // 8-bit Timer/Counter0
    
    // Enable Clear Timer on Compare Match (CTC) mode
    TCCR0A = (1 << WGM01);
    
    // Set the prescaler to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    
    // Set the Output Compare Register to a counter value (TCNT0) of 250
    OCR0A = 250;
    
    // Enable Output Compare A Match interrupt
    TIMSK0 = (1 << OCIE0A);
    
    // 16-bit Timer/Counter1
    
    // Enable Clear Timer on Compare Match (CTC) mode
    TCCR1B = (1 << WGM12);
    
    // Set the prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    
    // Set the Output Compare Register to a counter value (TCNT1) of 25000
    OCR1A = 25000;
    
    // Enable Output Compare A Match interrupt
    TIMSK1 = (1 << OCIE1A);
}

// Counter0 Output Compare A Interrupt
// This Interrupt Service Routine (ISR) is triggered 1000 times per second
ISR(TIMER0_COMPA_vect)
{
    // Call timers1000Hz() defined in main.c
    timers1000Hz();
}

// Counter1 Output Compare A Interrupt
// This Interrupt Service Routine (ISR) is triggered 10 times per second
ISR(TIMER1_COMPA_vect)
{
    // Call timers10Hz() defined in main.c
    timers10Hz();
}
