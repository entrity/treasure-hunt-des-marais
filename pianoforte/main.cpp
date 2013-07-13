#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "../2013-common.h"
#include "song-handling.h"

SoftwareSerial mySerial(2,3); // input to midiListener
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff);

void setup()
{
  /* start hw & sw serial */
  Serial.begin(2400); // output
  mySerial.begin(MIDI_BAUDRATE); // input
  /* set up song and songbank callbacks */
  setupSongHandling();
  /* setup status leds for debugging */
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
}

/* Called by callbacks for songbank failure and song success.
  Sends a packet to William. */
void transmitData(char * p_triplet)
{

}