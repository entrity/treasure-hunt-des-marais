#define TEST_BUILD
unsigned char DDRB, PB1, PB0, ADMUX, ADMUX_VR, PORTB, ADCSRA, ADIF, ADSC, ADCL, ADCH;

#define ADCSRA_SETTING (0x97)
#define CLEAR_ADMUX_INPUT_CHANNEL (0xf0)
#define main mainMain
	#include "main.cpp"
#undef main

#include <iostream>
#include <cmath>
using namespace std;

int failures, assertions;

bool test(bool b)
{
	if (!b) {
		failures += 1;
	}
	assertions += 1;
	cout << (b ? 1 : 0) << endl;
	return b;
}

uint16_t last(uint16_t arr[])
{
	int i = (reading_i-1)%READINGS_CT;
	if (i<0) i = READINGS_CT - 1;
	return arr[i];
}

void outputArray(uint16_t arr[])
{
	for (int i=0; i < READINGS_CT; i++)
		cout << arr[i] << '\t';
	cout << endl;
}

void setAdc(int x) {
	ADCL = x;
	ADCH = (x >> 8);
}

void reset()
{
	for (int i=0; i < READINGS_CT; i++)
	{
		iceReadings[i] = fireReadings[i] = 0;
	}
	iceAvg = fireAvg = 0;
	reading_i = 0;
}

void pushAndTest(int ice, int fire, int iA, int iF)
{
	setAdc(ice);
	takeReading(ICE_ADMUX, iceReadings, &iceTemp, &iceAvg);
  setAdc(fire);
  takeReading(FIRE_ADMUX, fireReadings, &fireTemp, &fireAvg);
  reading_i = (reading_i + 1) % READINGS_CT;
  if (!test(iceAvg == iA)) cout << '\t' << iceAvg << endl;
	if (!test(fireAvg == iF)) cout << '\t' << fireAvg << endl;
}

void testReadingAvg()
{
	reset();
	cout << "testReadingAvg" << endl;
	pushAndTest(441,456,0,0);  //0
	pushAndTest(432,450,44,45);//1
	pushAndTest(431,450,87,90);//2
	pushAndTest(430,443,130,135);//3
	pushAndTest(429,442,173,179);//4
	pushAndTest(428,440,216,224);//5
	pushAndTest(427,430,259,268);//6
	pushAndTest(426,428,301,311);//7
	pushAndTest(425,427,344,353);//8
	pushAndTest(424,427,386,396);//9
	pushAndTest(423,426,429,439);//10
	pushAndTest(422,424,427,436);//11
}

void testTakeReadings()
{
	reset();
	cout << "testTakeReadings" << endl;
	setAdc(333); takeReadings(); test(iceAvg ==  0); test(reading_i == 1); test(last(iceReadings) == 333);
	setAdc(111); takeReadings(); test(iceAvg == 33); test(reading_i == 2); test(last(iceReadings) == 111);
	setAdc(123); takeReadings(); test(iceAvg == 44); test(reading_i == 3); test(last(iceReadings) == 123);
	setAdc(312); takeReadings(); test(iceAvg == 56); test(reading_i == 4); test(last(iceReadings) == 312);
	setAdc(321); takeReadings(); test(iceAvg == 87); test(reading_i == 5); test(last(iceReadings) == 321);
	setAdc(245); takeReadings(); test(iceAvg ==120); test(reading_i == 6); test(last(iceReadings) == 245);
	setAdc(987); takeReadings(); test(iceAvg ==144); test(reading_i == 7); test(last(iceReadings) == 987);
	setAdc(  9); takeReadings(); test(iceAvg ==243); test(reading_i == 8); test(last(iceReadings) ==   9);
	setAdc(  0); takeReadings(); test(iceAvg ==244); test(reading_i == 9); test(last(iceReadings) ==   0);
	setAdc( 87); takeReadings(); test(iceAvg ==244); test(reading_i == 0); test(last(iceReadings) ==  87);
	setAdc(456); takeReadings(); test(iceAvg ==252); test(reading_i == 1); test(last(iceReadings) == 456);
	setAdc(765); takeReadings(); test(iceAvg ==265); test(reading_i == 2); test(last(iceReadings) == 765);
}

int main()
{
	testReadingAvg();
	testTakeReadings();
	cout << "failures: " << failures << " / " << assertions << endl;
}