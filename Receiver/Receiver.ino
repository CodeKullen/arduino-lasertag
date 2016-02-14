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

const int recvPin = 11;
const int ledPin = 13;

IRrecv receiver(recvPin);
//IRdecode decoder;
IRdecodePanasonic_Old decoder;
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
		decoder.decode();
		Serial.println(buffer[0]);
		decoder.DumpResults();
	}
}
