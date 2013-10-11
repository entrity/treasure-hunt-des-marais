/*
  PB1 Clock
  PB2 (INT0) Button
  PB4 Piezo
*/
#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "notes.h"
#include "mysong.h"

#define PIEZO_PIN PB4
#define BUTTON_PIN PB2

bool piezoHigh = false;
unsigned char piezoOnMask = (1<<PIEZO_PIN);
unsigned char piezoOffMask = ~(1<<PIEZO_PIN);

void playSong();
void playNote(note_t);
void delayMicroseconds(unsigned int us);
inline void togglePiezo() { piezoHigh = !piezoHigh; if (piezoHigh) PORTB &= piezoOffMask; else PORTB |= piezoOnMask; }

int main()
{
  // no prescaler on clock
  cli();
  CLKPR = (1<<CLKPCE);
  CLKPR &= 0b01110000;
  // enable falling external interrupt

  sei();
  // enable output on PIEZO_PIN
  DDRB = (1<<PIEZO_PIN);
  // indefinite loop
  playSong();
  note_t note = { 1915, 1000000L };
  playNote(note);
  while(1) {}
  return 0;
}

void playSong()
{
  // for(int i=0; i < mySong_n; i++)
  //   { playNote(mySong[i]); } 
}

void playNote(note_t note)
{
  unsigned int changes_n = 300;//note.duration / note.halfPrd;
  for(int i=0; i < changes_n; i++) {
    togglePiezo();
    delayMicroseconds(note.halfPrd);
  }
  _delay_us(note.duration % note.halfPrd);
}


ISR(PCINT0_vect)
{
  if (PINB & (1<<BUTTON_PIN)) playSong();
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{

  // for the 8 MHz internal clock on the ATmega168

  // for a one- or two-microsecond delay, simply return.  the overhead of
  // the function calls takes more than two microseconds.  can't just
  // subtract two, since us is unsigned; we'd overflow.
  if (--us == 0)
  return;
  if (--us == 0)
  return;

  // the following loop takes half of a microsecond (4 cycles)
  // per iteration, so execute it twice for each microsecond of
  // delay requested.
  us <<= 1;
  
  // partially compensate for the time taken by the preceeding commands.
  // we can't subtract any more than this or we'd overflow w/ small delays.
  us--;

  // busy wait
  __asm__ __volatile__ (
  "1: sbiw %0,1" "\n\t" // 2 cycles
  "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
}
