/*
  Use hardware setup from http://tthheessiiss.wordpress.com/2009/08/05/dirt-cheap-wireless/.
  USART TX -> resistor -> IR LED -> IR_MODULATOR_PIN
*/

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


volatile bool flagToReadFromUsart = false;
packet_t rx_packet, tx_packet;
char count;


#include "leds.h"
#include "button.h"
#include "tx.h"
#include "rx.h"

/* Main function */
int main(void)
{
  #ifdef DEBUG
    int i, j;
  #endif
  /* set up usart */
  init_usart();
  /* set up interrupt for USART RX */
  UCSR0B |= (1 << RXCIE0);
  /* init modulation for IR TX on IR_MODULATOR_PIN */
  init_ir_modulator();
  /* set up rgb leds */
  init_rgb_leds();
  /* set up button interrupt */
  init_button_interrupt();
  /* Enable global interrupts */
  sei();

  // enable pb0 for writing (for diagnostics)
  DDRB |= (1 << 0);

  // output colourTriplets
  #ifdef DEBUG
    for (i=0; i < COLOUR_TRIPLET_N; i++) {
      for (j=0; j < 3; j++) {
        usartOut(colourTriplets[i][j]);
        usartOut(5);
      }
      usartOut(99);
    }
  #endif

  /* loop: receive from USART & update *ValueTarget values */
  while (1) {
    /* Update rgb leds */
    updateLeds();
    /* Transmit packet through USART */
    #ifndef DEBUG
      write();
    #endif
    _delay_ms(200);
  }
}

