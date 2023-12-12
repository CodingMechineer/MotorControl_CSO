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
  stepper.setSpeed(500.96);

  delay(1000);

  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
  Serial.println("2 - Speed");
  Serial.println("3 - Speed 2");  
}

void loop()
{
  /* Calculations to convert the speed signal to required rounds per minute */
  float SpeedSignal = float(getPotiMap(potiSim, 5, 15))/floatFact; // Values "simulate" speed between the values
  float effSpeedMS = mphToMs(SpeedSignal);
  float adjPerc = float(getPotiMap(potiAdjSpeed, 950, 1050)/floatFact); // Adjust the speed between 95% and 105% 
  float adjSpeedMS = adjPotiSpeed(effSpeedMS, adjPerc); 
  float reqMotRPM = reqRPM(adjSpeedMS, seedWheelDia);
  float reqPPS = RPMtoPPS(reqMotRPM, pulsePerRev);

  // /* Debug the speed calculations */
  // int str = 10;
  // char str_simSpeedSignal[str];
  // char str_effSpeedMS[str];
  // char str_adjSpeedMS[str];
  // char str_reqMotRPM[str];
  // char str_reqPPS[str];

  // dtostrf(simSpeedSignal, 3, 1, str_simSpeedSignal);
  // dtostrf(effSpeedMS, 3, 1, str_effSpeedMS);
  // dtostrf(adjSpeedMS, 3, 1, str_adjSpeedMS);
  // dtostrf(reqMotRPM, 3, 1, str_reqMotRPM);
  // dtostrf(reqPPS, 3, 1, str_reqPPS);

  // sprintf(debugMessage, "SpeedMPH: %s mph, SpeedMS: %s m/s, AdjSpeedMS: %s, reqMotRPM: %s, reqPPS: %s",
  // str_simSpeedSignal, str_effSpeedMS, str_adjSpeedMS, str_reqMotRPM, str_reqPPS);
  // Serial.println(debugMessage);



  // Serial.println("---------------");
  // Serial.println(simSpeedSignal);
  // Serial.println(adjSpeedMS);
  if (delayTimeExpired(1000) == true){
    Serial.println(SpeedSignal);
    Serial.println(adjPerc);
    Serial.println(reqMotRPM);
  }

  /* Set the speed of the motor*/
  stepper.setSpeed(reqPPS);
  stepper.runSpeed();


  /* Updates the display */
  sevseg.refreshDisplay(); // Must run repeatedly
  sevseg.setNumberF(adjPerc, 1);
 
  while(Serial.available()){
  UserInput = Serial.read();
  // UserInput = 1;

    // "EngineSpeed" and "WBMspeed" pre-determined by library
    if (ISOBUS.available()){

      if (UserInput =='1'){
        receiveMessage = ISOBUS.getMessageISOBUS(EEC1_PGN, EngineSpeed_SPN, spn_buffer);
        printMessage(receiveMessage, spn_buffer);
        
        if (receiveMessage.spn_data != 0)
          sevseg.setNumber(receiveMessage.spn_data, 0);
      }

      if (UserInput == '2'){
        int potiVal = getPotiMap(potiAdjSpeed, minPercent, maxPercent);
        sevseg.setNumberF(float(potiVal)/floatFact, 1);
      }

      if (UserInput == '3'){
          receiveMessage = ISOBUS.getMessageISOBUS(NBVehicleSpeed_PGN, NBVehicleSpeed_SPN, spn_buffer);
          printMessage(receiveMessage, spn_buffer);
          
          if(receiveMessage.spn_data != 0){
            Serial.println(float(receiveMessage.spn_data)/floatFact); // transmit speed without float variable
            sevseg.setNumber(receiveMessage.spn_data, 1);            
          }
      }
    }  
  }
}