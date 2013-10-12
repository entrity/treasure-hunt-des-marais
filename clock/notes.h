#ifndef NOTES_H
#define NOTES_H


typedef struct {
  unsigned int halfPrd;   // us (microseconds)
  unsigned long duration;  // us (microseconds)
} note_t;

// definitions of frequencies for notes
// http://www.richardbrice.net/midi_notes.htm
#define A0       27.500 //* Lowest note on piano
#define As0      29.135
#define B0       30.867
#define C1       32.703
#define Cs1      34.648
#define D1       36.708
#define Ds1      38.890
#define E1       41.203
#define F1       43.653
#define Fs1      46.249
#define G1       48.999
#define Gs1      51.913
#define A1       55.000
#define As1      58.270
#define B1       61.735
#define C2       65.406
#define Cs2      69.295
#define D2       73.416
#define Ds2      77.781
#define E2       82.406
#define F2       87.307
#define Fs2      92.499
#define G2       97.998
#define Gs2      103.82
#define A2       110.00
#define As2      116.54
#define B2       123.47
#define C3       130.81
#define Cs3      138.59
#define D3       146.83
#define Ds3      155.56
#define E3       164.81
#define F3       174.61
#define Fs3      184.99
#define G3       195.99
#define Gs3      207.65
#define A3       220.00
#define As3      233.08
#define B3       246.94
#define C4       261.63 //* Middle-C
#define Cs4      277.18
#define D4       293.66
#define Ds4      311.13
#define E4       329.63
#define F4       349.23
#define Fs4      369.99
#define G4       391.99
#define Gs4      415.31
#define A4       440.00
#define As4      466.16
#define B4       439.88
#define C5       523.25
#define Cs5      554.37
#define D5       587.33
#define Ds5      622.25
#define E5       659.26
#define F5       698.46
#define Fs5      739.99
#define G5       783.99
#define Gs5      830.61
#define A5       880.00
#define As5      932.32
#define B5       987.77
#define C6       1046.5
#define Cs6      1108.7
#define D6       1174.7
#define Ds6      1244.5
#define E6       1318.5
#define F6       1396.9
#define Fs6      1480.0
#define G6       1568.0
#define Gs6      1661.2
#define A6       1760.0
#define As6      1864.7
#define B6       1975.5
#define C7       2093.0
#define Cs7      2217.5
#define D7       2349.3
#define Ds7      2489.0
#define E7        2637.0
#define F7        2793.8
#define Fs7       2960.0
#define G7        3136.0
#define Gs7       3322.4
#define A7        3520.0
#define As7       3729.3
#define B7        3951.1
#define C8        4186.0 //* Highest note on piano

#endif