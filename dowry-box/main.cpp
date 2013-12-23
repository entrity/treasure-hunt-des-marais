/*
  == ATTINY85 I/O ==
  PB0 : fire solenoid
  PB1 : ice solenoid
  ADC1(PB2) : control temperature
  ADC2(PB4) : fire lock temperature
  ADC3(PB3) : ice lock temperature
*/

#include <stdint.h>
#ifndef TEST_BUILD
  #include <avr/io.h>
  #include <util/delay.h>
  #define ADCSRA_SETTING (0b10010111)
  #define CLEAR_ADMUX_INPUT_CHANNEL (0b11110000)
#endif
#ifdef NO_ARDUINO
  #include "main.h"
#endif

#define CTRL_ADMUX    (1)
#define FIRE_ADMUX    (2)
#define ICE_ADMUX     (3)
#define ICE_SOLENOID  (PB1)
#define FIRE_SOLENOID (PB0)
#define READINGS_CT   (10)

uint16_t iceTemp, fireTemp;
uint32_t iceAvg, fireAvg;
uint16_t iceReadings[READINGS_CT], fireReadings[READINGS_CT];
uint8_t reading_i;

uint16_t readTemp(uint8_t admux)
{
  uint16_t reading;
  ADMUX  &= CLEAR_ADMUX_INPUT_CHANNEL; // clear the input channel selection 
  ADMUX  |= admux;      // input channel selection
  ADCSRA |= (1<<ADIF);  // write 1 to clear flag
  ADCSRA |= (1<<ADSC);  // start conversion on ADC
  while(! (ADCSRA & (1<<ADIF)) ) ; // wait until flag is set
  reading = ADCL;         // read low byte from ADC
  reading += (ADCH << 8); // read high byte from ADC
  return reading;
}

void takeReading(uint8_t admux, uint16_t readings[], uint16_t * p_temp, uint32_t * p_avg)
{
  *p_temp = readTemp(admux);
  *p_avg = 0;
  for (int i=0; i < READINGS_CT; i++) { *p_avg += readings[i]; }
  *p_avg /= READINGS_CT;
  readings[reading_i] = *p_temp;
}

void takeReadings()
{
  takeReading(ICE_ADMUX, iceReadings, &iceTemp, &iceAvg);
  takeReading(FIRE_ADMUX, fireReadings, &fireTemp, &fireAvg);
  reading_i = (reading_i + 1) % READINGS_CT;
}

int main() 
{
  // IO pins
  DDRB = (1<<ICE_SOLENOID)|(1<<FIRE_SOLENOID);
  // configure ADC
  ADMUX  = ADMUX_VR;   // set voltage reference !!! this differs on attiny85 vs atmega328 !!!
  ADCSRA = ADCSRA_SETTING; // full prescaler, enable ADC, set ADIF flag to clear it
  while (reading_i < READINGS_CT-1)
  {
    takeReadings();   
  }
  // loop
  while (1)
  {
    takeReadings();
    // check ice temp, set solenoid
    if (iceTemp < iceAvg - 2)
      { PORTB |= (1<<ICE_SOLENOID); }
    else if (iceTemp > iceAvg + 2)
      { PORTB &= ~(1<<ICE_SOLENOID); }
    // check fire temp, set solenoid
    if (fireTemp > fireAvg + 2)
      { PORTB |= (1<<FIRE_SOLENOID); }
    else if (fireTemp < fireAvg - 2)
      { PORTB &= ~(1<<FIRE_SOLENOID); }
    // debug
    #ifdef DEBUG
      Serial.print(" | i "); Serial.print(iceTemp);
      Serial.print(" | f "); Serial.print(fireTemp);
      Serial.println();
      delay(200);
    #endif
  }
  // return
  return 0;
}
