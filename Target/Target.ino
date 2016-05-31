/*
Name:		Target.ino
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

const int numBits = 3; // how many bits are send, important on the receiving side
const unsigned int Head_Mark = 1200;
const unsigned int Head_Space = 600;
const unsigned int Mark_One = 600;
const unsigned int Mark_Zero = 600;
const unsigned int Space_One = 1200;
const unsigned int Space_Zero = 600;
const unsigned int kHz = 38; // frequency of the signal, the led can emit any signal but the receiver is tuned to a specific freq
const bool Use_Stop = true; // add a stopBit to the end of the message


const int recvPin = 11;
unsigned int buffer[RAWBUF];

// Objects to send and receive IR data
IRsend Sender;
IRrecv receiver(recvPin);
IRdecode decoder;

const int ledPin = 13;
const int blinkPin = 2;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(blinkPin, OUTPUT);

	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);

	digitalWrite(blinkPin, HIGH);
}

void loop() {

	if (receiver.GetResults(&decoder)) {
		digitalWrite(ledPin, HIGH);
		digitalWrite(blinkPin, HIGH);

		int val = decoder.decodeGeneric(numBits * 2 + 4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero);

		Serial.println(decoder.value, HEX);
		decoder.DumpResults();

		if (decoder.value == 3) {
			for (int i = 0; i < 4; i++) {
				digitalWrite(blinkPin, HIGH);
				delay(300);
				digitalWrite(blinkPin, LOW);
				delay(100);
			}
		}
		else {
			delay(500);
		}
		digitalWrite(blinkPin, LOW);
		receiver.enableIRIn();
	}
	digitalWrite(ledPin, LOW);
	digitalWrite(blinkPin, LOW);
}
