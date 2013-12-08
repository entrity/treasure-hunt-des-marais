#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>


#define DEBUG 1
#define USE_ARDUINO
#define PB2 2
#define PB3 3
#define PB4 4
#define PB6 ERROR
#define PB5 ERROR // we don't want to ever touch this pin. It is RESET
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4

bool timerFlag;

inline void startTimer()
{
  TIFR1 = (1<<OCF1A); // clear interrupt flag by writing logical 1
  TCNT1 = 0; // clear timer/counter
  TIMSK1 = (1<<OCIE1A); // enable interrupt
}

inline void disableTimer()
{
  TIMSK1 = 0;
}

void reset();

void setup()
{
  reset();
  // enable output on solenoid and piezo
  DDRB = (1<<SOLENOID_PIN) | (1<<PIEZO_PIN);
  // configure trigger interrupt (INT0) on any change
  EIMSK = (1<<INT0);
  EICRA = (1<<ISC00);
  // configure but do not enable timer interrupt:
  TCCR1A = 0; TCCR1B = (1<<WGM12); // normal port operation, OC pins disconnected, CTC on OCRA
  TCCR1B |= (1<<CS12) | (1<<CS10); // 1024 prescaler
  OCR1A = (3125); // F_CPU / prescaler * (DASH_THRESHOLD/1000)
  // disable timer
  disableTimer();
  // enable global interrupts
  sei();
  // setup Serial
  Serial.begin(9600);
}

void loop() {

}


char dummyChar;
#define TIMSK dummyChar
#define TIFR dummyChar
#define OCF1A dummyChar
#define OCR1A dummyChar
#define OCR1C dummyChar
#define TCNT1 dummyChar
#define OCIE1A dummyChar
#define GIMSK dummyChar
#define CTC1 dummyChar
#define CS13 dummyChar
#define PCIE dummyChar
#define TCCR1 dummyChar
#define delayMicroseconds delayMicroseconds85
  #define startTimer startTimer85
  #define disableTimer disableTimer85
    #include "d:/dev/avr/treasure-hunt-2013/telegraph/main.h"
  #undef startTimer
  #undef disableTimer

  #define main main85
    #include "d:/dev/avr/treasure-hunt-2013/telegraph/main.cpp"
  #undef main

  #include "d:/dev/avr/treasure-hunt-2013/telegraph/morse_codes.cpp"
  #include "d:/dev/avr/treasure-hunt-2013/telegraph/strings.cpp"
#undef delayMicroseconds
