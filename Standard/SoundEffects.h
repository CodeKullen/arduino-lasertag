// SoundEffects.h

#ifndef _SOUNDEFFECTS_h
#define _SOUNDEFFECTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void playSiren(int start, int lower, int upper, int delta, int duration);
void playDots(int frequency, int signal, int gap, int number);
void playDudel(int min, int max, int signal, int gap, int number);
void playKhrrek(int min, int max, int signal, int gap, int number);
#endif

