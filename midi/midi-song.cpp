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
    bool songIsCompleteSoFar = true;
    bool success = false;
    unsigned int firstIndexInInstant = this->i;
    unsigned int i;
    // loop through subsequent events in this song, breaking when there is an offset or song end
    for (i = firstIndexInInstant; i < n; i ++) {
      // break if the end of the instant is reached:
      if (p_events[i].offset && i != firstIndexInInstant)
        { break; }
      // skip ahead if the current event is already complete
      if (p_events[i].complete)
        { continue; }
      // check whether the given midi event matches the current event in the loop:
      if ( matchesEvent(&p_events[i], action, note) ) {
        // successful attempt (i.e. a correct note was received)
        success = true;
        // mark the current event complete
        p_events[i].complete = true;
        // if no incomplete events have been encountered so far:
        if (songIsCompleteSoFar) {
          // update song index 'i'
          this->i = i + 1;
          // if the end of the song is reached, mark it complete
          if (this->i >= this->n) { handleCompletion(); }
        }
      } else { // given midi event does not match current event
        songIsCompleteSoFar = false;
      }
    }
    // end of loop
    if (!success) {
      if (failureCallback) { failureCallback(); }
      reset();
    }
    return success;
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
  }

  void SongBank::setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void))
  {
    this->completionCallback = completionCallback;
    this->failureCallback = failureCallback;
  }
}
