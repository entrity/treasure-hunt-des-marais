#ifndef RX_H
#define RX_H

#include "tx.h"

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
  for (i = 0; i < COLOUR_TRIPLET_N; i++) {
    if (packetMatchesColourTriplet(colourTriplets[i]))
      return true;
  }
  return false;
}

ISR(USART_RX_vect)
{
  int i;
  #ifdef DEBUG
    char c;
  #endif
  if (usartInToPacket(&rx_packet,PACKET_HEADER_TO_WILLIAM,true)) {
    if (packetMatchesAnyColourTriplet()) {
      setTxPacket(&rx_packet.body[1]);
    #ifdef DEBUG
      usartOut(2);
    #endif
    }
  }
  #ifdef DEBUG
    // else { usartOut(3); }
    c = UDR0;
    usartOut(c);
  #endif
  // PORTB ^= (1 << 0); // diagnostic
}

#endif