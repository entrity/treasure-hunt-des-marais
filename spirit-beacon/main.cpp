#include "2013-common.h"

extern "C" {
#include "usart-functions.h"
}
#include "colour-triplets.h"
#include <util/delay.h>
#include <avr/io.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define blinker (1<<0)

SoftwareSerial mySerial(1,NULL);
packet_t rx_packet;

char usartIn() { return mySerial.read(); }

char usartInWithoutCheck() { return mySerial.read(); }

inline void blink()
{
  // disable interrupts
  cli();
  // blink led
  for (int i = 0; i < 10; i++) {
    PORTB |= blinker;
    _delay_ms(250);
    PORTB &= ~blinker;
    _delay_ms(250); 
  }
  // enable interrupts
  sei();
}

/* Tries to read from sw serial. Return true if complete, valid packet read. */
inline bool poll()
{    
  while (mySerial.available()) {
    if (usartInToPacket(&rx_packet, PACKET_HEADER_FROM_WILLIAM, true))
      return true;
  }
  return false;
}

// return true if packet body matches the given colour triplet
// Define the COLOUR_TRIPLET in the Makefile. (see ../colour-triplets.h for options)
// E.g. COLOUR_TRIPLET = redTriplet;
inline bool packetMatch() {
  for (int i = 1; i < 4; i++) {
    if (rx_packet.body[i] != COLOUR_TRIPLET[i-1])
      return false;
  }
  return true;
}

void setup()
{
  // set led pin to output
  DDRB |= blinker;
  PORTB &= ~blinker;
  // start sw serial
  mySerial.begin(WILLIAM_BAUDRATE);
}

void loop()
{
  // poll SoftwareSerial
  if (poll()) {
    // if packet payload is a match: blink
    if (packetMatch()) {
      blink();
    }
  }
}