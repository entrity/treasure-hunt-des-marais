#include "usart-functions.h"

/* Set up UART transmission (TXD|PD1) */
void init_usart()
{
	// set baudrate 2400
	UBRR0 = UBRR_VALUE;
	// enable tx and rx
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	// 8-bit char, no parity, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/* Read a character from USART */
char usartIn()
{
	//Wait untill a data is available
  while( !(USART_IN_READY) )
  	;
  //Now USART has got data from host and is available is buffer
  return UDR0;
}

/* Read a character from USART without checking to see if a char is avilable */
char usartInWithoutCheck()
{
  return UDR0;
}

/* Write a character to USART */
void usartOut(char data)
{
	// Wait until the transmitter is ready
  while ( !(USART_OUT_READY) )
  	;
  // Now write the data to USART buffer
  UDR0 = data;
}
