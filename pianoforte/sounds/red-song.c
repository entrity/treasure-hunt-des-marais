MjMidi::event red_song_midiEvents[] = {
	{  0,MIDI_NOTE_ON , 64},
	{480,MIDI_NOTE_OFF, 64},
};

MjMidi::Song red_song(&red_song_midiEvents[0], 12);
