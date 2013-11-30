/*
Button pins (Pin change interrupts) are PORTA 2:0 (PCINT2:0 => PCIE0)
LED pins are PORTB 2:0 (connected to buttons)
Solenoid pin is PORTA 7. Use 220 ohm -- and no higher -- resistor between PA7 and transistor base
Diagnostic pins are PORTA 3:5

solid brown: led -
solid color: led +

strpd brown: btn +
strpd color: btn out to IC
*/ 

#define F_CPU 1000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#define SOLENOID_PIN 7
#define SUCCESS_N 5 // the requisite number of successes to open the box

char ledIndex, // which of the led pins is active
	successCount = 0; // how many good button clicks have been logged in a row
volatile bool buttonClickedFlag;

void buttonClicked();
signed char getButtonIndex();

/* Set ledIndex value to a random number that's not what it was before */
inline void randomizeLed()
{
	char newIndex = rand() % 2;
	ledIndex = (newIndex == ledIndex) ? newIndex + 1 : newIndex;
	PORTB &= ~(0b111);
	PORTB |= (1<<ledIndex);
}

inline void openBox()
{
	PORTA |= (1<<SOLENOID_PIN); 	// activate solenoid
	_delay_ms(1000 * 5); 					// wait 5 seconds
	PORTA &= ~(1<<SOLENOID_PIN); 	// deactivate solenoid
}

int main(void)
{
	DDRB |= (1<<0)|(1<<1)|(1<<2); // enable output on led pins
	DDRA |= (1<<SOLENOID_PIN); 		// enable output on solenoid pin
	DDRA |= (1<<3)|(1<<4)|(1<<5); // enable output on diag/count pins
	
	GIMSK = (1<<PCIE0); // enable pin change interrupts on PCINT7:0
	PCMSK0 = 0b111; // enable PCINT0 interrupts on 0,1,2
	
	sei(); // enable global interrupts
	
	ledIndex = rand() % 3;
	randomizeLed(); // set initial ledIndex value

	PORTA |= (1<<3)|(1<<5);
	
  while(1) {
  	if (buttonClickedFlag) {
  		PORTA &= 0b000111;
  		buttonClicked();
  		buttonClickedFlag = false;
  	}
  }
}

inline void displaySuccessCount()
{
	int i;
	char c = successCount > 7 ? 7 : successCount;
	if (successCount == 0) c = 7;
	c = c << 3; // outputting on PORTA 3:5
	for (i=3; i<6; i++) { // outputting on PORTA 3:5
		if (c & (1<<i))
			PORTA |= (1<<i);
		else
			PORTA &= ~(1<<i);
	}
}

void buttonClicked()
{
	_delay_ms(100); // wait 1/100 sec
	signed char buttonIndex = getButtonIndex();
	if (buttonIndex > -1) {
		PORTA |= (1<<buttonIndex<<3);
		if (buttonIndex == ledIndex) {
	 		if (++successCount >= SUCCESS_N) { openBox(); }
		} else {
			successCount = 0;
		}
		randomizeLed();
	}
}

/* Return -1 if no button is pressed. The pin number, otherwise. */
signed char getButtonIndex()
{
	PORTA &= 0b11000111;
	if (PINA & (1<<0))
	 return 0;
	if (PINA & (1<<1))
	 return 1;
	if (PINA & (1<<2))
	 return 2;
	PORTA |= 0b00111000;
	return -1;
}

ISR(PCINT0_vect) { buttonClickedFlag = true; }
