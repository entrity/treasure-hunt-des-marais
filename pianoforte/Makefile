TARGET = main
# NO_ARDUINO = 1

MIDI_DIR = /c/MinGW/msys/1.0/home/MJ/Arduino/libraries/mj_midi

PSRC = main.cpp \
	$(MIDI_DIR)/midi-listener.cpp \
	$(MIDI_DIR)/midi-song.cpp \
	song-handling.cpp \
	huart.cpp

SRC = ../usart/usart-functions-atmega328.c \
	../usart/usart-functions.c

ARDLIBS = SoftwareSerial

EXTRAINCDIRS = $(MIDI_DIR)

# LDFLAGS = -v 

include /d/dev/avr/treasure-hunt-2013/Makefile.base
