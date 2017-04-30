#ifndef _DEFINITIONS_h
#define _DEFINITIONS_h


#if defined(ARDUINO) && ARDUINO >= 100
	//#include "arduino.h" // not needed in Arduino IDE, if all sources are ino instead of cpp.
#else
	#include "WProgram.h"
#endif

// pins on the arduino
// DO NOT CHANGE, they directly relate to the hardware
#define BLINKPIN 8
#define BUTTONPIN 2
#define LEDPIN 13
#define RECVPIN 11

// values for messages to be sent
// they are probably subject to change
#define TEAM_ONE 1
#define TEAM_TWO 2

#define TEAM_ONE_SHOT 1
#define TEAM_TWO_SHOT 2
#define TEAM_ONE_SET 3
#define TEAM_TWO_SET 4
#define STATISTICS 5

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

const int numBits = 2; // how many bits are send, important on the receiving side
const unsigned int Head_Mark = 1400;
const unsigned int Head_Space = 1000;
const unsigned int Mark_One = 1000;
const unsigned int Mark_Zero = 1000;
const unsigned int Space_One = 1500;
const unsigned int Space_Zero = 1000;
const unsigned int kHz = 38; // frequency of the signal, the led can emit any signal but the receiver is tuned to a specific freq
const bool Use_Stop = true; // add a stopBit to the end of the message

#endif