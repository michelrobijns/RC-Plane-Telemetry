#
# Makefile
#
# Author: Michel Robijns
#
# This file is part of RC-Plane-Telemetry which is released under the MIT
# license. See the file LICENSE or go to http://opensource.org/licenses/MIT
# for full license details.
#

target = main
dependencies = timers.h uart.h adc.h i2c.h bmp.h
objects = main.o timers.o uart.o adc.o i2c.o bmp.o

microcontroller = 328p
programmerType = arduino
programmerPort = /dev/ttyACM0

cc = /usr/bin/avr-gcc
cFlags = -g -mmcu=atmega$(microcontroller) -Wall -Os -Werror -Wextra -std=gnu99

objectToHex = /usr/bin/avr-objcopy
programmer = /usr/bin/avrdude

all: flash

%.o: %.c $(dependencies)
	$(cc) $(cFlags) -c -o $@ $<

$(target): $(objects)
	$(cc) $(cFlags) -o $(target).elf $^

hex: $(target)
	$(objectToHex) -j .text -j .data -O ihex $(target).elf $(target).hex

flash: hex
	$(programmer) -p m$(microcontroller) -c $(programmerType) -P $(programmerPort) -e -U flash:w:$(target).hex

clean:
	rm -f *.elf *.hex *.obj *.o
