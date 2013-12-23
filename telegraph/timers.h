#ifndef TIMERS_H
#define TIMERS_H

#define OCR_VALUE (77) // (77)

extern volatile uint8_t keyOnTimerCt, keyOffTimerCt;

/* onTimer */

inline void startOnTimer()
{
  TIFR  = (1<<OCF0A);
  TCNT0 = 0;
  TIMSK |= (1<<OCIE0A);
}

inline void killOnTimer()
{
  TIMSK &= ~(1<<OCIE0A);
  keyOnTimerCt = 0;
}

inline bool onTimerIsRunning()
{
  return TIMSK & (1<<OCIE0A);
}

/* offTimer */

inline void startOffTimer()
{
  TIFR  = (1<<OCF1A);   // clear interrupt flag by writing logical 1
  TCNT1 = 0;            // clear timer/counter
  TIMSK |= (1<<OCIE1A); // enable interrupt
}

inline void killOffTimer()
{
  TIMSK &= ~(1<<OCIE1A);
  keyOffTimerCt = 0;
}

/* Both timers */

inline void killTimers()
{
  killOnTimer();
  killOffTimer();
}

inline void configureTimers()
{
  /* configure but do not enable timer interrupt for keyOn */
  TCCR0A = (1<<WGM01); // normal port operation, OC pins disconnected, CTC on OCRA
  TCCR0B = 0b101; // 1024 prescaler
  OCR0A  = 90; // 100x per second -- (F_CPU / 1024 / 100) - 1

  /* configure but do not enable timer interrupt for keyOff */
  TCCR1 =  (1<<CTC1); // normal port operation, OC pins disconnected, CTC on OCRA
  TCCR1 |= 0b1001; // 256 prescaler
  OCR1A =  77; // 100x per second -- (F_CPU / 1024 / 100) - 1

  /* don't let timers run yet*/
  TIMSK = 0;
}

#endif