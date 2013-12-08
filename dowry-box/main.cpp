/*
  == ATTINY85 I/O ==
  PB0 : fire solenoid
  PB1 : ice solenoid
  PB2/ADC1 : control temperature
  PB3/ADC3 : ice lock temperature
  PB4/ADC2 : fire lock temperature
*/

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#if MCU==attiny85
  #include "main.h"
#endif

#define CONTROL_ADMUX (0b00000001)
#define ICE_ADMUX     (0b00000011)
#define FIRE_ADMUX    (0b00000010)
#define ICE_SOLENOID  (PB1)
#define FIRE_SOLENOID (PB0)

uint16_t controlTemp;

uint16_t readTemp(uint8_t admux)
{
  uint16_t reading;
  ADMUX  &= 0b11110000; // clear the input channel selection 
  ADMUX  |= admux;      // input channel selection
  ADCSRA |= (1<<ADIF);  // write 1 to clear flag
  ADCSRA |= (1<<ADSC);  // start conversion on ADC
  while(! ADCSRA & (1<<ADIF) ) {} // wait until flag is set
  reading = ADCL;         // read low byte from ADC
  reading += (ADCH << 8); // read high byte from ADC
  return reading;
}

int main() 
{
  // configure ADC
  ADMUX  = ADMUX_VR;   // set voltage reference !!! this differs on attiny85 vs atmega328 !!!
  ADCSRA = 0b10010111; // full prescaler, enable ADC, set ADIF flag to clear it
  // loop
  while (1)
  {
    // read control temperature
    controlTemp = readTemp(CONTROL_ADMUX);
    CONTROL_CALLBACK()
    // read ice temp, set solenoid
    if (readTemp(ICE_ADMUX) < controlTemp - 2)
      { PORTB |= (1<<ICE_SOLENOID); }
    else
      { PORTB &= ~(1<<ICE_SOLENOID); }
    // read fire temp, set solenoid
    if (readTemp(FIRE_ADMUX) != controlTemp)
      { PORTB |= (1<<FIRE_SOLENOID); }
    else
      { PORTB &= ~(1<<FIRE_SOLENOID); }
  }
  // return
  return 0;
}