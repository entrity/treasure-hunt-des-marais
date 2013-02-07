#include <stdbool.h>

#define UBRR_VALUE 416 // baudrate 2400
#define PACKET_LENGTH 4 // header, plus 3 data bytes

typedef struct {
  uint8_t body[PACKET_LENGTH];
  uint8_t i;
} packet_t;

static bool b_deferUsartRx = false;

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
  while( !(UCSR0A & (1<<RXC0)) )
  	;
  //Now USART has got data from host and is available is buffer
  return UDR0;
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

/* Read byte from USART. Add to packet if indicated.
Returns true if packet is complete and checksum is OK.
If checksum is wrong, restart packet and set b_deferUsartRx */
bool usartInToPacket(packet_t * p_packet, char packetHeader)
{
  char c = usartIn();
  // ignore this char and wait for the next one if b_deferUsartRx
  if (b_deferUsartRx) {
    b_deferUsartRx = false;
    return false;
  }
  // receive header or body byte
  if ( p_packet->i < PACKET_LENGTH ) {
    if ( p_packet->i || c == packetHeader ) {
      p_packet->body[p_packet->i] = c;
      p_packet->i++;
    }
  }
  // receive checksum and verify data
  else {
    // calculate checksum
    char checksum = 0;
    int i;
    for (i=0; i < PACKET_LENGTH; i++)
      checksum += p_packet->body[i];
    p_packet->i = 0; // clear packet.i (so that the packet shall be overwritten by the next call to this function)
    if (c == checksum) // return true if this packet is complete
      return true;
    else // skip next incoming byte
      b_deferUsartRx = true;
  }
  return false;
}

/* Write header, packet, and checksum to USART */
void usartPacketOut(packet_t * p_packet)
{
  // calculate checksum and output packet (sans checksum)
  char checksum = 0;
	int i;
	for (i=0; i < PACKET_LENGTH; i++) {
		checksum += p_packet->body[i];
		usartOut(p_packet->body[i]);
	}
  // output checksum
	usartOut(checksum);
}