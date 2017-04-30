// 
// 
// 

#include "Definitions.h"
#include "SoundEffects.h"


void playSiren(int start, int lower, int upper, int delta, int duration) {
	int siren = start;
	int sign = 1;
	unsigned long sirenTimer = millis();

	while (millis() - sirenTimer < duration) {

		digitalWrite(BLINKPIN, HIGH);
		delayMicroseconds(siren / 2);

		digitalWrite(BLINKPIN, LOW);
		delayMicroseconds(siren / 2);

		siren += sign*delta;
		if (siren > upper)
			sign *= -1;
		if (siren < lower)
			sign *= -1;
	}
}

void playDots(int frequency, int signal, int gap, int number) {
	for (int i = 0; i < number; i++) {
		unsigned long timer = millis();
		while (millis() - timer < signal) {

			digitalWrite(BLINKPIN, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(BLINKPIN, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}

void playDudel(int min, int max, int signal, int gap, int number) {
	int frequency = 0;
	for (int i = 0; i < number; i++) {
		unsigned long timer = millis();
		frequency = random(min, max);
		while (millis() - timer < signal) {

			digitalWrite(BLINKPIN, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(BLINKPIN, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}

void playKhrrek(int min, int max, int signal, int gap, int number) {
	int frequency = 0;
	for (int i = 0; i < number; i++) {
		unsigned long timer = millis();
		while (millis() - timer < signal) {
			frequency = random(min, max);

			digitalWrite(BLINKPIN, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(BLINKPIN, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}