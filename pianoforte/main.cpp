#include "../2013-common.h"
#include "main.h"
#include "huart.h"
#include "song-handling.h"
#include "midi-listener.h"
#include "midi-song.h"

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../usart/usart-functions.h"

#include <util/delay.h>
#include <stdio.h>

// build stream for midi listening
SoftwareSerial mySerial(10,11);

// build handler for midi listening
MidiListener midiListener(mySerial, cb_noteOn, cb_noteOff);

// USART
FILE * uart_output;

int main()
{
  DDRD |= (1 << 5);
  PORTD |= (1<<5);

  uart_init();
  uart_output = fdevopen(uart_putchar, NULL);

  /* start sw serial */
  mySerial.begin(MIDI_BAUDRATE);

  while (1) {
    PORTD ^= (1<<5);
    _delay_ms(500);
    PORTD ^= (1<<5);
    _delay_ms(500);
    fprintf(uart_output, "hellow worl\n");
    _delay_ms(100);
    midiListener.poll();
  }
}

void transmitData(char triplet[])
{
  uart_putchar(PACKET_HEADER_TO_WILLIAM, NULL);
  char checksum = PACKET_HEADER_TO_WILLIAM;
  for (int i=0; i < 3; i++) {
    checksum += triplet[i];
    uart_putchar(triplet[i], NULL);
  }
  uart_putchar(checksum, NULL);
}
