/*
 Name:		V1.ino
 Created:	2/19/2016 4:34:36 PM
 Author:	lhk
*/
#include <IRLib.h>
#include <IRLibTimer.h>
#include <IRLibRData.h>
#include <IRLibMatch.h>

const int numBits = 4;
const unsigned int Head_Mark = 6000;
const unsigned int Head_Space = 900;
const unsigned int Mark_One = 900;
const unsigned int Mark_Zero = 900;
const unsigned int Space_One = 3700;
const unsigned int Space_Zero = 900; //ignored
const unsigned int kHz = 56;
const bool Use_Stop = true;


const int buttonPin = 2;
const int sendPin = 5;
const int recvPin = 11;
const int ledPin = 13;

int buttonState = 0;

IRsend Sender;
IRrecv receiver(recvPin);
IRdecodeBase decoder;

unsigned long data = 3;
unsigned int buffer[RAWBUF];

bool once = true;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);
	pinMode(sendPin, OUTPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);
}

void loop() {
	if (receiver.GetResults(&decoder)) {

		digitalWrite(ledPin, HIGH);
		receiver.resume();
		Serial.println(decoder.decodeGeneric(numBits * 2 + 4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero));


		Serial.println(decoder.value, HEX);
		decoder.DumpResults();
	}
	buttonState = digitalRead(buttonPin);
	if (buttonState == HIGH) {
		if (once) {
			once = false;
			digitalWrite(ledPin, HIGH);
			Sender.sendGeneric(data, numBits, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop, 0);
			receiver.enableIRIn();
		}
	}
	else {
		once = true;
		digitalWrite(ledPin, LOW);
	}
}
