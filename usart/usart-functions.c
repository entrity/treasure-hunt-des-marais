#include "usart-functions.h"

/* Read byte from USART. Add to packet if indicated.
Returns true if packet is complete and checksum is OK.
If checksum is wrong, restart packet and set b_deferUsartRx */
bool usartInToPacket(packet_t * p_packet, char packetHeader, bool skipCheck)
{
  char c = skipCheck ? usartInWithoutCheck() : usartIn();
  // ignore this char and wait for the next one if b_deferUsartRx
  if (b_deferUsartRx) {
    b_deferUsartRx = false;
    return false;
  }
  // receive header or body byte
  if ( p_packet->i < PACKET_LENGTH ) { // packet not filled yet
    if ( p_packet->i || c == packetHeader ) { // packet header has already been received or matches this byte
      p_packet->body[p_packet->i] = c;
      p_packet->i++;
    }
    return false;
  }
  // receive checksum and verify data
  else {
    // calculate checksum
    int i;
    char checksum = 0;
    for (i=0; i < PACKET_LENGTH; i++)
      checksum += p_packet->body[i];
    // clear packet.i (so that the packet shall be overwritten by the next call to this function)
    p_packet->i = 0;
    // return true if this packet is complete
    if (c == checksum)
      return true;
    // skip next incoming byte
    else {
      b_deferUsartRx = true;
      return false;
    }
  }
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