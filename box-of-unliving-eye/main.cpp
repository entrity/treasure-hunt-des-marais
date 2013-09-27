/*
Pin change interrupts set up on PORTD 2:0 (PCINT18:16 => PCIE2)
LED pins are PORTB 2:0 (connected to buttons)
Solenoid pin is PORTB 4
 */ 

#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#define SOLENOID_PIN 4
#define SUCCESS_N 5 // the requisite number of successes to open the box

char ledIndex, // which of the led pins is active
	successCount = 0; // how many good button clicks have been logged in a row

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
	PORTB |= (1<<SOLENOID_PIN); // activate solenoid
	_delay_ms(1000 * 10); // wait 10 seconds
	PORTB &= ~(1<<SOLENOID_PIN); // deactivate solenoid
}

int main(void)
{
	DDRB |= (1<<0)|(1<<1)|(1<<2); // enable output on led pins
	DDRB |= (1<<SOLENOID_PIN); // enable output on solenoid pin
	
	PCICR = (1<<PCIE2); // enable pin change interrupts on PCINT23..16
	PCMSK2 = 0b111; // enable PCINT2 interrupts on 0,1,2
	
	sei(); // enable global interrupts
	
	ledIndex = rand() % 3;
	randomizeLed(); // set initial ledIndex value
	
  while(1) {}
}

void buttonClicked()
{
	signed char buttonIndex = getButtonIndex();
	if (buttonIndex > -1) {
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
	if (PIND & (1<<0))
	 return 0;
	if (PIND & (1<<1))
	 return 1;
	if (PIND & (1<<2))
	 return 2;
	return -1;
}

ISR(PCINT2_vect) { buttonClicked(); }
