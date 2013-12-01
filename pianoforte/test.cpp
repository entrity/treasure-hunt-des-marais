#include "serial-mock.h"
#include "song-handling.h"
#include "midi-song.h"
#include "midi-listener.h"
#include <iostream>
using namespace std;

extern MjMidi::SongBank songBank;
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff); // input from midi keyboard


int main()
{
  midiListener.noteOnCallback(76, 144);
  midiListener.noteOffCallback(76);


  cout << "num of songs " << songBank.n << endl;
  for (int i=0; i < songBank.n; i++) {
    MjMidi::Song * p_song = songBank.pp_songs[i];
    cout << "== song " << dec << i << ": " << p_song->n << " events" << endl;
    for (int i=0; i < p_song->n; i++) {
      MjMidi::event * p_evt = &p_song->p_events[i];
      cout << "\t" << dec << (int) p_evt->note;
    }
    cout << endl;
  }
  return 0;
}
