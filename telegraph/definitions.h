#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MORSE_BUFFER_LEN 10		// how many dot/dash to remember from trigger
#define CHAR_BUFFER_LEN 30		// how many chars to remember from trigger
#define DASH_THRESHOLD 333 // ms that distinguishes dot from dash
#define TRIGGER_PIN PB3
#define SOLENDOID_PIN
#define DASH true
#define DOT false
#define MIN(a,b) { return a < b ? a : b; }

#endif