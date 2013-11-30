/*
  
  === ATMEGA328 I/O ===
  PB0: status LED 7 (WHITE)
  PB1: IR LED + (carrier wave)
  PB2: status LED 1 (YELLOW)
  PB3: status LED 2 (BLUE)
  PB4: status LED 3 (GREEN)
  PB5: status LED 4 (RED)
  PB6: 16MHz crystal
  PB7: 16MHz crystal
  PC0: 
  PC1: 
  PC2: 
  PC3: 
  PC4: 
  PC5: 
  PC6: RESET
  PD0: 
  PD1: IR LED - (TX)
  PD2: LTV 817C Optoisolator cathode (MIDI in)
  PD3: push button to +5V for manual colour change
  PD4: 
  PD5: status LED 5 (ORANGE)
  PD6: status LED 6 (VIOLET)
  PD7: 

*/


#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "../2013-common.h"
extern "C" {
  #include "../colour-triplets.h"
  #include "../colour-triplets.c"
}
#include "song-handling.h"
#include "main.h"
#include <midi-listener.h>
#include "../init-ir-modulator.h"

SoftwareSerial mySerial(2,NULL); // input from midi keyboard
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff); // input from midi keyboard
char outgoingPacket[5];
void buttonClicked();
uint8_t outgoingPacketOverrideIndex = WHITE;
bool packetChanged = false;

void setup()
{
  /* button click interrupt */
  attachInterrupt(1, buttonClicked, FALLING);
  /* start hw & sw serial */
  Serial.begin(WILLIAM_BAUDRATE); // output
  mySerial.begin(MIDI_BAUDRATE); // input
  /* set up song and songbank callbacks */
  setupSongHandling();
  /* set initial outgoing packet (to William) */
  setOutgoingPacket(WHITE);
  /* setup status leds for debugging */
  DDRB |= (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5);
  DDRD |= (1<<5)|(1<<6);
  /* start 38KHz carrier wave for infrared TX */
  init_ir_modulator();
#ifdef _DEBUG
  for (int i=0; i < COLOUR_TRIPLET_N; i++){
    char * p_triplet = colourTriplets[i];
    for (int j=0; j < 3; j++)
      Serial.write(p_triplet[j]);
    Serial.write(9);
  }
#endif
}

void loop()
{
  /* Check MidiListener for data. If it is found, it passes the data
  to the SongBank, which shall do one of the following:
    - fire a songbank failure callback (transmitData)
    - fire a song success callback (transmitData)
    - register a note success for one or more songs but fire no callback
    */
  midiListener.poll();
  /* handle change of packet in case of button interrupt */
  if (packetChanged) {
    packetChanged = false;
    setOutgoingPacket(outgoingPacketOverrideIndex);
  }
  // remember 10ms delay between bytes? packets?
  transmitPacket();
  _delay_ms(80);
}

/* Called by callbacks for songbank failure and song success.
  Sends a packet to William. */
void setOutgoingPacket(uint8_t tripletIndex)
{
  char * p_triplet = colourTriplets[tripletIndex];
  outgoingPacket[0] = PACKET_HEADER_TO_WILLIAM;
  outgoingPacket[1] = p_triplet[0];
  outgoingPacket[2] = p_triplet[1];
  outgoingPacket[3] = p_triplet[2];
  outgoingPacket[4] = 0;
  for (int i = 0; i < 4; i++)
    outgoingPacket[4] += outgoingPacket[i];
  /* diagnostic */
#ifdef _DEBUG
  for (int i = 0; i < 5; i++)
    Serial.write(outgoingPacket[i]);
  Serial.write(7);
  for (int i = 0; i < 3; i++)
    Serial.write(p_triplet[i]);
  Serial.write(88);
#endif
  // set led
  clearStatusLeds();
  setStatusLed(tripletIndex);
}

void transmitPacket()
{
  for (int i = 0; i < 5; i++)
    Serial.write(outgoingPacket[i]);
}

void buttonClicked()
{
  outgoingPacketOverrideIndex += 1;
  if (outgoingPacketOverrideIndex >= COLOUR_TRIPLET_N)
    outgoingPacketOverrideIndex = 0;
  packetChanged = true;
}