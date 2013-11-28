#ifndef mj_song_handling_h
#define mj_song_handling_h

/* Set callbacks for songbank and songs */
void setupSongHandling();

// declare callbacks for midi listener
void cb_noteOn(unsigned char note, unsigned char volume);
void cb_noteOff(unsigned char note);

// declare callbacks for song bank
void cb_songBankFailure();

// declare callbacks for individual songs
void cb_redSongComplete();
void cb_greenSongComplete();
void cb_blueSongComplete();
void cb_yellowSongComplete();
void cb_orangeSongComplete();
void cb_violetSongComplete();

#endif