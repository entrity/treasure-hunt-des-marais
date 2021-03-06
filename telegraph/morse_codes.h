#ifndef MORSE_CODE_H
#define MORSE_CODE_H

#ifdef _USING_GPP
  #include <cstring>
#else
  #include <string.h>
#endif
#include <stdint.h>

#define MORSE_BUFFER_LEN 5

typedef struct {
	char c;
	bool code[MORSE_BUFFER_LEN]; // sequence of DOTS and DASHES
	uint8_t n; // how many DOTS and DASHES are needed for char c (i.e. functional length of this->code)
} mCode_t;

inline bool mCodesMatch(mCode_t * a, mCode_t * b)
{
	return (a->n == b->n) && (memcmp(a->code, b->code, a->n) == 0);
}

char interpretMorse(mCode_t * data);

 mCode_t * toMorse(char c);

#endif