#ifndef MAIN_H
#define MAIN_H

#define ADMUX_VR (1<<REFS0) // AVCC with external capacitor at AREF pin
#define CONTROL_CALLBACK() { Serial.println(readTemp(CONTROL_ADMUX)); Serial.println(readTemp(ICE_ADMUX)); Serial.println(readTemp(FIRE_ADMUX)); }

#define PB0 0
#define PB1 1
#define PB2 2
#define main foo

int foo();

#endif