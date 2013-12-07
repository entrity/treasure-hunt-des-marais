#include "../test/Arduino.h"
#include "song-handling.h"
#include "midi-song.h"
#include "midi-listener.h"
#include <iostream>
#include <fstream>
using namespace std;

extern MjMidi::SongBank songBank;
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff); // input from midi keyboard
MjMidi::event * p_evt;
MjMidi::Song * p_song;

inline void outputSongSummary(std::ostream &out, MjMidi::Song * p_song, int i)
{
  out << "== song " << dec << i << ": " << p_song->n << " events" << endl;
}

inline void outputEvent(std::ostream &out, MjMidi::event * p_evt)
{
  out << (int) p_evt->note << ":" << (int) p_evt->action << endl;
}

void debugSongs()
{
  midiListener.noteOnCallback(76, 144);
  midiListener.noteOffCallback(76);
  cout << "num of songs " << songBank.n << endl;
  for (int i=0; i < songBank.n; i++) {
    MjMidi::Song * p_song = songBank.pp_songs[i];
    outputSongSummary(cout, p_song, i);
    for (int i=0; i < p_song->n; i++) {
      MjMidi::event * p_evt = &p_song->p_events[i];
      cout << "\t" << dec << (int) p_evt->note;
    }
    cout << endl;
  }
}

void debugInstants()
{
  ofstream fout;
  fout.open("debug-song-instants.debug");
  for (int i=0; i < songBank.n; i++) {
    MjMidi::Song * p_song = songBank.pp_songs[i];
    outputSongSummary(fout, p_song, i);
    fout << "\t -- first instant:" << endl;
    for (int i=0; i < p_song->instant_n; i++) {
      MjMidi::event * p_evt = p_song->instant[i];
      fout << "\t\t";
      outputEvent(fout, p_evt);
    }
    int instantCount = 0;
    while (p_song->i < p_song->n) {
      if (p_song->updateInstant()) {
        fout << "\t -- instant" << ++instantCount << endl;
        for (int i=0; i < p_song->instant_n; i++) {
          MjMidi::event * p_evt = p_song->instant[i];
          fout << "\t\t";
          outputEvent(fout, p_evt);
        }
      }
      p_song->i += p_song->instant_n;
    }
  }
  fout.close();
}

void yellowSongFirstInstant()
{
  p_song = songBank.pp_songs[3];
  p_evt = &p_song->p_events[0];
  outputEvent(cout, p_evt);
  outputEvent(cout, p_song->instant[0]);
}

int main()
{
  // debugSongs();
  debugInstants();
  yellowSongFirstInstant();
  return 0;
}
