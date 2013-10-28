/*
  PORTB 3:0 controls 4 LEDs
  PORTD 7:4 controls 3 LEDs and the solenoid
  PORTD 0 (Digital Pin 2) receives IR input from the TSOP32
*/

#define F_CPU 16000000L
#define SERIAL_RX_PIN 2
#define SERIAL_TX_PIN 3
#define REQUIRED_COLOURS_N 7
#define COLOUR_TRIPLET_SIZE 3

#include <Arduino.h>
#include <SoftwareSerial.h>

extern "C" {
  #include "usart-functions.h"
}
#include "colour-triplets.h"
#include "../2013-common.h"
#include <util/delay.h>
#include <avr/io.h>

/* Settings for serial RX  */
SoftwareSerial swSerial(SERIAL_RX_PIN, SERIAL_TX_PIN);
packet_t rx_packet;
char usartIn() { return swSerial.read(); }
char usartInWithoutCheck() { return swSerial.read(); }

/* Settings for colour expectations */
uint8_t packetMatches_i = 0;
char * requiredColours[] = {
  blueTriplet,
  orangeTriplet,
  greenTriplet,
  blueTriplet,
  redTriplet,
  yellowTriplet,
  greenTriplet
};

/* function definitions */
void handlePacket();
bool coloursMatch(char * a, char * b);

void setup()
{
  DDRB = 0x0f;
  DDRD = 0xf0;
  DDRD &= ~(1<<PD0);
  DDRD |= (1<<PD1)|(1<<PD3);
  swSerial.begin(WILLIAM_BAUDRATE);
}

void loop()
{
  /* read serial, handle packet (if any) */
  while (swSerial.available()) {
    if (usartInToPacket(&rx_packet, PACKET_HEADER_FROM_WILLIAM, true))
      { handlePacket(); }
  }
}

/*  */
void handlePacket()
{
  // Handle case that packet received is the packet expected
  if ( packetMatchesColour(&rx_packet, requiredColours[packetMatches_i]) ){
    packetMatches_i += 1;
    if (packetMatches_i < 5)
      PORTB |= (1 << packetMatches_i);
    else
      PORTD |= (1 << packetMatches_i);
  } else // packet received was not expected. reset
    { packetMatches_i = 0; return; }
  // activate solenoid if the final colour triplet has been received
  if ( packetMatches_i == REQUIRED_COLOURS_N ) {
    _delay_ms(1000 * 10); // hold solenoid for 10 sec
    PORTB &= 0xf0; // deactivate all LEDs and solenoid
    PORTD &= 0x0f; // deactivate all LEDs and solenoid
    packetMatches_i = 0; // reset counter
  }
}