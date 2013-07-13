#ifndef __main_h
#define __main_h

#define RED_STATUS_LED      0
#define GREEN_STATUS_LED    1
#define BLUE_STATUS_LED     2
#define YELLOW_STATUS_LED   3
#define ORANGE_STATUS_LED   4
#define FAILURE_STATUS_LED  5

// declare method for IR TX
void transmitData(char * p_triplet);

inline void setStatusLed(uint8_t pin) { PORTC |= (1<<pin); }

#endif