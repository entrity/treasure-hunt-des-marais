MCU = attiny85

# clock fuse: 8 MHz internal osc
LFUSE=0x62
F_CPU = 8000000

PSRC = main.cpp morse_codes.cpp strings.cpp

SRC = 

CFLAGS = 

EXTRAINCDIRS = 

NO_ARDUINO = 1
ARDLIBS = 

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile

test: test.cpp strings.cpp morse_codes.cpp
	g++ test.cpp strings.cpp morse_codes.cpp -D _USING_GPP -o a.out
	./a.out
