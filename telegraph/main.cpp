#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
	PINOUT:
	PB2 = INT0 = telegraph key (trigger)
	PB3 = piezo pin
	PB4 = solenoid pin
*/


/* duration of morse entities in milliseconds */
#define DOT_MS 200 // used only in output
#define DASH_MS (DOT_MS * 3) // used only in output
#define WORD_BREAK_MS (DOT_MS * 7) // used only in output
#define DASH_THRESHOLD 500 // ms that distinguishes dot from dash; used only in input
/* morse entities (values for bool value) */
#define DASH true
#define DOT false
/* pin designations */
#define PB5 ERROR // we don't want to ever touch this pin. It is RESET
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4

#ifndef USE_ARDUINO
	#include "timers.h"
	#include "main.h"
	#include "../delay-8mhz.h"
	#include "morse_codes.h"
	#include "operations.h"
	#include "strings.h"
#else
	#include "d:/dev/avr/treasure-hunt-2013/telegraph/main.h"
	#include "d:/dev/avr/treasure-hunt-2013/telegraph/../delay-8mhz.h"
	#include "d:/dev/avr/treasure-hunt-2013/telegraph/morse_codes.h"
	#include "d:/dev/avr/treasure-hunt-2013/telegraph/operations.h"
	#include "d:/dev/avr/treasure-hunt-2013/telegraph/strings.h"
#endif

// volatile vars
volatile char charBuffer[CHAR_BUFFER_LEN];
volatile mCode_t morseBuffer;
volatile bool value; // dash or dot
volatile uint8_t charBuffer_i;
volatile uint8_t keyOnTimerCt, keyOffTimerCt;
volatile bool triggerIsPressed;
volatile bool charBufferUpdated;

int main()
{
	/* setup output pins*/
	// enable output on solenoid and piezo
	DDRB = (1<<SOLENOID_PIN) | (1<<PIEZO_PIN);

	/* setup telegraph key interrupts*/
	// enable interrupts for pin change and external interrupt
	GIMSK = (1<<INT0) | (1<<PCIE);
	// configure trigger interrupt (INT0) on any change
	MCUCR = (1<<ISC00);

	/* configure timers */
	configureTimers();
	
	// enable global interrupts
	sei();

	/* loop */
	while (1) {
		if (charBufferUpdated) {
			charBufferUpdated = false;
			processChars();
		}
	}
	return 0;
}

// When key is pressed or released:
ISR(INT0_vect)
{
	// if key On
	if (PINB & (1<<TRIGGER_PIN)) {
		killOffTimer();
		if (!onTimerIsRunning()) {
			startOnTimer();
			value = DOT;
		}
	}
	// if key Off
	else {
		startOffTimer();
	}
}

// onTimer. fires every 1/100th seconds when in use
ISR(TIMER0_COMPA_vect)
{
	keyOnTimerCt ++;
	// at 1/4 second
	if (keyOnTimerCt >= 25 and value == DOT) {
		value = DASH;
	}
}

// offTimer. fires every 1/100th seconds when in use
ISR(TIMER1_COMPA_vect)
{
	keyOffTimerCt ++;
	// at 1/10th second: validate previous keyOn as key event
	if (keyOffTimerCt >= 10 && onTimerIsRunning()) {
		killOnTimer();
		pushMorseBuffer(value);
		#if DEBUG
			morseFlag = true;
		#endif
	}
	// at 1/2 second: validate this as char break
	if (keyOffTimerCt >= 50) {
		killOffTimer();
		char c = interpretMorseBuffer();
		pushCharBuffer(c);
	}
}

void outputMorse(int outputIndex)
{
	char * text = outputs[outputIndex];
	// process each character in message
	for (int i = 0; i < strlen(text); i ++) {
		char c = text[i];
		#ifdef DEBUG
			Serial.print(c);
		#endif
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
