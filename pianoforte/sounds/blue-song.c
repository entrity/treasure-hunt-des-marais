MjMidi::event blue_song_midiEvents[] = {
	{  0,MIDI_NOTE_ON , 68},
	{480,MIDI_NOTE_OFF, 68},
};

MjMidi::Song blue_song(&blue_song_midiEvents[0], 2);
