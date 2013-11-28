#ifndef MIDI_SONG_H
#define MIDI_SONG_H

/*
  The MidiSong holds many MidiInstants, which each hold many MidiActions.
  Each of these three models has a field for completion, to indicate that the
  action, instant, or song has been completed.
*/

#include <Arduino.h>
#include "midi-vars.h"

namespace MjMidi
{
  /* A Midi event */
  typedef struct {
    uint16_t offset;
    uint8_t
      action,
      note;
    bool complete;
  } event;

  /* A collection of Midi events */
  class Song
  {
    event * p_events;
    unsigned short i;
    unsigned short n;
    bool completed;
    void(*completionCallback)(void);
    void(*failureCallback)(void);
    void handleCompletion();
  public:
    /* constructor */
    Song(event * p_events, unsigned short n_events)
      : p_events(p_events), i((unsigned short) 0), n(n_events), completed(false) {};
    /* other methods */
    bool attempt(uint8_t action, uint8_t note);
    bool isCompleted() {return completed;}
    void reset();
    void setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void));
  };

  inline void Song::handleCompletion()
  {
    completed = true;
    reset();
    if (completionCallback) completionCallback();
  }

  /* A collection of songs */
  class SongBank
  {
    Song ** pp_songs;
    unsigned short n;
    void(*completionCallback)(void); // fired when all songs in the bank are completed
    void(*failureCallback)(void); // fired when a midi event fails to match any song in the bank
  public:
    /* constructor */
    SongBank(Song ** pp_songs, unsigned short n_songs)
      : pp_songs(pp_songs), n(n_songs) {};
    /* call attempt on all the songs in this bank */
    void handleMidiEvent(uint8_t action, uint8_t note);
    /* other methods */
    void setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void));
  };
}

#endif