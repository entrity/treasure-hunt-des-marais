#ifndef MAIN_H
#define MAIN_H

#define DEBUG
#define ADMUX_VR (1<<REFS0) // AVCC with external capacitor at AREF pin

#define PB0 0
#define PB1 1
#define PB2 2
#define main foo

#include <Arduino.h>
int foo();
void setup() { Serial.begin(9600); foo(); }
void loop() {}

#endif