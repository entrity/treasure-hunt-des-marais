#include "Arduino.h"
SerialC Serial;
SerialC mySerial;

char PORTA;
char PORTB;
char PORTC;
char PORTD;

int Stream::n;
char Stream::c;

void setOutgoingPacket(uint8_t tripletIndex){}