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

const int buttonPin = 2;
const int sendPin = 5;
const int ledPin = 13;

int buttonState = 0;

IRsendPanasonic_Old panasonic_Sender;
IRsend Sender;
IRsendRaw rawSender;

unsigned long data = 2;
unsigned int data2[3] = { 1,2,3 };
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
		//for (int i = 0; i < 15; i++)
		//rawSender.send(data2, len, freq);
		//otherSender.send(data2, len, 56);
		//digitalWrite(sendPin, HIGH);
		panasonic_Sender.send(data);
		//Sender.send(PANASONIC_OLD, data, 10);
		//rawSender.send(data2, sizeof(data2)/sizeof(int), 56);
		delay(1000);
	}
	else {
		digitalWrite(ledPin, LOW);
		//digitalWrite(sendPin, LOW);
	}
}
