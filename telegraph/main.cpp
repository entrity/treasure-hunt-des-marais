#include <avr/io>
#include <avr/delay>

#include "definitions.h"
#include "main.h"
#include "../delay-8mhz.h"
#include "morse_code.h"
#include "operations.h"
#include "strings.h"

bool value; // dash or dot
mCode_t morseBuffer;
char charBuffer[CHAR_BUFFER_LEN];
char * charBuffer_p = &charBuffer; // this is in use by operations.h instead of using charBuffer directly so that tests could be written more easily
uint8_t charBuffer_i;

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

// When key is pressed or released
ISR(INT0_vect)
{
	// handle trigger down
	if (PINB & (1<<TRIGGER_PIN))
		{value = DOT;} // clear value. if timer does not run out, it shall be DOT, not DASH
	// handle trigger up
	else
		{morseBuffer.code[morseBuffer.n++] = value;} // push dot/dash onto buffer
	// clear and enable timer
	startTimer();
}

// THRESHOLD seconds after key is pressed or released
ISR()
{
	// disable timer
	disableTimer();
	// if key down, current keypress represents DASH instead of DOT
	if (PINB & (1<<TRIGGER_PIN))
		{value = DASH;}
	// if key up, letter is complete
	else {
		charBuffer[charBuffer_i++] = interpretMorse(&morseBuffer); // compute character from morseBuffer
		processChars(); // check & handle match
	}
}

// When reset button is pressed
ISR()
{
	reset();
}

void activateSolendoid()
{

}

void outputMorse(int i)
{

}