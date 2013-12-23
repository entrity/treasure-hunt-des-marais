#ifndef PIN_CHANGES_H
#define PIN_CHANGES_H

inline void activateSolenoid()
{ 
  PORTB |= (1<<SOLENOID_PIN);
  _delay_ms(3000);
  PORTB &= ~(1<<SOLENOID_PIN);
}

inline void togglePiezoPin()
{
  if (PINB & (1<<PIEZO_PIN))  {PORTB &= ~(1<<PIEZO_PIN);}
  else                        {PORTB |= (1<<PIEZO_PIN);}
}

#endif