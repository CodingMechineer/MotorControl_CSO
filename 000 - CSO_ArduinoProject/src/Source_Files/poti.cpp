#include <Arduino.h>
#include "Header_Files/poti.h"

int getPotiMap(int inputPin, int minVal, int maxVal){

  // Read the potentiometer value
  int potValue = analogRead(inputPin);

  // Map the potentiometer value to the servo range
  int potiValue = map(potValue, 0, 1023, minVal, maxVal);
  
  return potiValue;
}