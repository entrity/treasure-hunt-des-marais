MjMidi::event yellow_song_midiEvents[] = {
	{  0,MIDI_NOTE_ON , 67},
	{480,MIDI_NOTE_OFF, 67},
};

MjMidi::Song yellow_song(&yellow_song_midiEvents[0], 12);
