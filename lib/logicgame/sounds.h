#ifndef sounds_h
#define sounds_h

void initializeSounds();

bool isSoundPlaying();

void startPlaying (uint8_t track);

void stopPlaying ();

void soundTick ();

#endif