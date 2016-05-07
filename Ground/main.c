/*
 * main.c
 *
 * Author: Michel Robijns
 *
 * This file is part of avr-telemetry which is released under the MIT license.
 * See the file LICENSE or go to http://opensource.org/licenses/MIT for full
 * license details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "serial.h"

// Update frequencies
#define FREQ_SERIAL_READ 1000
#define FREQ_SERIAL_WRITE 50
#define FREQ_TERMINAL_WRITE 10

// Constants for computing the battery voltage from the received ADC value
#define VCC 4.994
#define DIVISOR 3.083

void *serialReader(void *argument);
void *serialWriter(void *argument);
void *terminalWriter(void *argument);

double voltage = 0;
double current = 0;
uint16_t ADC = 0;

struct serialPort serialPort;

int main(void)
{
    serialPort = openSerial();
    
    printf("Serial port's file descriptor: %d\n\r", serialPort.fd);
    
    // Launch threads
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t tids[3];
    
    pthread_create(&tids[0], &attr, serialReader, NULL);
    pthread_create(&tids[1], &attr, serialWriter, NULL);
    pthread_create(&tids[2], &attr, terminalWriter, NULL);
    
    for (int i = 0; i < 3; i++)
    {
        pthread_join(tids[i], NULL);
    }
    
    // Never reached because the threads run forever
    closeSerial(&serialPort);
    
    return 0;
}

void* serialReader(__attribute__ ((unused)) void *argument)
{
    while (1)
    {
        readBytes(&serialPort);
        usleep(1000000 / FREQ_SERIAL_READ);
        
        // Compute battery voltage
        uint8_t ADCValue2 = (serialPort.bufferRX[3] >> 3) & 0b00011111;
        uint8_t ADCValue1 = (serialPort.bufferRX[4] >> 3) & 0b00011111;
        ADC = ADCValue1 | (ADCValue2 << 5);
        voltage = ADC / 1023.0 * VCC * DIVISOR;
        
        // Compute current reported by the 30A ACS 712 sensor
        ADCValue2 = (serialPort.bufferRX[5] >> 3) & 0b00011111;
        ADCValue1 = (serialPort.bufferRX[6] >> 3) & 0b00011111;
        ADC = ADCValue1 | (ADCValue2 << 5);
        current = ((.0049 * ADC) - 2.5) / 0.066;
    }
    
    pthread_exit(0);
}

void* serialWriter(__attribute__ ((unused)) void *argument)
{
    while (1)
    {
        // There is nothing to send right now
        
        usleep(1000000 / FREQ_SERIAL_WRITE);
    }
    
    pthread_exit(0);
}

void* terminalWriter(__attribute__ ((unused)) void *argument)
{
    while (1)
    {
        printf("Bat volt: %2.2f V          ", voltage);
        printf("Amps: % -2.2f A          ", current);
        printf("Power: % 3.0f W", voltage * current);
        printf("\r");
        fflush(stdout);
        
        usleep(1000000 / FREQ_TERMINAL_WRITE);
    }
    
    pthread_exit(0);
}
