/*
 * timers.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of avr-telemetry which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include "timers.h"

void setupTimers(void)
{
//     // 8-bit Timer/Counter0
//     
//     // Enable Clear Timer on Compare Match (CTC) mode
//     TCCR0A = (1 << WGM01);
//     
//     // Set the prescaler to 256
//     TCCR0B = (1 << CS02);
//     
//     // Set the Output Compare Register to a counter value (TCNT0) of 125
//     OCR0A = 125;
//     
//     // Enable Output Compare A Match interrupt
//     TIMSK0 = (1 << OCIE0A);
    
    // 16-bit Timer/Counter1
    
    // Enable Clear Timer on Compare Match (CTC) mode
    TCCR1B = (1 << WGM12);
    
    // Set the prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    
    // Set the Output Compare Register to a counter value (TCNT1) of 25
    OCR1A = 25;
    
    // Enable Output Compare A Match interrupt
    TIMSK1 = (1 << OCIE1A);
}

// Counter0 Output Compare A Interrupt
// ISR(TIMER0_COMPA_vect)
// {
//     ;
// }

// Counter1 Output Compare A Interrupt
// This Interrupt Service Routine (ISR) is triggered 10000 times per second
ISR(TIMER1_COMPA_vect)
{
    // Call doAt10kHz() defined in main.c
    doAt10kHz();
}
