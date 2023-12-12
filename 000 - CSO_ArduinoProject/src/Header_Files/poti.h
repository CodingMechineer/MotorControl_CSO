#ifndef POTI_H
#define POTI_H

/* Poti to adjust the percentage of the speed - Target speed 1 mph*/
#define potiAdjSpeed A0
#define potiPower 55 // analog pin A1, used to power the poti 

// Poti to simulate the speed from the ISOBUS
#define potiSim A2

/* Maps the poti input between the two values */ 
float getPotiMap(int inputPin, int minVal, int maxVal);

bool delayTimeExpired(int delayTime);

#endif