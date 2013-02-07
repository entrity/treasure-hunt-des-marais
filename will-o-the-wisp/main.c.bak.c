/*
	Use hardware setup from http://tthheessiiss.wordpress.com/2009/08/05/dirt-cheap-wireless/.
	USART TX -> resistor -> IR LED -> IR_MODULATOR_PIN
*/

#include <avr/io.h>

#define F_OSC 16000000UL
#define F_CPU 16000000UL
#define IR_MODULATION 38000
#define BAUDRATE 2400
#define IR_MODULATOR_PIN PB1
#define PACKET_LENGTH 8

#include <util/delay.h>


/* Set up PWM on ir-modulator pin (OC1A/PB1) */
void init_ir_modulator()
{
	// set pin as output
	DDRB |= (1 << IR_MODULATOR_PIN);
	// toggle on compare match
	TCCR1A |= (1 << COM1A0);
	// Fast PWM, using OCR1A for TOP
	TCCR1A |= (3 << WGM10);
	TCCR1B |= (3 << WGM12);
	// set prescaler fcpu/8
	TCCR1B |= (2 << CS10);
	// set compare register
	int prescaler = 8;
	uint32_t divisor = (IR_MODULATION + 1) * 2 * prescaler;
	OCR1A = (F_OSC / divisor) + (F_OSC % divisor >= divisor / 2 ? 1 : 0);
}

/* Set up UART transmission (TXD|PD1) */
void init_usart()
{
	// usart_freq = clockfreq / (16 * BAUDRATE) - 1
	unsigned long divisor = 16 * BAUDRATE;
	// set baudrate: normal equation for calculating clock, plus 1 if the remainder >= half the divisor
	// UBRR0 = (F_OSC / divisor) - 1 + ( F_OSC % divisor >= divisor / 2 ? 1 : 0 );	
	UBRR0 = 416;
	// enable tx
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	// 8-bit char, no parity, 1 stop bit
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00	);
}

/* Write a character to USART */
void usartOut(char data)
{
	// Wait until the transmitter is ready
  while ( !(UCSR0A & (1<<UDRE0)) )
  	;
  // Now write the data to USART buffer
  UDR0 = data;
}

void usartPacketOut(char packet[PACKET_LENGTH])
{
	char checksum = 0;
	int i;
	for (i=0; i < PACKET_LENGTH; i++) {
		checksum += i;
		usartOut(packet[i]);
	}
	usartOut(checksum);
}

unsigned char c;
char text[] = "ffoting\n";
int i;

int main(void)
{
	/*set up carrier wave*/
	init_ir_modulator();

	/*set up usart*/
	UBRR0 = 416;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	while (1) {
		if (i == 0) {
			_delay_ms(80);
		}
		usartOut(text[i]);
		i = (i + 1) % 8;
	}
}