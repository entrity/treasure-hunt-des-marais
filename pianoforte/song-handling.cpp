#include "song-handling.h"
#include <midi-listener.h>
#include <midi-song.h>
#include "main.h"
#include <Arduino.h>

#include "../colour-triplets.h"
#include "sounds/red-song.c"
#include "sounds/green-song.c"
#include "sounds/blue-song.c"
#include "sounds/yellow-song.c"
#include "sounds/orange-song.c"
#include "sounds/violet-song.c"

// build song pointers array for songbank
MjMidi::Song * songPointers[] = { &red_song, &green_song, &blue_song, &yellow_song, &orange_song, &violet_song };

// build song bank
MjMidi::SongBank songBank(&songPointers[0], COLOUR_TRIPLET_N-1);

/* Set callbacks for songbank and songs */
void setupSongHandling()
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

/* Send data to song bank */
void cb_noteOn( unsigned char note, unsigned char volume ) { songBank.handleMidiEvent(MIDI_NOTE_ON, note); }
/* Send data to song bank */
void cb_noteOff( unsigned char note ) { songBank.handleMidiEvent(MIDI_NOTE_OFF, note); }

void cb_songBankFailure() { setOutgoingPacket(WHITE); }

#define defineSongCompletionCallback(colour, COLOUR) \
  void cb_##colour##SongComplete() \
   { setOutgoingPacket(COLOUR); }

defineSongCompletionCallback(red, RED)
defineSongCompletionCallback(green, GREEN)
defineSongCompletionCallback(blue, BLUE)
defineSongCompletionCallback(yellow, YELLOW)
defineSongCompletionCallback(orange, ORANGE)
defineSongCompletionCallback(violet, VIOLET)
