#define DEBUG Serial
#define IR_OUT Serial1
#define MIDI_IN Serial2
#define PACKET_HEADER_TO_WILLIAM 'a'
#define PACKET_HEADER_FROM_WILLIAM 'A'
/*
  Digital Pin 3: button click (external) interrupt
*/

#include "d:/dev/avr/treasure-hunt-2013/colour-triplets.h"
#include "d:/dev/avr/treasure-hunt-2013/colour-triplets.c"
#include "d:/dev/avr/treasure-hunt-2013/midi/midi-vars.h"
#include "d:/dev/avr/treasure-hunt-2013/midi/midi-song.h"
#include "d:/dev/avr/treasure-hunt-2013/midi/midi-song.cpp"
#include "d:/dev/avr/treasure-hunt-2013/midi/midi-listener.h"
#include "d:/dev/avr/treasure-hunt-2013/midi/midi-listener.cpp"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/red-song.c"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/green-song.c"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/blue-song.c"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/yellow-song.c"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/orange-song.c"
#include "d:/dev/avr/treasure-hunt-2013/pianoforte/sounds/violet-song.c"

volatile bool packetChanged;
uint8_t outgoingPacketOverrideIndex = WHITE;

char outgoingPacket[5];
// input from midi keyboard
MidiListener midiListener(MIDI_IN, cb_noteOn, cb_noteOff);
// build song pointers array for songbank
MjMidi::Song * songPointers[] = { &red_song, &green_song, &blue_song, &yellow_song, &orange_song, &violet_song };
// build song bank
MjMidi::SongBank songBank(&songPointers[0], COLOUR_TRIPLET_N-1);

void initCarrierWave();
void setOutgoingPacket(uint8_t tripletIndex);

// declare callbacks for midi listener
void cb_noteOn(unsigned char note, unsigned char volume);
void cb_noteOff(unsigned char note);
// declare callbacks for song bank
void cb_songBankFailure();
// declare callbacks for individual songs
#define defineSongCompletionCallback(colour, COLOUR) void cb_##colour##SongComplete() { setOutgoingPacket(COLOUR); }
void cb_songBankFailure() { setOutgoingPacket(WHITE); }
defineSongCompletionCallback(red, RED)
defineSongCompletionCallback(green, GREEN)
defineSongCompletionCallback(blue, BLUE)
defineSongCompletionCallback(yellow, YELLOW)
defineSongCompletionCallback(orange, ORANGE)
defineSongCompletionCallback(violet, VIOLET)
void setupSongCallbacks();

/*** ***/
void setup()
{
  IR_OUT.begin(2400); // out to IR LED
  MIDI_IN.begin(MIDI_BAUDRATE); // in from MIDI
  DEBUG.begin(9600); // debugging output
  initCarrierWave();
  attachInterrupt(1, buttonClicked, FALLING); // PIN3
  setupSongCallbacks();
  #ifdef DEBUG
    DEBUG.println();
    DEBUG.print((int) songBank.n);
    DEBUG.println(" songs in songBank");
    for (int i=0; i < songBank.n; i ++) {
      DEBUG.print("\tsong ");
      DEBUG.println((int) i);
      DEBUG.print("\t\t");
      for (int j=0; j < songBank.pp_songs[i]->n; j++) {
        DEBUG.print((int) songBank.pp_songs[i]->p_events[j].note);
        DEBUG.print(" ");
      }
      DEBUG.println();
    }
  #endif
}

/*** ***/
void loop()
{
  /* Check MidiListener for data. If it is found, it passes the data
    to the SongBank, which shall do one of the following:
    - fire a songbank failure callback (transmitData)
    - fire a song success callback (transmitData)
    - register a note success for one or more songs but fire no callback
  */
  midiListener.poll();
  // transmit packet
  for (int i = 0; i < 5; i++)
    IR_OUT.write(outgoingPacket[i]);
  // update packet
  if (packetChanged)
    { packetChanged = false; setOutgoingPacket(outgoingPacketOverrideIndex); }
  // meaningless delay
  delay(10);
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
  DEBUG.println("setting Outgoing packet:");
  for (int i=0; i < 5; i++) {
    DEBUG.println((uint8_t) outgoingPacket[i]);
  }
}

void initCarrierWave()
{

}

void buttonClicked()
{
  #ifdef DEBUG
    DEBUG.println("!! button clicked !!");
  #endif
  outgoingPacketOverrideIndex += 1;
  if (outgoingPacketOverrideIndex >= COLOUR_TRIPLET_N)
    outgoingPacketOverrideIndex = 0;
  packetChanged = true;
}

/* Send data to song bank */
void cb_noteOn( unsigned char note, unsigned char volume ) { songBank.handleMidiEvent(MIDI_NOTE_ON, note); }

/* Send data to song bank */
void cb_noteOff( unsigned char note ) { songBank.handleMidiEvent(MIDI_NOTE_OFF, note); }

/* Set callbacks for songbank and songs */
void setupSongCallbacks()
{
  /* set callbacks for songbank */
  songBank.setCallbacks( NULL, cb_songBankFailure );
  /* set callbacks for individual songs */
  red_song.setCallbacks( cb_redSongComplete, NULL );
  green_song.setCallbacks( cb_greenSongComplete, NULL );
  blue_song.setCallbacks( cb_blueSongComplete, NULL );
  yellow_song.setCallbacks( cb_yellowSongComplete, NULL );
  orange_song.setCallbacks( cb_orangeSongComplete, NULL );
  violet_song.setCallbacks( cb_violetSongComplete, NULL );
}