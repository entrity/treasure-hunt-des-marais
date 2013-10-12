#ifndef MYSONG_H
#define MYSONG_H

#include "notes.h"

#define NOTE(freq, beats) { (1000000L/(2*freq)), (beats*1000000L/6) }

int mySong_n = 15;

note_t mySong[] = {
  NOTE(E5, 1),
  NOTE(F5, 1),
  NOTE(G5, 1),
  NOTE(E5, 1),
  NOTE(F5, 1),
  NOTE(G5, 1),
  
  NOTE(F5, 4),
  NOTE(E5, 2),
  
  NOTE(D5, 1),
  NOTE(E5, 1),
  NOTE(F5, 1),
  NOTE(D5, 1),
  NOTE(E5, 1),
  NOTE(F5, 1),
  
  NOTE(E5, 6)
};

#endif