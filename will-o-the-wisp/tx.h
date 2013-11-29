#ifndef TX_H
#define TX_H

inline void setTxPacket(char * triplet)
{
  int i;
  for (i=0; i< 3; i++) {
    tx_packet.body[i+1] = triplet[i];
  }
}

inline void transmitPacket()
{
  char unusedChar; // char for clearing USART data register  
  // reset counter
  count = 0;
  // clear USART data register
  unusedChar = UDR0;
  // set header for outgoing packet
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

#endif