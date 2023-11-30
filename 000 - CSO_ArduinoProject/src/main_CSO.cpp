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
  pinMode(PotiInput, INPUT);

  /* Setup for stepper */
  pinMode(driverPUL, OUTPUT);
  pinMode(driverDIR, OUTPUT);
  stepper.setMaxSpeed(200);

  delay(1000);

  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
  Serial.println("2 - Speed");
  Serial.println("3 - Speed 2");  
}

void loop()
{
  sevseg.refreshDisplay(); // Must run repeatedly
  sevseg.setNumber(6, 0);
  

  // motorTest();
  stepper.setSpeed(getPotiMap(PotiInput, 0, 1000));
  stepper.runSpeed();

  
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
        int potiVal = getPotiMap(PotiInput, minPercent, maxPercent);
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