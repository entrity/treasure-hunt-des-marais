#ifndef NOTES_H
#define NOTES_H

#define NOTE(halfPrd, duration) {.halfPrd=halfPrd, .duration=duration}

typedef struct {
  unsigned int halfPrd;   // us (microseconds)
  unsigned long duration;  // us (microseconds)
} note_t;

#endif