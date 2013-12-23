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
#define PINB PIND
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4
#define DASH true
#define DOT false

#include "d:/dev/avr/treasure-hunt-2013/telegraph/main.h"
bool morseFlag;
bool onTimerFlag;
bool offTimerFlag;
volatile bool startOffTimerFlag;
extern volatile bool value;

inline void startOnTimer()
{
  TIFR0 = (1<<OCF0A);
  TCNT0 = 0;
  TIMSK0 = (1<<OCIE0A); 
}

inline void startOffTimer()
{
  TIFR1 = (1<<OCF1A); // clear interrupt flag by writing logical 1
  TCNT1 = 0; // clear timer/counter
  TIMSK1 = (1<<OCIE1A); // enable interrupt
}

inline bool onTimerIsRunning()
{
  return TIMSK0;
}

inline void killOnTimer()
{
  TIMSK0 = 0;
  keyOnTimerCt = 0;
}

inline void killOffTimer()
{
  TIMSK1 = 0;
  keyOffTimerCt = 0;
}

inline void killTimers()
{
  killOnTimer();
  killOffTimer();
}

inline void configureTimers()
{
  // configure but do not enable timer interrupt for keyOff:
  TCCR1A = 0; // normal port operation, OC pins disconnected, CTC on OCRA
  TCCR1B = (1<<WGM12); // normal port operation, OC pins disconnected, CTC on OCRA
  TCCR1B |= (1<<CS12) | (1<<CS10); // 1024 prescaler
  OCR1A = (155); // (F_CPU / prescaler * 100) - 1 -- every 1/100th of a sec
  // configure but do not enable timer interrupt for keyOn:
  TCCR0A = (1<<WGM01); // normal port, OC disconnected, CTC on OCR0A
  TCCR0B = (1<<CS02) | (1<<CS00); // 1024 prescaler
  OCR0A = (155);
}

void setup()
{
  // enable output on solenoid and piezo
  DDRB = (1<<SOLENOID_PIN) | (1<<PIEZO_PIN);
  // configure trigger interrupt (INT0) on any change
  EIMSK = (1<<INT0);
  EICRA = (1<<ISC00);
  configureTimers();
  // disable timers
  killTimers();
  // enable global interrupts
  sei();
  // setup Serial
  Serial.begin(9600);
}

void loop() {
  #ifdef DEBUG
    if (morseFlag) {
      morseFlag = false;
      Serial.println(morseBuffer.code[morseBuffer.n - 1]);
    }
  #endif
  if (charBufferUpdated) {
    charBufferUpdated = false;
    #ifdef DEBUG
      for(int i=0; i < CHAR_BUFFER_LEN; i++) {
        Serial.print(charBuffer[i]);
      }
      Serial.println();
    #endif
    processChars(); // check & handle match
  }
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
  #define main main85
    #include "d:/dev/avr/treasure-hunt-2013/telegraph/main.cpp"
  #undef main

  #include "d:/dev/avr/treasure-hunt-2013/telegraph/morse_codes.cpp"
  #include "d:/dev/avr/treasure-hunt-2013/telegraph/strings.cpp"
#undef delayMicroseconds
