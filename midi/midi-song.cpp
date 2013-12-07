#include "midi-song.h"
#include <Arduino.h>

/* return true if the given event has a matching action and note for the params */
inline bool matchesEvent(MjMidi::event * p_event, uint8_t action, uint8_t note)
{
  return p_event->action == action && p_event->note == note;
}

namespace MjMidi
{
  /* Loop through events from current instant until an event is completed or the 
    end of the instant is reached or the end of the song is reached. 

    If none is completed in this attempt and the end of the instant or song is
    reached, the song shall be reset.

    If the final event is completed, the song's callback shall be fired, and the
    song shall be reset. */
  bool Song::attempt(uint8_t action, uint8_t note)
  {
    bool success = false;
    bool needToCheckCompletion = true;
    // loop through events in the current instant in this song
    for (unsigned int i = 0; i < instant_n; i++) {
      if (instant[i]->complete)
        { continue; }
      if ( matchesEvent(instant[i], action, note) ) {
        success = true;
        instant[i]->complete = true;
        if (needToCheckCompletion && instantIsComplete() && !updateInstant())
          { handleCompletion(); }
        break;
      } else {
        needToCheckCompletion = false;
      }
    }
    // end of loop
    if (!success) {
      if (failureCallback) { failureCallback(); }
      reset();
    }
    return success;
  }

  bool Song::instantIsComplete()
  {
    for (int i=0; i < instant_n; i ++) {
      if (!instant[i]->complete) { return false; }
    }
    return true;
  }

  bool Song::updateInstant()
  {
    // set the instant start to the next incomplete event
    for (; i < n; i++) {
      if (!p_events[i].complete) {
        instant[0] = &p_events[i];
        instant_n = 1;
        // add events simultaneous to i to instant
        for (int j = 1; i+j < n; j++) {
          if (p_events[i+j].offset)
            {break;}
          else {
            instant_n++;
            instant[j] = &p_events[i+j];
          }
        }
        return true;
      }
    }
    return false;
  }

  /* Clear i. Clear .complete on all events */
  void Song::reset()
  {
    for (unsigned int i = 0; i < this->n; i++) {
      p_events[i].complete = false;
    }
    this->i = 0;
  }

  void Song::setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void))
  {
    this->completionCallback = completionCallback;
    this->failureCallback = failureCallback;
  }

  /* Call attempt on all of the Songs in this SongBank */
  void SongBank::handleMidiEvent(uint8_t action, uint8_t note)
  {
    bool failure = true;
    bool completed = true;
    // loop through all songs in bank
    for (unsigned int i = 0; i < n; i++) {
      // clear failure if any song accepts the current midi event
      if (pp_songs[i]->attempt(action, note)) { failure = false; }
      // clear completed if any song is not complete
      if (!pp_songs[i]->isCompleted()) { completed = false; }
    }
    // fire callbacks if indicated
    if (completed && completionCallback)
      {completionCallback();}
    if (failure && failureCallback)
      {failureCallback();}
    #ifdef DEBUG
      if (failure) {
        DEBUG.print("MIDI ");
        DEBUG.print(note);
        DEBUG.print(":");
        DEBUG.println(action);
      }
    #endif
  }

  void SongBank::setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void))
  {
    this->completionCallback = completionCallback;
    this->failureCallback = failureCallback;
  }
}
