/*
 * uart.h
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

void uartSetup(void);
void uartSendTxBuffer(void);
void uartWriteTxBuffer(uint8_t location, uint8_t data);

#endif
