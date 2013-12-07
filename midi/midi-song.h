#ifndef MIDI_SONG_H
#define MIDI_SONG_H
#define MAX_MIDI_EVENTS_PER_INSTANT (32)
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
  public:
    event * p_events;
    event * instant[MAX_MIDI_EVENTS_PER_INSTANT];
    uint8_t instant_n; // number of events in the current instant
    unsigned short i;
    unsigned short n;
    bool completed;
    void(*completionCallback)(void);
    void(*failureCallback)(void);
    void handleCompletion();
    /* constructor */
    Song(event * p_events, unsigned short n_events)
      : p_events(p_events),
        instant_n((uint8_t) 0),
        i((unsigned short) 0),
        n(n_events),
        completed(false)
        { updateInstant(); };
    /* other methods */
    bool attempt(uint8_t action, uint8_t note);
    bool isCompleted() {return completed;}
    void reset();
    void setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void));
    bool updateInstant(); // returns false if there are no more events/instants to load
    bool instantIsComplete(); // returns whether all events in this->instant are complete
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
  public:
    Song ** pp_songs;
    unsigned short n;
    void(*completionCallback)(void); // fired when all songs in the bank are completed
    void(*failureCallback)(void); // fired when a midi event fails to match any song in the bank
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