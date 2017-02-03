/*
Name:		Standard.ino
Created:	2/19/2016 4:34:36 PM
Author:	lhk
*/
#include "Definitions.h"
#include "Timers.h"
#include "SoundEffects.h"
#include <IRLib.h>
#include <IRLibTimer.h>
#include <IRLibRData.h>
#include <IRLibMatch.h>



int team = TEAM_ONE;

// Objects to send and receive IR data
unsigned int buffer[RAWBUF];
IRsend Sender;
IRrecv receiver(RECVPIN);
IRdecode decoder;

// data to be transmitted by the IR led
// right now, we always send the value 3
// TODO: define different codes for different teams/players/pickups
unsigned long data = 3;


int buttonState = 0;

Timers timers = Timers();

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


void setup() {
	pinMode(LEDPIN, OUTPUT);
	pinMode(BUTTONPIN, INPUT_PULLUP);
	pinMode(BLINKPIN, OUTPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);

	digitalWrite(BLINKPIN, HIGH);
	playDudel(100, 1000, 50, 30, 5);

	digitalWrite(LEDPIN, LOW);

	//playSiren(500, 400, 900, 1, stunTime);
	//playKhrrek(200, 300, 90, 1, 20);
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

		// three outputs:
		// 1. was the decoding successful? 0 or 1
		// 2. decoded value
		// 3. IR signal data
		int val = decoder.decodeGeneric(numBits * 2 + 4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero);
		//if (val != 0) {
		Serial.println(decoder.value, HEX);
		decoder.DumpResults();

		if (val == 0) {
			playSiren(1450, 1400, 1500, 1, timers.invalidTimeLength/2);

			for (int i = 0; i < 5; i++) {
				digitalWrite(LEDPIN, HIGH);
				delay(100);
				digitalWrite(LEDPIN, LOW);
				delay(75);
			}
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
			digitalWrite(BLINKPIN, HIGH);

			timers.generalTimer = millis();

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
		if (millis() - timers.reloadTimer > timers.reloadTime) {
			reloading = false;
			timers.reloadTimer = millis();
			playDots(500, 300, 0, 1);
		}
		else if (millis() - timers.reloadStepTimer > timers.reloadStepTime) {
			timers.reloadStepTimer = millis();
			playDots(400, 100, 0, 1);
		}
	}
	// player is neither stunned nor reloading
	else {
		buttonState = digitalRead(BUTTONPIN);
		if (buttonState == LOW) {
			if (millis() - timers.shootTimer > timers.shootTime) {

				timers.shootTimer = millis();
				//reloading = true;
				roundsLeft--;
				if (roundsLeft <= 0)
				{
					reloading = true;
					roundsLeft = roundsMax;
					timers.reloadTimer = millis();
					timers.reloadStepTimer = millis();
				}

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
					timers.generalTimer = millis();
					while (millis() - timers.generalTimer < timers.shootToneLength) {

						digitalWrite(BLINKPIN, HIGH);
						delayMicroseconds((shootTone / 8) * 7);

						digitalWrite(BLINKPIN, LOW);
						delayMicroseconds((shootTone / 8));
					}
					delay(50);
				}
			}
		}
	}
}
