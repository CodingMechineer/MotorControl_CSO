#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <Arduino.h>
#include <ModbusMaster.h>

#define MODBUS_SLAVE_ID 1  // Adjust the slave ID as per your module
#define DISPLAY_NUMBER 0x0000  // Adjust the register address for your display
#define DISPLAY_COMMA 0x0001  // Address to set the comma after the certain number
#define DISPLAY_BRIGHTNESS 0x0002  // Address to set the brightness
#define DISPLAY_BAUDRATE 0x0003  // Address to set the baudrate

#endif
