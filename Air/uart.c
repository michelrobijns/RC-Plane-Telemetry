/*
 * uart.c
 *
 * Author: Michel Robijns
 * 
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#define F_CPU 16000000UL
#define BAUD 19200UL
#define BRC ((F_CPU / 16 / BAUD) - 1)
#define MESSAGE_SIZE 15

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

static volatile uint8_t txBuffer[MESSAGE_SIZE] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, '\0'};
static volatile uint8_t rxWritePos = 0;
static volatile uint8_t txReadPos = 0;
static volatile uint8_t txWritePos = 0;

void uartSetup(void)
{
    // Set baud rate
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;
    
    // Enable the USART Receiver and Transmitter
    UCSR0B |= (1 << TXEN0);
    
    // Enable the RX Complete Interrupt and the TX Complete Interrupt
    UCSR0B |= (1 << TXCIE0);
    
    // Set the Character Size to 8 bits. There is one stop bit, which is
    // the default setting.
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

// TX Complete Interrupt
ISR(USART_TX_vect)
{
    if (txReadPos != txWritePos)
    {
        UDR0 = txBuffer[txReadPos];
        txReadPos++;
        
        if (txReadPos > MESSAGE_SIZE)
        {
            txReadPos = 0;
        }
    } else {
        txReadPos = 0;
    }
}

void uartSendTxBuffer(void)
{
    if (UCSR0A & (1 << UDRE0))
    {
        UDR0 = txBuffer[0];
        
        txWritePos = MESSAGE_SIZE;
        txReadPos = 1;
    }
}

void uartWriteTxBuffer(uint8_t location, uint8_t data)
{
    txBuffer[location] = data;
}
