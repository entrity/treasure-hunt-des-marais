TARGET = main

# Uncomment this line to use no Arduino files
# NO_ARDUINO = 1

MIDI_DIR = /d/dev/avr/treasure-hunt-2013/midi

EXTRAINCDIRS = $(MIDI_DIR)

PSRC = \
	$(MIDI_DIR)/midi-listener.cpp \
	$(MIDI_DIR)/midi-song.cpp \
	song-handling.cpp \
	main.cpp

SRC = \

ARDLIBS = SoftwareSerial

# uncomment this line to have debug data sent to Serial out
# CFLAGS = -D_DEBUG=1

# LDFLAGS = -v 

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile
