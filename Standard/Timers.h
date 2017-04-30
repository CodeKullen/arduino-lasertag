#pragma once

struct Timers {
	unsigned long generalTimer = 0;
	unsigned long blinkTimer = 0;
	unsigned long shootTimer = 0;
	unsigned long reloadTimer = 0;
	unsigned long reloadStepTimer = 0;

	// different delays for different actions
	const unsigned long shootTime = 400;
	const unsigned long reloadTime = 3000;
	const unsigned long reloadStepTime = 1000;
	const unsigned long stunTime = 4000; // after a hit, you are disabled for a specific time
	const unsigned long blinkFreq = 500; // after a hit, the leds blink with this frequency
	const int reloadToneLength = 100;
	const int invalidTimeLength = 1000;
	const int shootToneLength = 40;
};