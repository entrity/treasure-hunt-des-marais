#ifndef MAIN_H
#define MAIN_H

void outputMorse(int i);

void outputMorseChar(char c);

inline void disableTimer()
{
  TIMSK = 0;
}

inline void startTimer()
{
  TIFR |= (1<<OCF1A); // writing one to the flag clears timer interrupt flag, if extant
  TCNT1 = 0; // clear timer/counter
  TIMSK = (1<<OCIE1A); // interrupt on overflow
}

inline void togglePiezoPin()
{
  if (PINB & (1<<PIEZO_PIN))  {PORTB &= ~(1<<PIEZO_PIN);}
  else                        {PORTB |= (1<<PIEZO_PIN);}
}

inline void activateSolenoid()
{ 
  PORTB |= (1<<SOLENOID_PIN);
  _delay_ms(3000);
  PORTB &= ~(1<<SOLENOID_PIN);
}

#endif