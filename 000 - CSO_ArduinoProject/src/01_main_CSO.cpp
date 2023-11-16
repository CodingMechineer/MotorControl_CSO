#include <Arduino.h>
#include "99_globals.h"

#include <SPI.h>
#include <ISOBUS.h>

#include "SevSeg.h"

/* Initialize functions*/
void printMessage(ISOBUSMessage msg, char buffer[]);
int getPoti(int poti);
int floatFact = 10;


/* Variables */
ISOBUSMessage receiveMessage; // Instantiate ISOBUS
char spn_buffer[512];  // Data will be temporarily stored to this buffer before being written to the filechar buffer[512];  //Data will be temporarily stored to this buffer before being written to the file
int UserInput = 0; // Variable for input with keyboard

SevSeg sevseg; // Instantiate a seven segment object


void setup()
{
  ISOBUS.begin(CAN_SPEED_250000);
  ISOBUS.setMode (CAN_MODE_NORMAL);
  Serial.begin(115200);

  pinMode(PotiInput, INPUT);


  delay(1000);

  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
  Serial.println("2 - Speed");
  Serial.println("3 - Speed 2");


  /* Setup for 7-segment */
  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {11, 13, 9, 7, 6, 12, 10, 8};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  

}

void loop()
{
  sevseg.refreshDisplay(); // Must run repeatedly
  
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
        int potiVal = 0; 
        potiVal = getPoti(PotiInput);
        sevseg.setNumberF(float(potiVal)/floatFact, 1);
      }

      if (UserInput == '3'){
          receiveMessage = ISOBUS.getMessageISOBUS(NBVehicleSpeed_PGN, NBVehicleSpeed_SPN, spn_buffer);
          printMessage(receiveMessage, spn_buffer);
          
          if(receiveMessage.spn_data != 0){
            int trFactor = 10;
            Serial.println(float(receiveMessage.spn_data)/trFactor); // transmit speed without float variable
            sevseg.setNumber(receiveMessage.spn_data, 1);            
          }
      }
    }  
  }
}



void printMessage(ISOBUSMessage msg, char buffer[])
{
  if (msg.status == 0){
    Serial.print(msg.status);
    Serial.print(",");
    Serial.print(msg.id);
    Serial.print(",");
    Serial.print(msg.pgn);
    Serial.print(",");
    Serial.print(msg.spn_data);
    Serial.print(",");
    Serial.println(buffer);
  }
}


int getPoti(int poti){
  const int potiMin = 900;
  const int potiMax = 1100;

  // Read the potentiometer value
  int potValue = analogRead(poti);
  Serial.println(potValue);

  // Map the potentiometer value to the servo range
  int potiValue = map(potValue, 0, 1023, potiMin, potiMax);
  
  return potiValue;
}

