#include "song-handling.h"
#include "midi-listener.h"
#include "midi-song.h"
#include "main.h"

#include "../colour-triplets.h"
#include "sounds/red-song.c"
#include "sounds/green-song.c"
#include "sounds/blue-song.c"
#include "sounds/yellow-song.c"
#include "sounds/orange-song.c"

// build song pointers array for songbank
MjMidi::Song * songPointers[] = { &red_song, &green_song, &blue_song, &yellow_song, &orange_song };

// build song bank
MjMidi::SongBank songBank(&songPointers[0], SONG_COUNT);

void setupSongHandling()
{
  /* set callbacks for individual songs */
  red_song.setCallbacks( cb_redSongComplete, NULL );
  green_song.setCallbacks( cb_greenSongComplete, NULL );
  blue_song.setCallbacks( cb_blueSongComplete, NULL );
  yellow_song.setCallbacks( cb_yellowSongComplete, NULL );
  orange_song.setCallbacks( cb_orangeSongComplete, NULL );
}

/* Send data to song bank */
void cb_noteOn( unsigned char note, unsigned char volume ) { songBank.handleMidiEvent(MIDI_NOTE_ON, note); }
/* Send data to song bank */
void cb_noteOff( unsigned char note ) { songBank.handleMidiEvent(MIDI_NOTE_OFF, note); }

void cb_songBankFailure() { transmitData(whiteTriplet); }

void cb_redSongComplete() { transmitData(redTriplet); }
void cb_greenSongComplete() { transmitData(greenTriplet); }
void cb_blueSongComplete() { transmitData(blueTriplet); }
void cb_yellowSongComplete() { transmitData(yellowTriplet); }
void cb_orangeSongComplete() { transmitData(orangeTriplet); }
