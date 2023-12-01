#include <Arduino.h>
#include "Header_Files/poti.h"

float getPotiMap(int inputPin, int minVal, int maxVal){

  // Read the potentiometer value
  float potValue = analogRead(inputPin);

  // Map the potentiometer value to the servo range
  float potiValue = map(potValue, 0, 1023, minVal, maxVal);
  
  return potiValue;
}

//------------ Function to replace delay() -----------
//------------ Variables -------------
int startTime;
int pastTime;
bool timeExpired = true;

bool delayTimeExpired(int delayTime){

  int timeNow = millis();

  if (timeExpired == true){
    startTime = timeNow;
    timeExpired = false;
  }
  
  pastTime = timeNow - startTime;

  if (pastTime >= delayTime){
    timeExpired = true;
  }

  return timeExpired;
}