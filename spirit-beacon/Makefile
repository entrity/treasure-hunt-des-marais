MCU = attiny85

# clock fuse: 16 mhz crystal
LFUSE=0xff

PSRC = main.cpp

SRC = ../colour-triplets.c ../usart/usart-functions.c

CFLAGS = -DCOLOUR_TRIPLET=redTriplet

EXTRAINCDIRS = .. ../usart

ARDLIBS = SoftwareSerial

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile
