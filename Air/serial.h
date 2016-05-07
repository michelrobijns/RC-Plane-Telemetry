/*
 * serial.h
 *
 * Author: Michel Robijns
 * 
 * This file is part of avr-telemetry which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#define MESSAGE_SIZE 15

//extern char rxBuffer[MESSAGE_SIZE];
extern char txBuffer[MESSAGE_SIZE];

void setupSerial(void);
void sendTxBuffer(void);

#endif
