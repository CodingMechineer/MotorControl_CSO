#include <Arduino.h>
#include "99_globals.h"

#include <SPI.h>
#include <ISOBUS.h>

char spn_buffer[512];  //Data will be temporarily stored to this buffer before being written to the filechar buffer[512];  //Data will be temporarily stored to this buffer before being written to the file
char UserInput;

void printMessage(ISOBUSMessage msg, char buffer[]);

ISOBUSMessage receiveMessage1;
ISOBUSMessage receiveMessage2;

void setup()
{
  ISOBUS.begin(CAN_SPEED_250000);
  ISOBUS.setMode (CAN_MODE_NORMAL);
  Serial.begin(115200);

  // initialize digital pin 2 and 3 as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  delay(1000);

  Serial.println("Please choose a menu option.");
  Serial.println("1 - RPM");
  Serial.println("2 - Speed");

}

void loop()
{
  while(Serial.available()){
  UserInput = Serial.read();

    /* Receive data if available */
    if (ISOBUS.available()){
      if (UserInput =='1'){
        receiveMessage1 = ISOBUS.getMessageISOBUS(EEC1_PGN, EngineSpeed_SPN, spn_buffer);
        printMessage(receiveMessage1, spn_buffer);
        }
    }
    if (UserInput == '2'){
        receiveMessage2 = ISOBUS.getMessageISOBUS(NBVS_PGN, NBVS_SPN, spn_buffer);
        printMessage(receiveMessage2, spn_buffer);
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

  if (msg.status == 10){
    Serial.println("PGN not available");
  }
  
}
