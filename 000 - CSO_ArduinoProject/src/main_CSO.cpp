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

/* Adjustable percentage range */
int minPercent = 95;
int maxPercent = 105;

/* Instantiate seven segment display */
SevSeg sevseg;

/* Instantiate ISOBUS */
ISOBUSMessage receiveMessage; 

/* Instantiate stepper */
AccelStepper stepper(1,driverPUL,driverDIR); // first number is the type of stepper, PUL output, DIR output

void setup()
{
  /* Initialize arduino communication*/
  Serial.begin(115200);

  /* Initialize ISOBUS communication */
  ISOBUS.begin(CAN_SPEED_250000);
  ISOBUS.setMode (CAN_MODE_NORMAL);

  /* Initialize display */
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  
  /* Setup for poti */
  pinMode(potiAdjSpeed, INPUT);
  pinMode(potiPower, OUTPUT);
  digitalWrite(potiPower, HIGH); // Output used as power supply for the poti

  /* Setup for stepper */
  pinMode(driverPUL, OUTPUT);
  pinMode(driverDIR, OUTPUT);
  stepper.setMaxSpeed(1000);

  delay(1000);

  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
}

void loop()
{

  /* Debugging with number one to print engine RPM */
  UserInput = Serial.read();
  receiveMessage = ISOBUS.getMessageISOBUS(EEC1_PGN, EngineSpeed_SPN, spn_buffer);
  if (UserInput =='1'){
    printMessage(receiveMessage, spn_buffer);
    
    if (receiveMessage.spn_data != 0)
      sevseg.setNumber(receiveMessage.spn_data, 0);
  }
 
  /* Reading of the ISOBUS from the tractor*/
  receiveMessage = ISOBUS.getMessageISOBUS(NBVehicleSpeed_PGN, NBVehicleSpeed_SPN, spn_buffer);

  /* Calculations to convert the speed signal to required rounds per minute */
  // float SpeedSignal = float(getPotiMap(potiSim, 5, 15))/floatFact; // Values "simulate" speed between the values
  float SpeedSignal = float(receiveMessage.spn_data)/floatFact; // Values "simulate" speed between the values
  float effSpeedMS = mphToMs(SpeedSignal);
  float adjPerc = float(getPotiMap(potiAdjSpeed, 950, 1050)/floatFact); // Adjust the speed between 95% and 105% 
  float adjSpeedMS = adjPotiSpeed(effSpeedMS, adjPerc); 
  float reqMotRPM = reqRPM(adjSpeedMS, seedWheelDia);
  float reqPPS = RPMtoPPS(reqMotRPM, pulsePerRev);

  /* Debugging */
  // if (delayTimeExpired(1000) == true){
  //   Serial.println(SpeedSignal);
  //   Serial.println(adjPerc);
  //   Serial.println(reqMotRPM);
  // }

  /* Set the speed of the motor*/
  stepper.setSpeed(reqPPS);
  stepper.runSpeed();

  /* Updates the display */
  sevseg.refreshDisplay(); // Must run repeatedly
  sevseg.setNumberF(adjPerc, 1);
}

