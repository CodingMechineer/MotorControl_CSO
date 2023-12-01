#ifndef POTI_H
#define POTI_H

/* Poti to adjust the percentage of the speed - Target speed 1 mph*/
#define potiAdjSpeed A0 

// Poti to simulate the speed from the ISOBUS
#define potiSim A1

/* Maps the poti input between the two values */ 
float getPotiMap(int inputPin, int minVal, int maxVal);

bool delayTimeExpired(int delayTime);

#endif