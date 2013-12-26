#include <Arduino.h>
#define SENSOR 				(0)
#define ICE_SOLENOID 	(4)
#define FIRE_SOLENOID (3)
#define ICE_ON 		(-3*64)
#define ICE_OFF 	(-1*64)
#define FIRE_ON 	(3*64)
#define FIRE_OFF 	(1*64)

int32_t baseline, reading, diff;

void setup()
{
	Serial.begin(9600);
	pinMode( ICE_SOLENOID, OUTPUT);
	pinMode(FIRE_SOLENOID, OUTPUT);
	for (int i = 0; i < 64; i++) { baseline += analogRead(SENSOR); delay(20); }
}

void loop()
{
	// take reading
	analogRead(SENSOR);
	reading = analogRead(SENSOR) * 64;
	// calc diff
	diff = reading - baseline;
	// update baseline
	baseline = (baseline * 63 + reading + 31) / 64;
	// debug
	Serial.print("read: ");		Serial.print(reading / 64);
	Serial.print("\tbase:"); 	Serial.print(baseline / 64);
	Serial.print("\tdiff:");	Serial.print(diff);
	Serial.println();
	// Serial.println((reading * 0.004882814f/64 - 0.5f)*100);
	// handle ice solenoid
	if (diff < ICE_ON) 	{digitalWrite(ICE_SOLENOID,HIGH); /*digitalWrite(FIRE_SOLENOID, LOW);*/ Serial.println("ICE ON");}
	if (diff > FIRE_ON) {/*digitalWrite(ICE_SOLENOID, LOW);*/ digitalWrite(FIRE_SOLENOID,HIGH); Serial.println("FIRE ON");}
	// delay
	delay(512);
}
