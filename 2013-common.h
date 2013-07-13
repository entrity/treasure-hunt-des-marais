#ifndef COMMON_2013_H
#define COMMON_2013_H

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define IR_MODULATION 38000
#define BAUDRATE 2400
#define WILLIAM_BAUDRATE 2400
#define MIDI_BAUDRATE 31250

#define PACKET_HEADER_TO_WILLIAM 'a'
#define PACKET_HEADER_FROM_WILLIAM 'A'

#endif