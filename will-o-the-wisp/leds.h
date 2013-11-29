#define redValueTarget (tx_packet.body[1])
#define greenValueTarget (tx_packet.body[2])
#define blueValueTarget (tx_packet.body[3])

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
    RED_VALUE_TCR = redValueTarget;
  if (greenValueTarget != GREEN_VALUE_TCR)
    GREEN_VALUE_TCR = greenValueTarget;
  if (blueValueTarget != BLUE_VALUE_TCR)
    BLUE_VALUE_TCR = blueValueTarget;
}