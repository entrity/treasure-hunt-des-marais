#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "../2013-common.h"
#include "../colour-triplets.h"
#include "song-handling.h"
#include "main.h"
#include <midi-listener.h>
#include "../init-ir-modulator.h"

SoftwareSerial mySerial(2,NULL); // input from midi keyboard
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff); // input from midi keyboard
uint8_t outgoingPacket[5];
void buttonClicked();
uint8_t outgoingPacketOverrideIndex = WHITE;

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
  // remember 10ms delay between bytes? packets?
  transmitPacket();
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
    Serial.println((int) outgoingPacket[i]);
  Serial.println("===============");
#endif
  // set led
  clearStatusLeds();
  setStatusLed(tripletIndex);
}

void transmitPacket()
{
  for (int i = 0; i < 5; i++)
#ifdef _DEBUG
    Serial.println((int) outgoingPacket[i]);
  Serial.println("===============");
#else
    Serial.write(outgoingPacket[i]);
#endif
}

void buttonClicked()
{
  outgoingPacketOverrideIndex += 1;
  if (outgoingPacketOverrideIndex >= COLOUR_TRIPLET_N)
    outgoingPacketOverrideIndex = 0;
  setOutgoingPacket(outgoingPacketOverrideIndex);
}