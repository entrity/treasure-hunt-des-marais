#ifndef USART_FUNCTIONS_H
#define USART_FUNCTIONS_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

#define UBRR_VALUE 416 // baudrate 2400
#define PACKET_LENGTH 4 // header, plus 3 data bytes
#define USART_IN_READY (UCSR0A & (1<<RXC0))
#define USART_OUT_READY (UCSR0A & (1<<UDRE0))

typedef struct {
  uint8_t body[PACKET_LENGTH];
  uint8_t i;
} packet_t;

static bool b_deferUsartRx = false;

/* Set up UART transmission (TXD|PD1) */
void init_usart();

/* Read a character from USART */
char usartIn();

/* Read a character from USART without checking to see if a char is avilable */
char usartInWithoutCheck();

/* Write a character to USART */
void usartOut(char data);

bool packetMatchesColour(packet_t * packet, char * colourTriplet);

bool usartInToPacket(packet_t * p_packet, char packetHeader, bool skipCheck);
void usartPacketOut(packet_t * p_packet);

#endif
