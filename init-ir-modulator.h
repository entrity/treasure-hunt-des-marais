#ifndef SETUP_IR_MODULATOR_H
#define SETUP_IR_MODULATOR_H

#ifndef IR_MODULATOR_PIN
#define IR_MODULATOR_PIN PB1
#endif

/* Set up timer 1 and PWM on ir-modulator pin (OC1A/PB1) */
inline void init_ir_modulator()
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

#endif