/*
Blink
Turns on an LED on for one second, then off for one second, repeatedly.

Most Arduinos have an on-board LED you can control. On the Uno and
Leonardo, it is attached to digital pin 13. If you're unsure what
pin the on-board LED is connected to on your Arduino model, check
the documentation at http://www.arduino.cc

This example code is in the public domain.

modified 8 May 2014
by Scott Fitzgerald
*/
/*

// the setup function runs once when you press reset or power the board
void setup() {
// initialize digital pin 13 as an output.
pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
delay(100);              // wait for a second
digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
delay(100);              // wait for a second
}*/

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


#define LIGHT_STRIKE_DATA_LENGTH    32
#define LIGHT_STRIKE_RAW_LENGTH     66
#define LIGHT_STRIKE_HEADER_MARK  6750
#define LIGHT_STRIKE_HEADER_SPACE  900
#define LIGHT_STRIKE_MARK_ONE      900
#define LIGHT_STRIKE_MARK_ZERO     900
#define LIGHT_STRIKE_SPACE_ONE    3700
#define LIGHT_STRIKE_SPACE_ZERO    900
#define LIGHT_STRIKE_KHZ            56//38
#define LIGHT_STRIKE_USE_STOP     true
#define LIGHT_STRIKE_MAX_EXTENT      0

const int recvPin = 11;
const int ledPin = 13;

IRrecv receiver(recvPin);
IRdecodeBase decoder;
IRdecode fdecoder;
IRdecodePanasonic_Old gdecoder;

unsigned int buffer[RAWBUF];

void setup() {
	Serial.begin(9600);
	receiver.enableIRIn();
	decoder.UseExtnBuf(buffer);
	digitalWrite(ledPin, LOW);
}

void loop() {
	if (receiver.GetResults(&decoder)) {

		digitalWrite(ledPin, HIGH);
		receiver.resume();
		Serial.println(decoder.decodeGeneric(numBits*2+4, Head_Mark, Head_Space, 0, Mark_Zero, Space_One, Space_Zero));
		/*decoder.decodeGeneric(
			LIGHT_STRIKE_RAW_LENGTH, 
			LIGHT_STRIKE_HEADER_MARK, 
			LIGHT_STRIKE_HEADER_SPACE, 
			//LIGHT_STRIKE_MARK_ONE, 
			0,
			LIGHT_STRIKE_MARK_ZERO,
			LIGHT_STRIKE_SPACE_ONE, 
			LIGHT_STRIKE_SPACE_ZERO);
*/
		

		Serial.println(decoder.value,HEX);
		decoder.DumpResults();
	}
}
