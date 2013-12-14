

#include <iostream>
using namespace std;

#define NO_MORSE -2

char PINB, PORTB, PB4, PB3, PB2, PB1, PCINT1;

#define F_CPU 8000000UL
#define PB5 ERROR // we don't want to ever touch this pin. It is RESET
#define DASH_THRESHOLD 333 // ms that distinguishes dot from dash
#define RESET_PIN PCINT1
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4
#define DASH true
#define DOT false

// dummy methods
void _delay_ms(int x){}

/* mocking pin_changes.h */
#define PIN_CHANGES_H
bool solenoid;
int morseOutput;
inline void activateSolenoid() { solenoid = true; }
inline void outputMorse(int i) { morseOutput = i; }

#include "main.h"
#include "morse_codes.h"
#include "strings.h"
#include "operations.h"

int fail = 0;


// vars from main.cpp
volatile char charBuffer[CHAR_BUFFER_LEN];
volatile uint8_t charBuffer_i;
volatile mCode_t morseBuffer;
volatile bool charBufferUpdated;

void setCharBuffer(string s) {
	memcpy((void *) &charBuffer, s.c_str(), s.length());
	charBuffer_i = s.length();
}

inline void resetTest()
{
	solenoid = false;
	morseOutput = NO_MORSE;
}

void test(bool b)
{
	cout << b << endl;
	if (!b) fail ++;
}

void testMCodes()
{
	cout << "testMCodes" << endl;

	mCode_t a = { 'a', {0,1,0}, 3 };

	cout << (a.c == 'a') << endl;
	cout << (a.code[0] == 0) << endl;
	cout << (a.code[1] == 1) << endl;
	cout << (a.code[2] == 0) << endl;
	cout << (a.code[3] == 0) << endl;
	cout << (a.code[4] == 0) << endl;
	cout << (a.n == 3) << endl;

	mCode_t b = { 'b', {0,1}, 3 };
	mCode_t c = { 'b', {0,1}, 2 };
	mCode_t d = { 'b', {1,1}, 3 };

	cout << (mCodesMatch(&a, &b)) << endl;
	cout << (mCodesMatch(&a, &c) == 0) << endl;
	cout << (mCodesMatch(&a, &d) == 0) << endl;
}

void test_InterpretMorse()
{
	cout << "test_InterpretMorse" << endl;

	mCode_t a = { 'x', {0,1}, 2 };
	char c = interpretMorse(&a);
	test(c == 'a');
	
	mCode_t a2 = { 'a', {1,0}, 2 };
	c = interpretMorse(&a2);
	cout << (c == 'n') << endl;

	mCode_t a3 = { 'a', {1,0,1,1,1}, 5 };
	c = interpretMorse(&a3);
	cout << (c == 0) << endl;

	mCode_t a4 = { 'a', {1,1,0,0}, 4 };
	c = interpretMorse(&a4);
	cout << (c == 'z') << endl;
}

void toMorse_test()
{
	cout << "toMorse_test" << endl;

	mCode_t * p;
	p = toMorse('d');

	test(p->code[0] == 1);
	test(p->code[1] == 0);
	test(p->code[1] == 0);
	test(p->n == 3);
	test(p->c == 'd');
}

void findCharBufferMatch_test()
{
	cout << "findCharBufferMatch_test" << endl;

	string a = "july";
	string b = "foo thios matches none";
	string c = "dew";
	
	setCharBuffer(a);
	test(findCharBufferMatch() == 0);
	setCharBuffer(b);
	test(findCharBufferMatch() == -1);
	setCharBuffer(c);
	test(findCharBufferMatch() == 1);
}

void processChars_test()
{
	cout << "processChars_test" << endl;

	string a = "july";
	string b = "foo this matches none";
	string c = "dew";

	resetTest();
	test(solenoid == false);
	test(morseOutput == NO_MORSE);
	setCharBuffer(a);
	resetTest(); processChars();
	test(solenoid == true);
	test(morseOutput == NO_MORSE);
	setCharBuffer(b);
	resetTest(); processChars();
	test(solenoid == false);
	test(morseOutput == NO_MORSE);
	setCharBuffer(c);
	resetTest(); processChars();
	test(solenoid == false);
	test(morseOutput == 1);
}

void circularBuffer_test()
{
	cout << "circularBuffer_test" << endl;
	setCharBuffer("bazqux");
	test(charBuffer_i == 6);
	test(charBuffer[charBuffer_i-1]=='x');
	test(charBufferMatchesString("qux"));
	test(charBufferMatchesString("zqux"));
	test(charBufferMatchesString("bazqux"));
	test( ! charBufferMatchesString("quxr"));
	test( ! charBufferMatchesString("qrux"));
	charBuffer[0] = 'a';
	charBuffer[1] = 'r';
	charBuffer[2] = 'c';
	charBuffer[3] = 't';
	charBuffer_i = 4;
	char * s = "arct";
	test(charBufferMatchesString(s));
	setCharBuffer("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	test(charBuffer_i == 36);
	pushCharBuffer('a');
	test(charBuffer_i == 37);
	test(charBuffer[36] == 'a');
}

void interpretMorseBuffer_test()
{
	cout << "interpretMorseBuffer_test" << endl;
	interpretMorseBuffer(); // reset morseBuffer
	pushMorseBuffer(DOT);
	pushMorseBuffer(DOT);
	pushMorseBuffer(DOT);
	test(interpretMorseBuffer() == 's');
	pushMorseBuffer(DOT);
	pushMorseBuffer(DASH);
	test(interpretMorseBuffer() == 'a');
	pushMorseBuffer(DOT);
	test(interpretMorseBuffer() == 'e');
	pushMorseBuffer(DASH);
	pushMorseBuffer(DOT);
	pushMorseBuffer(DASH);
	pushMorseBuffer(DASH);
	test(interpretMorseBuffer() == 'y');
}

void pushMorseBuffer_test()
{
	cout << "pushMorseBuffer_test" << endl;
	test(morseBuffer.n == 0);
	pushMorseBuffer(DOT);
	test(morseBuffer.n == 1);
	test(morseBuffer.code[morseBuffer.n-1] == DOT);
	pushMorseBuffer(DASH);
	test(morseBuffer.n == 2);
	test(morseBuffer.code[morseBuffer.n-1] == DASH);
}

int main()
{
	testMCodes();	
	test_InterpretMorse();
	toMorse_test();
	findCharBufferMatch_test();
	processChars_test();
	circularBuffer_test();
	interpretMorseBuffer_test();
	pushMorseBuffer_test();
	cout << endl << "FAILURES: " << fail << endl << endl;
	return 0;
}