#include "2013-common.h"
#include "usart-functions.h"
#include <util/delay.h>
#include <avr/io.h>
#include <Arduino.h>

int main()
{
  DDRB |= (1 << PB0);
  PORTB |= (1 << PB0);
  while (1) {
    _delay_ms(200);
    PORTB ^= (1 << 0);
  }
}