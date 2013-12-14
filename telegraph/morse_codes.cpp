#include "morse_codes.h"

#define MORSE_CODES_N 26

mCode_t morseCodes[] = {
	{'a', {0,1}, 			2},
	{'b', {1,0,0,0}, 	4},
	{'c', {1,0,1,0}, 	4},
	{'d', {1,0,0}, 		3},
	{'e', {0}, 				1},
	{'f', {0,0,1,0}, 	4},
	{'g', {1,1,0}, 		3},
	{'h', {0,0,0,0}, 	4},
	{'i', {0,0}, 			2},
	{'j', {0,1,1,1}, 	4},
	{'k', {1,0,1}, 		3},
	{'l', {0,1,0,0},	4},
	{'m', {1,1},			2},
	{'n', {1,0},			2},
	{'o', {1,1,1},		3},
	{'p', {0,1,1,0},	4},
	{'q', {1,1,0,1},	4},
	{'r', {0,1,0},		3},
	{'s', {0,0,0},		3},
	{'t', {1},				1},
	{'u', {0,0,1},		3},
	{'v', {0,0,0,1},	4},
	{'w', {0,1,1},		3},
	{'x', {1,0,0,1},	4},
	{'y', {1,0,1,1},	4},
	{'z', {1,1,0,0},	4}
};


/* Return char corresponding to contents of morseBuffer */
char interpretMorse(mCode_t * p_data)
{
	for (int i = 0; i < MORSE_CODES_N; i++) {
		if ( mCodesMatch(&morseCodes[i], p_data) ) {
			return morseCodes[i].c;
		}
	}
	return 0;
}

/* Return morse code corresponding to given char */
mCode_t * toMorse(char c)
{
	for (int i = 0; i < MORSE_CODES_N; i++) {
		if (morseCodes[i].c == c) {
			return &morseCodes[i];
		}
	}
	return NULL;
}
