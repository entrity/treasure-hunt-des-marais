#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "morse_codes.h"
#include "main.h"
#include "strings.h"
#include "pin_changes.h"

inline void pushMorseBuffer(bool value)
{
	morseBuffer.code[morseBuffer.n] = value;
	if (morseBuffer.n < MORSE_BUFFER_LEN) {morseBuffer.n ++;}
}

/* Evaluate contents of morseBuffer. Push it onto charBuffer.
	Clear morseBuffer. Set charBufferUpdated flag */
inline void pushCharBuffer(char c)
{
	charBuffer[charBuffer_i] = c;
	charBuffer_i = (charBuffer_i + 1) % CHAR_BUFFER_LEN;
	charBufferUpdated = true;
}

/* Return whether charBuffer matches string at +inputs[i]+ */
inline bool charBufferMatchesString(char * p_input)
{
	uint16_t inputLen = strlen(p_input);
	uint16_t charBufferOffset = charBuffer_i - inputLen;
	for (uint16_t o = 0; o < inputLen; o++) {
		char c = charBuffer[(charBufferOffset + o) % CHAR_BUFFER_LEN];
		if (p_input[o] != c) { return false; }
	}
	return true;
}

/* Try to match charBuffer against all strings in +inputs+. Return index or -1. */
inline short findCharBufferMatch()
{
	for (uint16_t i=0; i < INPUTS_N; i++) {
		if (charBufferMatchesString(inputs[i])) { return i; }
	}
	return -1;
}

/* Try to match current charBuffer against all of the keys in inputs[].
	If a match is found, output the corresponding output */
inline void processChars()
{
	short i = findCharBufferMatch();
	if (i == 0) { activateSolenoid(); }
	else if (i > 0) { outputMorse(i); }
}

inline char interpretMorseBuffer()
{
	mCode_t morseBufferCopy;
	memcpy((void *) &morseBufferCopy, (void *) &morseBuffer, sizeof(mCode_t));
	morseBuffer.n = 0;
	return interpretMorse(&morseBufferCopy);
}

#endif