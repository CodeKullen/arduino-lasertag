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

IRsend Sender;
IRrecv receiver(recvPin);
IRdecode decoder;


// pins on the arduino
const int buttonPin = 2;
const int ledPin = 13;
int buttonState = 0;

unsigned long data = 3;

bool once = true;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);
}

void loop() {

	// check for data on the receiver
	if (receiver.GetResults(&decoder)) {
		digitalWrite(ledPin, HIGH);

		// immediately resume listening
		receiver.resume();

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
	}

	buttonState = digitalRead(buttonPin);
	if (buttonState == HIGH) {
		if (once) {
			once = false;
			digitalWrite(ledPin, HIGH);

			// shoot
			// the only important parameter is data
			// you can send any number you want (but only numBits bits of it).
			// with 4 bits being sent, this means we can transmit 2^4=16 distinct values
			// just increase numBits if you need more
			Sender.sendGeneric(data, numBits, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop, 0);
			
			// you can't send and receive at the same time. "shooting" automatically disables the receiver
			receiver.enableIRIn();

			//TODO:
			// -> start a timeout, don't let the player fire again until it runs out
			// -> play a weapon sound
			// -> blink some leds
		}
	}
	else {
		once = true;
		digitalWrite(ledPin, LOW);
	}
}
