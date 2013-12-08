/*
  == ATTINY85 I/O ==
  PB0 : fire solenoid
  PB1 : ice solenoid
  ADC1(PB2) : control temperature
  ADC2(PB4) : fire lock temperature
  ADC3(PB3) : ice lock temperature
*/

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#ifdef NO_ARDUINO
  #include "main.h"
#endif

#define CTRL_ADMUX    (1)
#define FIRE_ADMUX    (2)
#define ICE_ADMUX     (3)
#define ICE_SOLENOID  (PB1)
#define FIRE_SOLENOID (PB0)

uint16_t controlTemp, iceTemp, fireTemp;

uint16_t readTemp(uint8_t admux)
{
  uint16_t reading;
  ADMUX  &= 0b11110000; // clear the input channel selection 
  ADMUX  |= admux;      // input channel selection
  ADCSRA |= (1<<ADIF);  // write 1 to clear flag
  ADCSRA |= (1<<ADSC);  // start conversion on ADC
  while(! (ADCSRA & (1<<ADIF)) ) ; // wait until flag is set
  reading = ADCL;         // read low byte from ADC
  reading += (ADCH << 8); // read high byte from ADC
  return reading;
}

int main() 
{
  // IO pins
  DDRB = (1<<ICE_SOLENOID)|(1<<FIRE_SOLENOID);
  // configure ADC
  ADMUX  = ADMUX_VR;   // set voltage reference !!! this differs on attiny85 vs atmega328 !!!
  ADCSRA = 0b10010111; // full prescaler, enable ADC, set ADIF flag to clear it
  // loop
  while (1)
  {
    // read control temperature
    controlTemp = readTemp(CTRL_ADMUX);
    // read ice temp, set solenoid
    iceTemp = readTemp(ICE_ADMUX);
    if (iceTemp < controlTemp - 2)
      { PORTB |= (1<<ICE_SOLENOID); }
    else
      { PORTB &= ~(1<<ICE_SOLENOID); }
    // read fire temp, set solenoid
    fireTemp = readTemp(FIRE_ADMUX);
    if (fireTemp > controlTemp + 2)
      { PORTB |= (1<<FIRE_SOLENOID); }
    else
      { PORTB &= ~(1<<FIRE_SOLENOID); }
    // debug
    #ifdef DEBUG
      Serial.print("c "); Serial.println(controlTemp);
      Serial.print("i "); Serial.println(iceTemp);
      Serial.print("f "); Serial.println(fireTemp);
      Serial.println("====="); delay(1000);
    #endif
  }
  // return
  return 0;
}
