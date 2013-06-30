MjMidi::event orange_song_midiEvents[] = {
	{  0,MIDI_NOTE_ON , 65},
	{480,MIDI_NOTE_OFF, 65},
};

MjMidi::Song orange_song(&orange_song_midiEvents[0], 12);
