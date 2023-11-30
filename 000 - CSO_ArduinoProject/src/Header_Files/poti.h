#ifndef POTI_H
#define POTI_H

#define PotiInput A0

/* Initialize functions */

/* Input pin from the Arduino, minimal returned value, maximal returned value */ 
int getPotiMap(int inputPin, int minVal, int maxVal); 

#endif