/*
Name:		Standard.ino
Created:	2/19/2016 4:34:36 PM
Author:	lhk
*/
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

const int numBits = 4; // how many bits are send, important on the receiving side
const unsigned int Head_Mark = 6000;
const unsigned int Head_Space = 900;
const unsigned int Mark_One = 900;
const unsigned int Mark_Zero = 900;
const unsigned int Space_One = 3700;
const unsigned int Space_Zero = 900;
const unsigned int kHz = 56; // frequency of the signal, the led can emit any signal but the receiver is tuned to a specific freq
const bool Use_Stop = true; // add a stopBit to the end of the message

const int recvPin = 11;
unsigned int buffer[RAWBUF];

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

// different delays for different actions
unsigned long reloadTime = 1000;
unsigned long stunTime = 4000;
unsigned long blinkFreq = 500; // after a hit, the leds blink with this frequency

bool reloading = false;
bool stunned = false;
bool blink = false;

// sound effects
// the note C, for reference
const int C = 1912;

// after a hit, a siren sound is being played.
// it starts with sirenStart and goes up and down with deltaSiren
int sirenStart = C;
int siren = 0;
int deltaSiren = 3;
int reloadTone = 2000;
int shootTone = 500;

int reloadToneLength = 100;
int shootToneLength = 40;


void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);
	pinMode(blinkPin, OUTPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);

	digitalWrite(blinkPin, HIGH);
}

void loop() {
	// check for data on the receiver
	if (receiver.GetResults(&decoder)) {
		digitalWrite(ledPin, HIGH);

		// three outputs:
		// 1. was the decoding successful? 0 or 1
		// 2. decoded value
		// 3. IR signal data
		Serial.println(decoder.decodeGeneric(numBits * 2 + 4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero));
		Serial.println(decoder.value, HEX);
		decoder.DumpResults();


		//TODO: look at decoder.value. if it's the code of the enemy team
		// -> blink some red leds
		// -> play a sound
		// -> disable the system for a few seconds

		//TODO:
		// right now a shot is signalled by sending a 3.
		// this needs to be refined
		if (decoder.value == 3) {
			stunned = true;
			reloading = false;
			blink = true;
			digitalWrite(blinkPin, HIGH);


			timer = millis();
			blinkTimer = millis();

			siren = sirenStart;
			int sign = 1;

			while (millis() - timer < stunTime) {

				digitalWrite(blinkPin, HIGH);
				delayMicroseconds(siren / 2);

				digitalWrite(blinkPin, LOW);
				delayMicroseconds(siren / 2);

				siren += sign*deltaSiren;
				if (siren > 2500)
					sign *= -1;
				if (siren < 1000)
					sign *= -1;
			}
			stunned = false;
		}

		receiver.resume();
	}


	// if the player is stunned, blink with the LEDs
	if (stunned) {
		if (millis() - timer > stunTime) {
			stunned = false;
			digitalWrite(blinkPin, LOW);
		}
		else {
			// switch the LED's on and off
			if (millis() - blinkTimer > blinkFreq) {
				digitalWrite(blinkPin, blink ? HIGH : LOW);
				blink = !blink;
				blinkTimer = millis();
			}
		}
	}
	// if the player is reloading, he can't do anything
	else if (reloading) {
		if (millis() - timer > reloadTime) {

			timer = millis();
			while (millis() - timer<reloadToneLength) {

				digitalWrite(blinkPin, HIGH);
				delayMicroseconds(reloadTone / 2);

				digitalWrite(blinkPin, LOW);
				delayMicroseconds(reloadTone / 2);
			}

			reloading = false;
			digitalWrite(blinkPin, HIGH);
			delay(100);
			digitalWrite(blinkPin, LOW);


		}
	} 
	// player is neither stunned nor reloading
	else{
		buttonState = digitalRead(buttonPin);
		if (buttonState == HIGH) {
			reloading = true;
			timer = millis();
			digitalWrite(ledPin, HIGH);

			// shoot
			// the only important parameter is data
			// you can send any number you want (but only numBits bits of it).
			// with 4 bits being sent, this means we can transmit 2^4=16 distinct values
			// just increase numBits if you need more
			Sender.sendGeneric(data, numBits, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop, 0);

			// you can't send and receive at the same time. "shooting" automatically disables the receiver
			receiver.enableIRIn();

			
			for (int i = 0; i < 3; i++) {
				timer = millis();
				while (millis() - timer < shootToneLength) {

					digitalWrite(blinkPin, HIGH);
					delayMicroseconds(shootTone / 2);

					digitalWrite(blinkPin, LOW);
					delayMicroseconds(shootTone / 2);
				}
				delay(50);
			}

			//TODO:
			// -> start a timeout, don't let the player fire again until it runs out
			// -> play a weapon sound
			// -> blink some leds

			digitalWrite(ledPin, LOW);
		}
	}
}
