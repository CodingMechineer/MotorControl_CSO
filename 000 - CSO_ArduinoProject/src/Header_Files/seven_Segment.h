#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "SevSeg.h"
#include <Arduino.h>
#include <ModbusMaster.h>

#define MODBUS_SLAVE_ID 1  // Adjust the slave ID as per your module
#define DISPLAY_NUMBER 0x0000  // Adjust the register address for your display
#define DISPLAY_COMMA 0x0001  // Address to set the comma after the certain number

byte numDigits = 4;
byte digitPins[] = {2, 3, 4, 5};
byte segmentPins[] = {11, 13, 9, 7, 6, 12, 10, 8};
bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
byte hardwareConfig = COMMON_CATHODE; // See README.md for options
bool updateWithDelays = false; // Default 'false' is Recommended
bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

#endif
