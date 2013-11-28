TARGET = main

# UDIR = /d/dev/avr/treasure-hunt-2013/usart

SRC = \
	../usart/usart-functions.c \
	../usart/usart-functions-atmega328.c \
	../colour-triplets.c \
	main.c

PSRC = 

EXTRAINCDIRS = .. ../usart

NO_ARDUINO = 1

# clock fuse: 16MHz external crystal
LFUSE = 0xff

# uncomment this line to have debug data sent to Serial out
# CFLAGS = -DDEBUG=1

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile

# main.elf: $(OBJ)
# 	@echo $(MSG_LINKING) $@
# 	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)