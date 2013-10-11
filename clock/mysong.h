#ifndef MYSONG_H
#define MYSONG_H

#include "notes.h"

#define NOTE(freq, beats) { (1000000L/(2*freq)), (beats*1000000L/6) }

int mySong_n = 13;

note_t mySong[] = {
  NOTE(C4, 2),
  NOTE(D4, 2),
  NOTE(E4, 4),
  NOTE(C4, 2),
  NOTE(E4, 2),
  NOTE(D4, 4),
  NOTE(C4, 2),
  NOTE(D4, 2),
  NOTE(E4, 2),
  NOTE(C4, 2),
  NOTE(E4, 2),
  NOTE(D4, 2),
  NOTE(C4, 4)
};

#endif