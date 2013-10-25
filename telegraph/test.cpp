#include "morse_code.h"
#include "strings.h"
#include "operations.h"

#include <iostream>
using namespace std;

#define NO_MORSE -2

#define F_CPU 8000000UL
#define PB5 ERROR // we don't want to ever touch this pin. It is RESET
#define MORSE_BUFFER_LEN 10		// how many dot/dash to remember from trigger
#define CHAR_BUFFER_LEN 30		// how many chars to remember from trigger
#define DASH_THRESHOLD 333 // ms that distinguishes dot from dash
#define RESET_PIN PCINT1
#define TRIGGER_PIN PB2
#define PIEZO_PIN PB3
#define SOLENOID_PIN PB4
#define DASH true
#define DOT false


int fail = 0;

char * charBuffer_p;
uint8_t charBuffer_i;
bool solenoid;
int morseOutput;
mCode_t morseBuffer;

inline void resetTest()
{
	solenoid = false;
	morseOutput = NO_MORSE;
}

inline void activateSolenoid() {	solenoid = true; }

inline void outputMorse(int i) { morseOutput = i; }

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

void getMatchingInput_test()
{
	cout << "getMatchingInput_test" << endl;

	char a[] = "july";
	char b[] = "foo this matches none";
	char c[] = "dew";
	
	charBuffer_p = a;
	charBuffer_i = 4;
	test(getMatchingInput() == 0);
	charBuffer_p = b;
	charBuffer_i = strlen(b);
	test(getMatchingInput() == -1);
	charBuffer_p = c;
	charBuffer_i = strlen(c);
	test(getMatchingInput() == 1);
}

void processChars_test()
{
	cout << "processChars_test" << endl;

	char a[] = "july";
	char b[] = "foo this matches none";
	char c[] = "dew";

	charBuffer_p = a;
	charBuffer_i = strlen(a);
	resetTest(); processChars();
	test(solenoid == true);
	test(morseOutput == NO_MORSE);
	charBuffer_p = b;
	charBuffer_i = strlen(b);
	resetTest(); processChars();
	test(solenoid == false);
	test(morseOutput == NO_MORSE);
	charBuffer_p = c;
	charBuffer_i = strlen(c);
	resetTest(); processChars();
	test(solenoid == false);
	test(morseOutput == 1);
}

void misc_test()
{
	cout << "misc_test" << endl;

	uint8_t ocr = F_CPU / 16384 * DASH_THRESHOLD; // set output compare register
	test(ocr = 163);
}

int main()
{
	testMCodes();	
	test_InterpretMorse();
	toMorse_test();
	getMatchingInput_test();
	processChars_test();
	misc_test();
	cout << endl << "FAILURES: " << fail << endl << endl;
	return 0;
}