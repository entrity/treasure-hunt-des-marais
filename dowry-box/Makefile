MCU = attiny85
# clock fuse: internal 8MHz osc
LFUSE = 0x62
F_CPU = 8000000

TARGET = main

PSRC = \
	main.cpp

SRC = 

NO_ARDUINO = 1

EXTRAINCDIRS = attiny85

CFLAGS = -D NO_ARDUINO

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile

test: main.cpp test.cpp
	g++ -o a.out test.cpp
	./a.out
