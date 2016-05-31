/*
Name:		Standard.ino
Created:	2/19/2016 4:34:36 PM
Author:	lhk
*/
#include "Protocol.h"
#include <IRLib.h>
#include <IRLibTimer.h>
#include <IRLibRData.h>
#include <IRLibMatch.h>


// IR configuration
// each IR message consists of pulses of IR light
// these pulses form a digital signal
// attention: usually, one would expect a pulse = 1, a gap = 0
// a pulse of IR light is called a mark, a gap is called a space
// here each bit in the signal is transmitted by both a mark and a space
// -> 1 is encoded as the mark for a one and the mark for a space
// -> 0 is enoceded by mark for 0 and space for 0
//
// it gets more complicated: usually only spaces are varied
// -> mark_one = mark_zero
// and at the beginning of a message are a special mark and space for the head

/*const int numBits = 4; // how many bits are send, important on the receiving side
const unsigned int Head_Mark = 6000;
const unsigned int Head_Space = 900;
const unsigned int Mark_One = 900;
const unsigned int Mark_Zero = 900;
const unsigned int Space_One = 3700;
const unsigned int Space_Zero = 900;
const unsigned int kHz = 56; // frequency of the signal, the led can emit any signal but the receiver is tuned to a specific freq
const bool Use_Stop = true; // add a stopBit to the end of the message
*/

const int recvPin = 11;
unsigned int buffer[RAWBUF];

int team = TEAM_ONE;

// Objects to send and receive IR data
IRsend Sender;
IRrecv receiver(recvPin);
IRdecode decoder;

// pins on the arduino
const int buttonPin = 2;
const int ledPin = 13;
const int blinkPin = 8;
int buttonState = 0;

// data to be transmitted by the IR led
// right now, we always send the value 3
// TODO: define different codes for different teams/players/pickups
unsigned long data = 3;

// timers
// TODO: the timer variable is used too often, refactor this into separate variables
unsigned long timer = 0;
unsigned long blinkTimer = 0;
unsigned long shootTimer = 0;
unsigned long reloadTimer = 0;
unsigned long reloadStepTimer = 0;

// different delays for different actions
unsigned long shootTime = 400;
unsigned long reloadTime = 3000;
unsigned long reloadStepTime = 1000;
unsigned long stunTime = 4000;
unsigned long blinkFreq = 500; // after a hit, the leds blink with this frequency

int roundsLeft = 5;
const int roundsMax = 5;

bool reloading = false;
bool stunned = false;
bool blink = false;

// sound effects
// the note C, for reference
const int C = 1912;

// after a hit, a siren sound is being played.
// it starts with sirenStart and goes up and down with deltaSiren
int sirenStart = C;
int deltaSiren = 3;
int deltaInvalidSiren = 8;
int reloadTone = 300;
int shootTone = 500;
int invalidSignal = 200;

int reloadToneLength = 100;
int invalidTimeLength = 1000;
int shootToneLength = 40;


void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT_PULLUP);
	pinMode(blinkPin, OUTPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);

	digitalWrite(blinkPin, HIGH);
	playDudel(100, 1000, 50, 30, 20);
	digitalWrite(13, HIGH);

	//playSiren(500, 400, 900, 1, stunTime);
	//playKhrrek(200, 300, 90, 1, 20);
}

void playSiren(int start, int lower, int upper, int delta, int duration) {
	int siren = start;
	int sign = 1;
	unsigned long sirenTimer = millis();

	while (millis() - sirenTimer < duration) {

		digitalWrite(blinkPin, HIGH);
		delayMicroseconds(siren / 2);

		digitalWrite(blinkPin, LOW);
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
		timer = millis();
		while (millis() - timer < signal) {

			digitalWrite(blinkPin, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(blinkPin, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}

void playDudel(int min, int max, int signal, int gap, int number) {
	int frequency = 0;
	for (int i = 0; i < number; i++) {
		timer = millis();
		frequency = random(min, max);
		while (millis() - timer < signal) {

			digitalWrite(blinkPin, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(blinkPin, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}

void playKhrrek(int min, int max, int signal, int gap, int number) {
	int frequency = 0;
	for (int i = 0; i < number; i++) {
		timer = millis();
		while (millis() - timer < signal) {
			frequency = random(min, max);

			digitalWrite(blinkPin, HIGH);
			delayMicroseconds(frequency / 2);

			digitalWrite(blinkPin, LOW);
			delayMicroseconds(frequency / 2);
		}
		delay(gap);
	}
}

void setTeam(int _team)
{
	team = _team;
	if (team == TEAM_ONE) {
		shootTone = 500;
		playDots(500, 500, 0, 1);
	}
	else {
		shootTone = 550;
		playDots(550, 500, 0, 1);
	}
}

void loop() {
	if (receiver.GetResults(&decoder)) {
		digitalWrite(ledPin, HIGH);

		// three outputs:
		// 1. was the decoding successful? 0 or 1
		// 2. decoded value
		// 3. IR signal data
		int val = decoder.decodeGeneric(numBits * 2 + 4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero);
		//if (val != 0) {
		Serial.println(decoder.value, HEX);
		decoder.DumpResults();

		if (val == 0) {
			playSiren(1450, 1400, 1500, 1, invalidTimeLength);
		}
		
		int hit=3;/*
		int friendly_fire;

		if (team == TEAM_ONE)
		{
			hit = TEAM_TWO_SHOT;
			friendly_fire = TEAM_ONE_SHOT;
		}
		else
		{
			hit = TEAM_ONE_SHOT;
			friendly_fire = TEAM_TWO_SHOT;
		}


		if (decoder.value == TEAM_ONE_SET)
			setTeam(TEAM_ONE);
		else if (decoder.value == TEAM_TWO_SET)
			setTeam(TEAM_TWO);
		//TODO:
		// right now a shot is signalled by sending a 3.
		// this needs to be refined
		else */if (decoder.value == hit) {
			stunned = true;
			reloading = false;
			blink = true;
			digitalWrite(blinkPin, HIGH);

			timer = millis();

			//300,200,700,3,stunTime
			//playSiren(500, 400, 900, 1, stunTime);// -- sounds like an ambulance
			//playKhrrek(100, 1000, 50, 30, 20);
			//playKhrrek(100, 300, 80, 10, 20); geiger
			playKhrrek(200, 300, 90, 1, 20);
			//playDots(6000, 100, 50, 3);
			stunned = false;
		}
		//}
		receiver.enableIRIn();
	}


	// if the player is reloading, he can't do anything
	if (reloading) {
		if (millis() - reloadTimer > reloadTime) {
			reloading = false;
			reloadTimer = millis();
			playDots(500, 300, 0, 1);
		}
		else if (millis() - reloadStepTimer > reloadStepTime) {
			reloadStepTimer = millis();
			playDots(400, 100, 0, 1);
		}
	}
	// player is neither stunned nor reloading
	else {
		buttonState = digitalRead(buttonPin);
		if (buttonState == LOW) {
			if (millis() - shootTimer > shootTime) {

				shootTimer = millis();
				//reloading = true;
				roundsLeft--;
				if (roundsLeft <= 0)
				{
					reloading = true;
					roundsLeft = roundsMax;
					reloadTimer = millis();
					reloadStepTimer = millis();
				}

				digitalWrite(ledPin, HIGH);

				// shoot
				// the only important parameter is data
				// you can send any number you want (but only numBits bits of it).
				// with 4 bits being sent, this means we can transmit 2^4=16 distinct values
				// just increase numBits if you need more
				Serial.println(millis());
				Sender.sendGeneric(data, numBits, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop, 0);
				Serial.println(millis());
				// you can't send and receive at the same time. "shooting" automatically disables the receiver
				receiver.enableIRIn();


				for (int i = 0; i < 3; i++) {
					timer = millis();
					while (millis() - timer < shootToneLength) {

						digitalWrite(blinkPin, HIGH);
						delayMicroseconds((shootTone / 8) * 7);

						digitalWrite(blinkPin, LOW);
						delayMicroseconds((shootTone / 8));
					}
					delay(50);
				}

				digitalWrite(ledPin, LOW);
			}
		}
	}
}
