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

const int numBits = 2;
const unsigned int Head_Mark = 1500;
const unsigned int Head_Space = 800;
const unsigned int Mark_One = 2000;
const unsigned int Mark_Zero = 1300;
const unsigned int Space_One = Head_Space;
const unsigned int Space_Zero = 700; //ignored
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
#define LIGHT_STRIKE_KHZ            56
#define LIGHT_STRIKE_USE_STOP     true
#define LIGHT_STRIKE_MAX_EXTENT      0

const int buttonPin = 2;
const int sendPin = 5;
const int ledPin = 13;

int buttonState = 0;

IRsendPanasonic_Old panasonic_Sender;
IRsend Sender;
IRsendRaw rawSender;

unsigned long data = 3;
unsigned int data2[3] = { 10,2,3 };
int len = 3;
int freq = 56;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);
	pinMode(sendPin, OUTPUT);
}

void loop() {
	buttonState = digitalRead(buttonPin);
	if (buttonState == HIGH) {
		digitalWrite(ledPin, HIGH);
		for (int i = 0; i < 1; i++) {
			//panasonic_Sender.send(data);
			//Sender.sendGeneric(data, 32, 564 * 16, 564 * 4, 564 * 4, 564*2, 564 * 4, 564, 56, true); 
			
			Sender.sendGeneric(data, 2, Head_Mark, Head_Space, Mark_One, Mark_Zero, Space_One, Space_Zero, kHz, Use_Stop);

			/*Sender.sendGeneric(
				0x0102+ 0x0700,
				LIGHT_STRIKE_DATA_LENGTH,
				LIGHT_STRIKE_HEADER_MARK,
				LIGHT_STRIKE_HEADER_SPACE,
				LIGHT_STRIKE_MARK_ONE,
				LIGHT_STRIKE_MARK_ZERO,
				LIGHT_STRIKE_SPACE_ONE,
				LIGHT_STRIKE_SPACE_ZERO,
				LIGHT_STRIKE_KHZ,
				LIGHT_STRIKE_USE_STOP,
				LIGHT_STRIKE_MAX_EXTENT);
				*/
			delay(1000);
		}
	}
	else {
		digitalWrite(ledPin, LOW);
		//digitalWrite(sendPin, LOW);
	}
}
