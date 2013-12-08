#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "morse_codes.h"
#include "main.h"
#include "strings.h"

extern mCode_t morseBuffer;
extern char * charBuffer_p;
extern volatile uint8_t charBuffer_i;

inline void reset()
{
	morseBuffer.n = 0;
	charBuffer_i = 0;
}

/* Iterate through all inputs[]. If one matches charBuffer, return its index. Else,
	return -1. */
inline int getMatchingInput()
{
	for (int i=0; i < INPUTS_N; i++) {
		if ( strlen(inputs[i]) == charBuffer_i && memcmp(inputs[i], charBuffer_p, charBuffer_i) == 0)
		{ return i; }
	}
	return -1;
}

/* Try to match current charBuffer against all of the keys in inputs[].
	If a match is found, output the corresponding output */
inline void processChars()
{
	int i = getMatchingInput();
	if (i < 0) return;
	if (i == 0)
		activateSolenoid();
	else
		outputMorse(i);
	reset();
}

#endif