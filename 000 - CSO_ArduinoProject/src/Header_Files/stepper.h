#ifndef STEPER_H
#define STEPER_H

#include <Arduino.h>
#include <AccelStepper.h>

/////////////////////////////////////////////////////////////////
/*  Variables   */
#define driverPUL 23
#define driverDIR 24

const float seedWheelDia = 0.254; // in meters

const int pulsePerRev = 1000;
/////////////////////////////////////////////////////////////////

/* Speed in meters per second, wheel radius in meters */
float reqRPM(float speed_ms, float wheelRad);

/* Speed in meters per second, percentage "### %" */
float adjPotiSpeed(float speed_ms, float percentage);

/* Converts the speed from miles per hour to meters per second */
float mphToMs(float speed_mph);

/* Converts the required RPM from the motor to required pulse per second sent to the driver*/
float RPMtoPPS(float rpm, int pulsePerRev); 

#endif
