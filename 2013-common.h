#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define IR_MODULATION 38000
#define BAUDRATE 2400
#define IR_MODULATOR_PIN PB1

#define PACKET_HEADER_TO_WILLIAM 'a'
#define PACKET_HEADER_FROM_WILLIAM 'A'

#include "../usart-functions.h"
