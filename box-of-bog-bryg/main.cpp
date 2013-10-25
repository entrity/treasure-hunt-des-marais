/*
  PORTA 7:0 controls the 7 LEDs and the solenoid
  PORTB 2 (Digital Pin 8) receives IR input from the TSOP32
*/

#define F_CPU 16000000L
#define SERIAL_RX_PIN 8
#define REQUIRED_COLOURS_N 7
#define COLOUR_TRIPLET_SIZE 3

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
SoftwareSerial swSerial(SERIAL_RX_PIN, NULL);
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

/* function definitions */
void handlePacket();
bool coloursMatch(char * a, char * b);

void setup()
{
  DDRA = 0xff;
  // PORTA = 0b10101010;
  swSerial.begin(WILLIAM_BAUDRATE);
}

void loop()
{
    // PORTA = 0xff;
    // _delay_ms(1000);
    // PORTA = 0x0;
    // _delay_ms(1000);


  /* read serial, handle packet (if any) */
  // while (swSerial.available()) {
  //   PORTA = 0b11110000;
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
    PORTA = 0; // deactivate all LEDs and solendoid\
    requiredColours_i = 0; // reset counter
  }
}