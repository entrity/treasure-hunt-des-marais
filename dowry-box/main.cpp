/*
  == ATTINY85 I/O ==
  PB0 : fire solenoid
  PB1 : ice solenoid
  PB2/ADC1 : control temperature
  PB3/ADC3 : ice lock temperature
  PB4/ADC2 : fire lock temperature
*/

#define CONTROL_ADMUX (0b00000001)
#define ICE_ADMUX     (0b00000011)
#define FIRE_ADMUX    (0b00000010)
#define ICE_SOLENOID (PB1)
#define FIRE_SOLENOID (PB0)

#include <stdint.h>
#include <avr/io.h>
#include <avr/delay.h>

uint8_t controlTemp, iceTemp, fireTemp;

uint8_t readTemp(uint8_t admux)
{
  uint8_t reading;
  ADMUX = (admux)|(1<<ADSC); // start conversion
  while(! ADMUX & (1<<ADIF) ) {} // wait until flag is set

  ADMUX |= (1<<ADIF); // write 1 to clear flag
  return reading;
}

int main() 
{
  // configure ADC
  ADCSRA = 0b10000000;
  while (1)
  {
    // read control temperature
    controlTemp = readTemp(CONTROL_ADMUX);
    // read ice temp, set solenoid
    if (readTemp(ICE_ADMUX) < controlTemp - 2)
      { PORTB |= (1<<ICE_SOLENOID); }
    else
      { PORTB &= ~(1<<ICE_SOLENOID); }
    // read fire temp, set solenoid
    if (readTemp(FIRE_ADMUX) > controlTemp + 2)
      { PORTB |= (1<<FIRE_SOLENOID); }
    else
      { PORTB &= ~(1<<FIRE_SOLENOID); }
  }
  // return
  return 0;
}