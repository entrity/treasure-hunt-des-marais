#include "../delay-8mhz.h"
#include "morse_code.h"


#define MORSE_BUFFER_LEN 10		// how many dot/dash to remember from trigger
#define CHAR_BUFFER_LEN 30		// how many chars to remember from trigger
#define DASH_THRESHOLD 333 // ms that distinguishes dot from dash
#define TRIGGER_PIN PB3
#define DASH true
#define DOT false

bool value; // dash or dot
mCode_t morseBuffer;
char charBuffer[CHAR_BUFFER_LEN];
uint8_t charBuffer_i;

#define INPUTS_N 1
char * inputs[] = {
	"JULY"
};
char * outputs[] = {
	"SHINE THE CRIMSON LIGHT UPON THE ROSE HOROLOGE"
};

inline void outputMorse(bool *);
inline void reset();
inline void processChars();
inline void disableTimer() {  } todo
inline void startTimer() {  } todo

int main()
{
	// configure trigger interrupt (INT0) on any change
	// configure but do not enable timer interrupt
	// enable global interrupts
	sei();
	while (1) {}
	return 0;
}

inline void reset()
{
	morseBuffer.n = 0;
	charBuffer_i = 0;
}


// Trigger interrupt
ISR(INT0_vect)
{
	if (PINB & (1<<TRIGGER_PIN)) {
		// clear value. if timer does not run out, it shall be DOT, not DASH
		value = DOT;
	} else {
		// push dot/dash onto buffer
		morseBuffer.code[morseBuffer.n++] = value;
	}
	// clear and enable timer
	startTimer();
}

// Timer interrupt
ISR()
{
	// disable timer
	disableTimer();
	if (PINB & (1<<TRIGGER_PIN)) {
		// set value
		value = DASH;
	} else {
		// compute character from morseBuffer
		charBuffer[charBuffer_i++] = interpretMorse(&morseBuffer);
		// check & handle match
		processChars();
	}
}

// PC interrupt
ISR()
{
	reset();
}