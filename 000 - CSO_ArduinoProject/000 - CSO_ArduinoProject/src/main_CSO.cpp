#include <Arduino.h>

#include "Header_Files/seven_Segment.h"
#include "Header_Files/stepper.h"
#include "Header_Files/isobus_var.h"
#include "Header_Files/poti.h"

/* Variables */
#define SERIAL_SPEED 115200
const int floatFact = 10;

int UserInput = 0; // Variable for input with keyboard
char spn_buffer[512];  // Data will be temporarily stored to this buffer before being written to the filechar buffer[512];  //Data will be temporarily stored to this buffer before being written to the file
char debugMessage[50];

float SpeedSignal = 0;
float effSpeedMS = 0;
float adjPerc = 0;
int oldAdjPerc = 0;
float adjSpeedMS = 0;
float reqMotRPM = 0;
float reqPPS = 0;

/* Adjustable percentage range */
int minPercent = 95;
int maxPercent = 105;

/* Instantiate seven segment display */
ModbusMaster node;

/* Instantiate ISOBUS */
ISOBUSMessage receiveMessage; 

/* Instantiate stepper */
AccelStepper stepper(1,driverPUL,driverDIR); // first number is the type of stepper, PUL output, DIR output

void setup()
{
  /* Initialize arduino communication*/
  Serial.begin(SERIAL_SPEED);

  /* Initialize ISOBUS communication */
  ISOBUS.begin(CAN_SPEED_250000);
  ISOBUS.setMode (CAN_MODE_NORMAL);

  /* Initialize display

  0x0000: Display data (00 32 decimal is 50, and negative numbers are represented by the inverse code. If ff fe is sent, -001 is displayed);
  0x0001: Set the position of the decimal point (the range is 01-03, 01 is the decimal point after the tens, 02 is the decimal point after the hundreds, 03 is the thousands, and only the tens and hundreds are displayed when it is negative)
  0x0002: Set the brightness (range 01-07, the larger the data, the brighter);
  0x0003: Set the baud rate (data range 0-7 corresponds to 1200 2400 4800 9600 19200 38400 57600 115200 respectively);
  */
  Serial1.begin(9600); // slower speed for display
  node.begin(MODBUS_SLAVE_ID, Serial1);
  node.writeSingleRegister(DISPLAY_COMMA, 1); // comma after the "number" digit, DISPLAY_COMMA ist the address
  node.writeSingleRegister(DISPLAY_BRIGHTNESS, 7); // comma after the "number" digit, DISPLAY_COMMA ist the address

  /* Setup for poti */
  pinMode(potiAdjSpeed, INPUT);
  pinMode(potiPower, OUTPUT);
  digitalWrite(potiPower, HIGH); // Output used as power supply for the poti

  /* Setup for stepper */
  pinMode(driverPUL, OUTPUT);
  pinMode(driverDIR, OUTPUT);
  stepper.setMaxSpeed(2000);
  stepper.setSpeed(0);

  delay(1000);

  /* Debugging */
  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
}

void loop()
{
  /* Debugging with number one to print engine RPM */
  UserInput = Serial.read();
  if (UserInput =='1'){
    receiveMessage = ISOBUS.getMessageISOBUS(EEC1_PGN, EngineSpeed_SPN, spn_buffer);
    printMessage(receiveMessage, spn_buffer);
    node.writeSingleRegister(DISPLAY_NUMBER, float(receiveMessage.spn_data)/floatFact);  // Adjust the value as needed
  }

  /* Reading of the ISOBUS from the tractor*/
  receiveMessage = ISOBUS.getMessageISOBUS(NBVehicleSpeed_PGN, NBVehicleSpeed_SPN, spn_buffer);

  /* Calculations to convert the speed signal to required rounds per minute */

  // For debugging to simulate the tractor speed with a poti:
  // SpeedSignal = float(getPotiMap(potiSim, 10, 20))/floatFact; // Values "simulate" speed between the values
  
  if (receiveMessage.status == 0){
    SpeedSignal = float(receiveMessage.spn_data)/floatFact;
  }
  effSpeedMS = kmhToMs(SpeedSignal);
  adjPerc = float(getPotiMap(potiAdjSpeed, 950, 1050)/floatFact); // Adjust the speed between 95% and 105% 
  adjSpeedMS = adjPotiSpeed(effSpeedMS, adjPerc); 
  reqMotRPM = reqRPM(adjSpeedMS, seedWheelDia);
  reqPPS = RPMtoPPS(reqMotRPM, pulsePerRev);
  
  /* Debugging */
  // if (delayTimeExpired(1000) == true){
  //   Serial.println(receiveMessage.spn_data);
  //   Serial.println(SpeedSignal);
  //   Serial.println(adjPerc);
  //   Serial.println(reqMotRPM);
  //   Serial.println(reqPPS);
  // }

  /* Set the speed of the motor*/
  stepper.setSpeed(reqPPS);
  stepper.runSpeed();

  /* Updates the display 
  
  if statement necessary because if executed to often, motor turn slower
  most likely because the display gets controlled with a lower baud rate
  Couldn't find out how to change the baud rate of the display 
  */

  if (delayTimeExpired(1000) == true){ // Display gets only updated once in a second, calculations happen all the time
    node.writeSingleRegister(DISPLAY_NUMBER, adjPerc*floatFact);   // Send MODBUS command to update the display
    Serial.println("Updated");
  }
}