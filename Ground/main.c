/*
 * main.c
 *
 * Author: Michel Robijns
 *
 * This file is part of RC-Plane-Telemetry which is released under the MIT
 * license. See the file LICENSE or go to http://opensource.org/licenses/MIT
 * for full license details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "uart.h"
#include "math.h"

// Frequency of loop iterations in Herz
#define FREQ_SERIAL_READ    1000
#define FREQ_SERIAL_WRITE   50
#define FREQ_TERMINAL_WRITE 5

// Constants for computing the battery voltage from the received ADC value
#define VCC     4.994
#define DIVISOR 3.083

void *serialReader(void *argument);
void *serialWriter(void *argument);
void *terminalWriter(void *argument);
void getVoltage();
void getCurrent();
void getPressure();
void getRSSI();

double voltage = 0;
double current = 0;
int32_t pressure = 0;
int RSSI[4] = {0, 0, 0, 0};
int altitudeTotal = 0;
int altitudeSetToZero = 0;
int altitudeOffset = 0;
int samples = 0;

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
        // Collect the received data
        readBytes(&serialPort);
        
        usleep(1000000 / FREQ_SERIAL_READ);
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
        getVoltage();
        getCurrent();
        getPressure();
        getRSSI();
        
        float temp;
        int altitude;
    
        temp = (float) pressure / 101325;
        temp = 1 - pow(temp, 0.19029);
        altitude = round(44330 * temp * 10) - altitudeOffset;
        
        if (!altitudeSetToZero)
        {
            samples++;
            
            if (samples > 10) {
                altitudeTotal += altitude;
            }
            
            if (samples == 110) {
                altitudeOffset = altitudeTotal / 100;
                altitudeSetToZero = 1;
            }
        }

        // Print terminal output
        printf("Volts: %5.2f V     ", voltage);
        printf("Amps: % 4.1f A     ", current);
        printf("Power: % 3.0f W     ", voltage * current);
        printf("Altitude: % 4d m     ", (int) round(altitude / 10.0));
        printf("Pressure: % d     ", pressure);
        printf("L/R RSSI: %3d/%3d     ", RSSI[0], RSSI[1]);
        printf("L/R noise: %3d/%3d", RSSI[2], RSSI[3]);
        printf("\r");
        
        fflush(stdout);
        
        usleep(1000000 / FREQ_TERMINAL_WRITE);
    }
    
    pthread_exit(0);
}

void getVoltage()
{
    uint8_t ADCValue2 = (serialPort.bufferRX[0] >> 3) & 0b00011111;
    uint8_t ADCValue1 = (serialPort.bufferRX[1] >> 3) & 0b00011111;
    uint16_t ADC = ADCValue1 | (ADCValue2 << 5);
    voltage = ADC / 1023.0 * VCC * DIVISOR;
}

void getCurrent()
{
    uint8_t ADCValue2 = (serialPort.bufferRX[2] >> 3) & 0b00011111;
    uint8_t ADCValue1 = (serialPort.bufferRX[3] >> 3) & 0b00011111;
    uint16_t ADC = ADCValue1 | (ADCValue2 << 5);
    current = ((.0049 * ADC) - 2.5) / 0.066;   
}

void getPressure()
{
    uint8_t ADCValue6 = (serialPort.bufferRX[4] >> 2) & 0b00111111;
    uint8_t ADCValue5 = (serialPort.bufferRX[5] >> 2) & 0b00111111;
    uint8_t ADCValue4 = (serialPort.bufferRX[6] >> 2) & 0b00111111;
    uint8_t ADCValue3 = (serialPort.bufferRX[7] >> 2) & 0b00111111;
    uint8_t ADCValue2 = (serialPort.bufferRX[8] >> 2) & 0b00111111;
    uint8_t ADCValue1 = (serialPort.bufferRX[9] >> 2) & 0b00000011;
    
    int32_t ADC = ADCValue1 | (ADCValue2 << 2) | (ADCValue3 << 8) | (ADCValue4 << 14) | (ADCValue5 << 20) | (ADCValue6 << 26);
    pressure = ADC;
}

void getRSSI()
{
    // L/R RSSI: 170/171  L/R noise: 56/50 pkts: 19  txe=0 rxe=7 stx=0 srx=0 ecc=34/29 temp=30 dco=0
    
    char c;
    int slashes = 0, spaces = 0;
    int localRSSI = 0, remoteRSSI = 0, localNoise = 0, remoteNoise = 0;
    
    for (int i = 0; i < 200; i++)
    {
        c = serialPort.bufferRSSI[i];
        
        if (c == '/')
            slashes++;
        
        if (c == ' ')
            spaces++;
        
        if (c != ' ' && c != '/') {         
            if (spaces == 2 && slashes == 1) {
                localRSSI = localRSSI * 10 + c - '0';
            } else if (spaces == 2 && slashes == 2) {
                remoteRSSI = remoteRSSI * 10 + c - '0';
            } else if (spaces == 6 && slashes == 3) {
                localNoise = localNoise * 10 + c - '0';
            } else if (spaces == 6 && slashes == 4) {
                remoteNoise = remoteNoise * 10 + c - '0';
            }
        }
    }
    
    RSSI[0] = localRSSI;
    RSSI[1] = remoteRSSI;
    RSSI[2] = localNoise;
    RSSI[3] = remoteNoise;
}
