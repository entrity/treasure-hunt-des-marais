#ifndef BUTTON_H
#define BUTTON_H
#include "tx.h"

uint8_t outgoingPacketOverride;

inline void init_button_interrupt()
{
  EICRA |= (1<<ISC01); // falling edge on INT0 creates interrupt
  EIMSK |= (1<<INT0); // enable interrupt request 0
}

ISR(INT0_vect)
{
  outgoingPacketOverride = (outgoingPacketOverride + 1) % COLOUR_TRIPLET_N;
  setTxPacket(colourTriplets[outgoingPacketOverride]);
}

#endif