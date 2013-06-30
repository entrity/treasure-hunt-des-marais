MjMidi::event green_song_midiEvents[] = {
	{  0,MIDI_NOTE_ON , 66},
	{480,MIDI_NOTE_OFF, 66},
};

MjMidi::Song green_song(&green_song_midiEvents[0], 12);
