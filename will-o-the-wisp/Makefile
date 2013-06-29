TARGET = main

# UDIR = /d/dev/avr/treasure-hunt-2013/usart

SRC = main.c ../usart/usart-functions.c ../usart/usart-functions-atmega328.c
EXTRAINCDIRS = .. ../usart

NO_ARDUINO = 1

include /d/dev/avr/treasure-hunt-2013/Makefile.base

# main.elf: $(OBJ)
# 	@echo $(MSG_LINKING) $@
# 	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)