TARGET = main

# UDIR = /d/dev/avr/treasure-hunt-2013/usart

SRC = \
	../usart/usart-functions.c \
	../usart/usart-functions-atmega328.c \
	../colour-triplets.c \
	main.c

EXTRAINCDIRS = .. ../usart

NO_ARDUINO = 1

include /d/dev/avr/treasure-hunt-2013/MakefileBase.makefile

# main.elf: $(OBJ)
# 	@echo $(MSG_LINKING) $@
# 	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)