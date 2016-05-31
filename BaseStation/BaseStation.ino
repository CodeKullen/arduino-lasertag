/*
 Name:		BaseStation.ino
 Created:	4/12/2016 7:01:30 PM
 Author:	lhk
*/

#include <IRLib.h>
#include <IRLibTimer.h>
#include <IRLibRData.h>
#include <IRLibMatch.h>
#include "Protocol.h"


IRsend Sender;
IRdecode decoder;

// \TODO define with real pin
const int teamPin = 2;

// \TODO define with real pin
const int sendButtonPin = 11;
const int ledPin = 13;
bool enableSend = false;
// millis() when send button was pressed.
long timer = 0;
int duration = 1000; // in ms

// the setup function runs once when you press reset or power the board
void setup() {

	pinMode(teamPin, INPUT_PULLUP);
	pinMode(sendButtonPin, INPUT_PULLUP);
	pinMode(ledPin, OUTPUT);

	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {

	if (digitalRead(sendButtonPin) == LOW) {
		int data = (digitalRead(teamPin) == LOW) ? TEAM_ONE_SET : TEAM_TWO_SET;
		Sender.sendGeneric(data, numBits, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop, 0);
		digitalWrite(ledPin, HIGH);
		delay(duration);
		digitalWrite(ledPin, LOW);
	}
}
