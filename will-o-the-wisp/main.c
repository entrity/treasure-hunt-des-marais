/*
  Use hardware setup from http://tthheessiiss.wordpress.com/2009/08/05/dirt-cheap-wireless/.
  USART TX -> resistor -> IR LED -> IR_MODULATOR_PIN
*/

#define F_CPU 16000000UL

#include "2013-common.h"
#include "colour-triplets.h"
#include "../usart/usart-functions.h"
#include <util/delay.h>
#include "../init-ir-modulator.h"

#define IR_MODULATOR_PIN PB1
#define INITIAL_RED_VALUE 30
#define INITIAL_GREEN_VALUE 70
#define INITIAL_BLUE_VALUE 100
#define RED_VALUE_TCR OCR0A
#define GREEN_VALUE_TCR OCR0B
#define BLUE_VALUE_TCR OCR2A

// volatile unsigned char // hold values read from USART
//   redValueTarget = INITIAL_RED_VALUE,
//   greenValueTarget = INITIAL_GREEN_VALUE,
//   blueValueTarget = INITIAL_BLUE_VALUE;
#define redValueTarget (tx_packet.body[1])
#define greenValueTarget (tx_packet.body[2])
#define blueValueTarget (tx_packet.body[3])
volatile bool flagToReadFromUsart = false;
packet_t rx_packet, tx_packet;
char count;

/* Initialize timer 0 and timer 2 */
inline void init_rgb_leds()
{
  // set pins as output (OC0A, OC0B, OC2A)
  DDRD |= (1 << PD6); // OC0A (RED)
  DDRD |= (1 << PD5); // OC0B (GREEN)
  DDRB |= (1 << PB3); // OC2A (BLUE)
  // set on BOTTOM, clear on compare match
  TCCR0A |= (1 << COM0A1); // timer 0, channel A (red)
  TCCR0A |= (1 << COM0B1); // timer 0, channel B (greem)
  TCCR2A |= (1 << COM2A1); // timer 2, channel A (blue)
  // Fast PWM, using OCRXY for compare, 0xff for TOP
  TCCR0A |= (1 << WGM01) | (1 << WGM00); // timer 0
  TCCR2A |= (1 << WGM21) | (1 << WGM20); // timer 2
  // set prescaler fcpu/1
  TCCR0B |= (1 << CS00); // timer 0
  TCCR2B |= (1 << CS20); // timer 2
  // set the compare register to its initial value
  RED_VALUE_TCR = INITIAL_RED_VALUE;
  GREEN_VALUE_TCR = INITIAL_GREEN_VALUE;
  BLUE_VALUE_TCR = INITIAL_BLUE_VALUE;
}

/* Increment the colours of the leds toward the current value of c */
inline void updateLeds()
{
  if (redValueTarget != RED_VALUE_TCR)
    // RED_VALUE_TCR += redValueTarget > RED_VALUE_TCR ? 1 : -1;
    RED_VALUE_TCR = redValueTarget;
  if (greenValueTarget != GREEN_VALUE_TCR)
    // GREEN_VALUE_TCR += greenValueTarget > GREEN_VALUE_TCR ? 1 : -1;
    GREEN_VALUE_TCR = greenValueTarget;
  if (blueValueTarget != BLUE_VALUE_TCR)
    // BLUE_VALUE_TCR += blueValueTarget > BLUE_VALUE_TCR ? 1 : -1;
    BLUE_VALUE_TCR = blueValueTarget;
}

inline void transmitPacket()
{
  char unusedChar; // char for clearing USART data register  
  // reset counter
  count = 0;
  // clear USART data register
  unusedChar = UDR0;  // set header for outgoing packet
  tx_packet.body[0] = PACKET_HEADER_FROM_WILLIAM;
  // transmit
  usartPacketOut(&tx_packet);  
  // clear USART data register
  unusedChar = UDR0;  
}

void write()
{
  cli(); // disable global interrupts
  transmitPacket();
  sei(); // enable global interrupts
}


/* Main function */
int main(void)
{
  /* set up usart */
  init_usart();
  /* set up interrupt for USART RX */
  UCSR0B |= (1 << RXCIE0);
  /* init modulation for IR TX on IR_MODULATOR_PIN */
  init_ir_modulator();
  /* set up rgb leds */
  init_rgb_leds();
  /* Enable global interrupts */
  sei();

  // enable pb0 for writing (for diagnostics)
  DDRB |= (1 << 0);

  /* loop: receive from USART & update *ValueTarget values */
  while (1) {
    /* Update rgb leds */
    updateLeds();
    /* Transmit packet through USART */
    write();
    _delay_ms(200);
  }
}

inline bool packetMatchesColourTriplet(char * triplet)
{
  return rx_packet.body[1] == triplet[0] &&
  rx_packet.body[2] == triplet[1] &&
  rx_packet.body[3] == triplet[2];
}

// return true if packet matches any array in colour-triplets.c
bool packetMatchesAnyColourTriplet()
{
  int i;
  for (i = 0; i < 6; i++) {
    if (packetMatchesColourTriplet(colourTriplets[i]))
      return true;
  }
  return false;
}

ISR(USART_RX_vect)
{
  int i;
  if (usartInToPacket(&rx_packet,PACKET_HEADER_TO_WILLIAM,true)) {
    if (packetMatchesAnyColourTriplet()) {
      for (i = 1; i < 4; i++)
        tx_packet.body[i] = rx_packet.body[i];
    }
  }
  // PORTB ^= (1 << 0); // diagnostic
}