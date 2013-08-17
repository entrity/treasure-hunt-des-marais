#include <Arduino.h>
#include <SoftwareSerial.h>

extern "C" {
  #include "usart-functions.h"
}
#include "colour-triplets.h"
#include "../2013-common.h"
#include <util/delay.h>
#include <avr/io.h>

/* pin labels */
enum portA_pin {
  PORTA_PIN_B,
  PORTA_PIN_O,
  PORTA_PIN_G,
  PORTA_PIN_B2,
  PORTA_PIN_R,
  PORTA_PIN_Y,
  PORTA_PIN_G2,
  PORTA_PIN_SOLENOID
};

/* Settings for serial RX  */
SoftwareSerial swSerial(5,NULL);
packet_t rx_packet;
char usartIn() { return swSerial.read(); }
char usartInWithoutCheck() { return swSerial.read(); }

/* Settings for colour expectations */
uint8_t requiredColours_i = 0;
char * requiredColours[] = {
  blueTriplet,
  orangeTriplet,
  greenTriplet,
  blueTriplet,
  redTriplet,
  yellowTriplet,
  greenTriplet
};
#define REQUIRED_COLOURS_N 7
#define COLOUR_TRIPLET_SIZE 3

/* function definitions */
void handlePacket();
bool coloursMatch(char * a, char * b);

void setup()
{
  // DDRA |= (1<< PA3);
  // PORTA |= (1<< PA3);
  // DDRB |= (1<<PB0);
  // PORTB |= (1<<PB0);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void loop()
{
  digitalWrite(10, HIGH);
  /* read serial, handle packet (if any) */
  // while (swSerial.available()) {
  //   if (usartInToPacket(&rx_packet, PACKET_HEADER_FROM_WILLIAM, true))
  //     { handlePacket(); }
  // }
}

/*  */
void handlePacket()
{
  // Handle case that packet received is the packet expected
  if ( packetMatchesColour(&rx_packet, requiredColours[requiredColours_i]) ){
    PORTA |= (1 << requiredColours_i);
    requiredColours_i += 1;
  } else // packet received was not expected. reset
    { requiredColours_i = 0;}
  // activate solenoid if the final colour triplet has been received
  if ( requiredColours_i == REQUIRED_COLOURS_N ) {
    PORTA |= (1 << PORTA_PIN_SOLENOID); // activate solenoid
    _delay_ms(1000 * 10); // hold solenoid for 10 sec
    PORTA = 0; // deactivate all LEDs and solendoid
  }
}