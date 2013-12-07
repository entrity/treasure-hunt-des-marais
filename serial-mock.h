#ifndef SERIAL_MOCK_H
#define SERIAL_MOCK_H

#include <cstddef>
#include <stdint.h>

extern char PORTA;
extern char PORTB;
extern char PORTC;
extern char PORTD;

class Stream
{
public:
  static int n;
  static char c;
  static int available(){ return n; }
  static char read(){ return c; }
  static void begin(int a){};
  static void write(uint8_t c){};
};

class SerialC : public Stream
{
};

extern SerialC Serial;
extern SerialC mySerial;
#endif
