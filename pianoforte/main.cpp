#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "../2013-common.h"
#include "song-handling.h"
#include <midi-listener.h>

SoftwareSerial mySerial(2,3); // input from midi keyboard
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff); // input from midi keyboard

void setup()
{
  /* start hw & sw serial */
  Serial.begin(WILLIAM_BAUDRATE); // output
  mySerial.begin(MIDI_BAUDRATE); // input
  /* set up song and songbank callbacks */
  setupSongHandling();
  /* setup status leds for debugging */
  DDRC = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);
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
  Sends a packet to William. Remember 10ms delay between bytes? */
void transmitData(char * p_triplet)
{

}