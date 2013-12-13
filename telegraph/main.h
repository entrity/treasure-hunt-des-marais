#ifndef MAIN_H
#define MAIN_H

#include "./morse_codes.h"

#define MORSE_BUFFER_LEN 10   // how many dot/dash to remember from trigger
#define CHAR_BUFFER_LEN 256    // how many chars to remember from trigger

extern volatile char charBuffer[CHAR_BUFFER_LEN];
extern volatile mCode_t morseBuffer;
extern volatile bool value; // dash or dot
extern volatile uint8_t charBuffer_i;
extern volatile uint8_t keyOnTimerCt, keyOffTimerCt;
extern volatile bool charBufferUpdated;

void outputMorse(int i);
void outputMorseChar(char c);

inline void processChars();

#endif