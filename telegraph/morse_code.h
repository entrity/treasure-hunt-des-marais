#ifndef MORSE_CODE_H
#define MORSE_CODE_H

#include <cstring>
#include <stdint.h>

typedef struct {
	char c;
	bool code[5];
	uint8_t n;
} mCode_t;

inline bool mCodesMatch(mCode_t * a, mCode_t * b)
{
	return (a->n == b->n) && (memcmp(a->code, b->code, a->n) == 0);
}

char interpretMorse(mCode_t * data);

const mCode_t * toMorse(char c);

#endif