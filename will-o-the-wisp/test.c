#include <stdint.h>
#define bool uint8_t
#define true 1
#define false 0
#define PACKET_LENGTH 4

typedef struct {
  uint8_t body[PACKET_LENGTH];
  uint8_t i;
} packet_t;

inline void updateLeds(){}
inline void cli(){}
inline void sei(){}
inline void usartOut(char c) { printf("%d ", (unsigned char) c); }
bool usartInToPacket(packet_t * p_packet, char packetHeader, bool skipCheck){ return false; }
void usartPacketOut(packet_t * p_packet)
{
  char checksum = 0;
  int i;
  for (i=0; i < PACKET_LENGTH; i++) {
    checksum += p_packet->body[i];
    usartOut(p_packet->body[i]);
  }
  usartOut(checksum);
}

int foo;

#define UDR0 0;
#define ISR(name) void name()
#define EICRA foo
#define ISC01 foo
#define EIMSK foo
#define INT0 foo
packet_t rx_packet, tx_packet;
int fail = 0;
int count;
#define IR_MODULATION 38000
#define BAUDRATE 2400
#define WILLIAM_BAUDRATE 2400
#define MIDI_BAUDRATE 31250
#define PACKET_HEADER_TO_WILLIAM 'a'
#define PACKET_HEADER_FROM_WILLIAM 'A'

#include "colour-triplets.h"
#include "button.h"
#include "tx.h"
#include "rx.h"
#include <stdio.h>


void test(char b)
{
  printf("%d\n", b);
  if (!b) fail ++;
}

void testButtonClick()
{
  int i;
  printf("\n\ntestButtonClick\n");
  for (i=0; i < 16; i ++) {
    transmitPacket();
    INT0_vect();
    printf("\n");
  }
}

void testColourTriplets()
{
  int i,j;
  unsigned char x;
  char * p;
  for (i=0; i < COLOUR_TRIPLET_N; i++){
    p = colourTriplets[i];
    for (j=0; j<3; j++) {
      x = p[j];
      printf("%d, ", x);
    }
    printf("\n");
  }
}

int main()
{
  testColourTriplets();
  testButtonClick();
  printf("\nFAILURES: %d\n\n", fail);
  return 0;
}