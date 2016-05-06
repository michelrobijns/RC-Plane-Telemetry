/*
 * timers.h
 *
 * Created: 8/4/2015
 * Author: Michel Robijns
 * 
 * This file is part of avrplane which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED

#include <avr/io.h>

void setupTimers(void);
extern void doAt10kHz(void);

#endif
