#ifndef SETUP_IR_MODULATOR_H
#define SETUP_IR_MODULATOR_H

#ifndef IR_MODULATOR_PIN
#define IR_MODULATOR_PIN PB1
#endif

/* Set up timer 1 and PWM on ir-modulator pin (OC1A/PB1) */
inline void init_ir_modulator()
{
  // set pin as output
  DDRB |= (1 << PB1);
  
  // CTC, using OCR1A for TOP
  TCCR1B = (1 << WGM12);
  
  // toggle output pin on compare match
  TCCR1A = (1 << COM1A0);
  
  // set prescaler fcpu/1
  TCCR1B |= (1 << CS10);
  
  // set compare register
  OCR1A = 210;
}

#endif