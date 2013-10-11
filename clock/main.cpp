/*
  PB1 (INT0) Button
  PB2 Clock
  PB4 Piezo
*/
#define F_CPU 600000L

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "notes.h"
#include "mysong.h"

#define PIEZO_PIN PB4
#define BUTTON_PIN PB1

bool piezoHigh = false;
unsigned char piezoOnMask = (1<<PIEZO_PIN);
unsigned char piezoOffMask = ~(1<<PIEZO_PIN);

void playSong();
void playNote(note_t);
inline void togglePiezo() { piezoHigh = !piezoHigh; if (piezoHigh) PORTB &= piezoOffMask; else PORTB |= piezoOnMask; }

int main()
{
  // enable falling external interrupt
  // MCUCR |= (1<<ISC01);
  // GIMSK |= (1<<INT0);
  // SREG  =  (1<<7);
  // enable output on PIEZO_PIN
  DDRB |= (1<<PIEZO_PIN);
  // indefinite loop
  playSong();
  while(1) {}
  return 0;
}

void playSong()
{
  for(int i=0; i < mySong_n; i++)
  {}
  //   playNote(mySong[i]);
}

void playNote(note_t note)
{
  int changes_n = note.duration / note.halfPrd;
  for(int i=0; i < changes_n; i++) {
    togglePiezo();
    _delay_us(note.halfPrd);
  }
  _delay_us(note.duration % note.halfPrd);
}

ISR(PCINT0_vect)
{
  if (PINB & (1<<BUTTON_PIN)) playSong();
}