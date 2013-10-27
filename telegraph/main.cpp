#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

/*
	PINOUT:
	PB1 = PCINT1 = reset button
	PB2 = INT0 = telegraph key (trigger)
	PB3 = piezo pin
	PB4 = solenoid pin
*/

#define MORSE_BUFFER_LEN 10		// how many dot/dash to remember from trigger
#define CHAR_BUFFER_LEN 30		// how many chars to remember from trigger
/* duration of morse entities in milliseconds */
#define DOT_MS 200
#define DASH_MS (DOT_MS * 3)
#define WORD_BREAK_MS (DOT_MS * 7)
#define DASH_THRESHOLD 500 // ms that distinguishes dot from dash
/* morse entities (values for bool value) */
#define DASH true
#define DOT false
/* pin designations */
#define PB5 ERROR // we don't want to ever touch this pin. It is RESET
#define RESET_PIN PCINT1
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4

#include "main.h"
#include "../delay-8mhz.h"
#include "morse_code.h"
#include "operations.h"
#include "strings.h"

bool value; // dash or dot
mCode_t morseBuffer;
char charBuffer[CHAR_BUFFER_LEN];
char * charBuffer_p = charBuffer; // this is in use by operations.h instead of using charBuffer directly so that tests could be written more easily
uint8_t charBuffer_i;

inline void disableTimer() { TIMSK = 0; }
inline void startTimer()
{
	TCNT1 = 0; // clear timer/counter
	TIMSK = (1<<OCIE1A); // interrupt on overflow
}
inline void togglePiezoPin()
{
	if (PINB & (1<<PIEZO_PIN)) 	{PORTB &= ~(1<<PIEZO_PIN);}
	else 												{PORTB |= (1<<PIEZO_PIN);}
}

int main()
{
	// enable output on solenoid and piezo
	DDRB = (1<<SOLENOID_PIN) | (1<<PIEZO_PIN);
	// enable interrupts for pin change and external interrupt
	GIMSK = (1<<INT0) | (1<<PCIE);
	// configure trigger interrupt (INT0) on any change
	MCUCR = (1<<ISC00);
	// configure pin change interrupt on reset button pin
	PCMSK = (1<<RESET_PIN);
	// configure but do not enable timer interrupt:
	TCCR1 = (1<<CTC1) | (1<<CS13) | (1<<CS12) | (1<<CS11) | (1<<CS10); // ctc mode, prescaler 16384 ()
	OCR1A = OCR1C = 244; // F_CPU / 16384 * DASH_THRESHOLD;
	disableTimer();
	// enable global interrupts
	sei();

	// boilerplate
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
ISR(TIMER1_COMPA_vect)
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
ISR(PCINT0_vect)
{
	reset();
}

void activateSolenoid()
{	
	PORTB |= (1<<SOLENOID_PIN);
	_delay_ms(3000);
	PORTB &= ~(1<<SOLENOID_PIN);
}

void outputMorse(int outputIndex)
{
	char * text = outputs[outputIndex];
	// process each character in message
	for (int i = 0; i < strlen(text); i ++) {
		char c = text[i];
		if (c == ' ') { _delay_ms(WORD_BREAK_MS); }
		else { outputMorseChar(c); }
	}
}

void outputMorseChar(char c)
{
	mCode_t * charCode = toMorse(c);
	for (int i = 0; i < charCode->n; i++) {
		// output tone
		int ms = (charCode->code[i] == DASH) ? DASH_MS : DOT_MS;
		for (int tick = 0; tick < ms; tick++) { togglePiezoPin(); _delay_ms(1); }
		// pause for length of dot between signals
		_delay_ms(DOT_MS);
	}
	// pause for length of dash at end of char
	_delay_ms(DASH_MS);
}