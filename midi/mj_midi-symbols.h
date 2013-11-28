/*
	a map of 3bytes to an index for all midi symbols.

	adjust as follows:
		( <midi note number> - 21 ) * 3
	e.g.
		( 22 - 21 ) * 3 = 3
		noteSymbols[3] is "A#0"
*/

#ifndef MJ_MIDI_NOTE_SYMBOLS
#define MJ_MIDI_NOTE_SYMBOLS

char noteSymbols[] = "A0 A#0B0 C1 C#1D1 D#1E1 F1 F#1G1 G#1A2 A#2B2 C2 C#2D2 D#2E2 F2 F#2G2 G#2A3 A#3B3 C3 C#3D3 D#3E3 F3 F#3G3 G#3A4 A#4B4 C4 C#4D4 D#4E4 F4 F#4G4 G#4A5 A#5B5 C5 C#5D5 D#5E5 F5 F#5G5 G#5A6 A#6B6 C6 C#6D6 D#6E6 F6 F#6G6 G#6A7 A#7B7 C7 C#7D7 D#7E7 F7 F#7G7 G#7A8 A#8B8 C8 C#8D8 D#8E8 F8 F#8G8 G#8";

#endif