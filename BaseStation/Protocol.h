// Protocol.h

#ifndef _PROTOCOL_h
#define _PROTOCOL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define TEAM_ONE 1
#define TEAM_TWO 2

#define TEAM_ONE_SHOT 1
#define TEAM_TWO_SHOT 2
#define TEAM_ONE_SET 3
#define TEAM_TWO_SET 4
#define STATISTICS 5

const int numBits = 3; // how many bits are send, important on the receiving side
const unsigned int Head_Mark = 1800;
const unsigned int Head_Space = 1000;
const unsigned int Mark_One = 1000;
const unsigned int Mark_Zero = 1000;
const unsigned int Space_One = 2000;
const unsigned int Space_Zero = 1000;
const unsigned int kHz = 38; // frequency of the signal, the led can emit any signal but the receiver is tuned to a specific freq
const bool Use_Stop = true; // add a stopBit to the end of the message


#endif

