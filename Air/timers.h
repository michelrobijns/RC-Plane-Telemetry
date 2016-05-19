/*
 * timers.h
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED

void timersSetup(void);
extern void timers1000Hz(void);
extern void timers5Hz(void);

#endif
