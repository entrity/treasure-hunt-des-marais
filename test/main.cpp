#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);
bool lit;
char c;
int i;

int main()
{
  Serial.begin(31250);
  mySerial.begin(2400);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

  while (1) {
    int available = Serial.available();
    
    if (available){
      i = Serial.read();
      digitalWrite(5, HIGH);  
      mySerial.println(i);
    }

    // mySerial.print("available: ");
    // mySerial.println(available);

    lit = !lit;
    char dp = lit ? HIGH : LOW;
    digitalWrite(4, dp);
  }

}