#ifndef __main_h
#define __main_h

#define RED_STATUS_LED      0
#define GREEN_STATUS_LED    1
#define BLUE_STATUS_LED     2
#define YELLOW_STATUS_LED   3
#define ORANGE_STATUS_LED   4
#define FAILURE_STATUS_LED  5

void transmitPacket();

void setOutgoingPacket(uint8_t tripletIndex);

inline void setStatusLed(uint8_t tripletIndex) {
  switch(tripletIndex) {
    case 0:
      PORTB |= (1<<tripletIndex); break;
    case 1:
    case 2:
    case 3:
    case 4:
      PORTB |= (1 << (tripletIndex+1)); break;
    case 5:
    case 6:
      PORTD |= (1<<tripletIndex); break;
  }
}

inline void clearStatusLeds() {
  PORTB &= ~(0b00111101);
  PORTD &= ~(0b01100000);
}

#endif