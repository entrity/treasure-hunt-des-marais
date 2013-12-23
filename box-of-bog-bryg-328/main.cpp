/*
  PORTB 3:0 controls 4 LEDs
  PORTD 7:4 controls 3 LEDs and the solenoid
  PORTD 2 (Digital Pin 2) receives IR input from the TSOP32
*/

// #define DEBUG

#define F_CPU 16000000L
#define SERIAL_RX_PIN 2
#define SERIAL_TX_PIN 3
#define REQUIRED_COLOURS_N 7
#define COLOUR_TRIPLET_SIZE 3

#include <Arduino.h>

extern "C" {
  #include "usart-functions.h"
}
#include "colour-triplets.h"
#include "../2013-common.h"
#include <util/delay.h>
#include <avr/io.h>

/* Settings for serial RX  */
packet_t rx_packet;
char usartIn() { return Serial.read(); }
char usartInWithoutCheck() { return Serial.read(); }

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
inline void printPacket() // for debugging
{
  for (int i; i < PACKET_LENGTH; i++) Serial.println((uint8_t) rx_packet.body[i]);
}
inline void printColour(char * colour) // for debugging
{
  for (int i; i < 3; i ++) Serial.println((uint8_t) colour[i]);
}

void setup()
{
  DDRB = 0x0f | (1<<PB5);
  DDRD = 0xf0;
  Serial.begin(WILLIAM_BAUDRATE);
  Serial.println("+");
  printColour(requiredColours[packetMatches_i]);
}

void loop()
{
  /* read serial, handle packet (if any) */
  while (Serial.available()) {
    if (usartInToPacket(&rx_packet, PACKET_HEADER_FROM_WILLIAM, true))
      { 
        Serial.print('.');
        handlePacket();
      }
  }
}

/*  */
void handlePacket()
{
  #ifdef DEBUG
    // Serial.write((uint8_t) 99);
    // printPacket();
  #endif
  // Handle case that packet received is the packet expected
  if ( packetMatchesColour(&rx_packet, requiredColours[packetMatches_i]) ) {
    #ifdef DEBUG
      Serial.println("*");
      printColour(requiredColours[packetMatches_i]);
    #endif
    if (packetMatches_i < 4)
      PORTB |= (1 << packetMatches_i);
    else
      PORTD |= (1 << packetMatches_i);
    packetMatches_i += 1;
  }
  else {
    #ifdef DEBUG
    #endif 
  }

  // activate solenoid if the final colour triplet has been received
  if ( packetMatches_i == REQUIRED_COLOURS_N ) {
    PORTB |= (1<<PB5);
    _delay_ms(1000 * 20); // hold solenoid for 10 sec
    PORTB &= 0xf0; // deactivate all LEDs and solenoid
    PORTD &= 0x0f; // deactivate all LEDs and solenoid
    packetMatches_i = 0; // reset counter
  }
}