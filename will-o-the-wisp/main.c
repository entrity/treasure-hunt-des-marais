/*
  Use hardware setup from http://tthheessiiss.wordpress.com/2009/08/05/dirt-cheap-wireless/.
  USART TX -> resistor -> IR LED -> IR_MODULATOR_PIN
*/

#include "2013-common.h"
#include "usart-functions.h"
#include <util/delay.h>

#define IR_MODULATOR_PIN PB1
#define INITIAL_RED_VALUE 10
#define INITIAL_GREEN_VALUE 10
#define INITIAL_BLUE_VALUE 10
#define RED_VALUE_TCR OCR0A
#define GREEN_VALUE_TCR OCR0B
#define BLUE_VALUE_TCR OCR2A

inline void input();
inline void output();
volatile unsigned char redValueTarget, greenValueTarget, blueValueTarget; // hold values read from USART
volatile bool flagToReadFromUsart = false;
packet_t packet;
char count;

/* Set up timer 1 and PWM on ir-modulator pin (OC1A/PB1) */
void init_ir_modulator()
{
  // set pin as output
  DDRB |= (1 << IR_MODULATOR_PIN);
  // toggle output pin on compare match
  TCCR1A |= (1 << COM1A0);
  // Fast PWM, using OCR1A for TOP
  TCCR1A |= (3 << WGM10);
  TCCR1B |= (3 << WGM12);
  // set prescaler fcpu/8
  TCCR1B |= (2 << CS10); // if this changes, the variable 'prescaler' needs to change too
  // set compare register
  int prescaler = 8; 
  uint32_t divisor = (IR_MODULATION + 1) * 2 * prescaler;
  OCR1A = (F_CPU / divisor) + (F_CPU % divisor >= divisor / 2 ? 1 : 0);
}

/* Initialize timer 0 and timer 2 */
void init_rgb_leds()
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
void updateLeds()
{
  if (redValueTarget != RED_VALUE_TCR)
    RED_VALUE_TCR += redValueTarget > RED_VALUE_TCR ? 1 : -1;
  if (greenValueTarget != GREEN_VALUE_TCR)
    GREEN_VALUE_TCR += greenValueTarget > GREEN_VALUE_TCR ? 1 : -1;
  if (blueValueTarget != BLUE_VALUE_TCR)
    BLUE_VALUE_TCR += blueValueTarget > BLUE_VALUE_TCR ? 1 : -1;
}

inline void transmitPacket()
{
  char unusedChar; // char for clearing USART data register
  // disable global interrupts
  cli();
  // reset counter
  count = 0;
  // clear USART data register
  unusedChar = UDR0;
  // set header for outgoing packet
  packet.body[0] = PACKET_HEADER_FROM_WILLIAM;
  // transmit
  usartPacketOut(&packet);  
  // clear USART data register
  unusedChar = UDR0;
  // enable global interrupts
  sei();
}

/* Main function */
int main(void)
{
  /*set up carrier wave*/
  init_ir_modulator();
  /*set up usart*/
  init_usart();
  /* set up interrupt for USART RX */
  UCSR0B |= (1 << RXCIE0);
  /* set up timers for leds on PWM */
  init_rgb_leds();
  /* Enable global interrupts */
  sei();

  /* loop: receive from USART & update *ValueTarget values */
  while (1) {
    /* update rgb leds */
    updateLeds();
    /* Read from USART if flag is set */
    input();
    /* Transmit packet through USART */
    output();
  }
}

void input()
{
  /* read from usart if usart rx flag is set */
  if (flagToReadFromUsart) {
    if (usartInToPacket(&packet,PACKET_HEADER_TO_WILLIAM)) {
      redValueTarget = packet.body[1];
      greenValueTarget = packet.body[2];
      blueValueTarget = packet.body[3];
    }
    flagToReadFromUsart = false;
  }
  
}

void output()
{
  /* count and transmit */
  _delay_ms(80);
  // cli();
  // UCSR0A |= (1 << UDRE0);
  transmitPacket();
  // sei();
}

ISR(USART_RX_vect) 
{
  flagToReadFromUsart = true;
}