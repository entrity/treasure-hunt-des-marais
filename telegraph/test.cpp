#include "morse_code.h"
#include <iostream>
using namespace std;

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
	cout << (c == 'a') << endl;
	
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

int main()
{
	testMCodes();	
	test_InterpretMorse();
	return 0;
}