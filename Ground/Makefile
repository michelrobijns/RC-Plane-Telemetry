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
dependencies = uart.h
objects = main.o uart.o

cc = /usr/bin/gcc
cFlags = -I -g -Wall -Os -Wextra -std=gnu99 -pthread -lm

all: $(target)

%.o: %.c $(dependencies)
	$(cc) $(cFlags) -c -o $@ $<

$(target): $(objects)
	$(cc) $(cFlags) -o $@ $^

clean:
	rm -f $(target) *.o

