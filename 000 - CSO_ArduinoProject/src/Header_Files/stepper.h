#ifndef STEPER_H
#define STEPER_H

#include <Arduino.h>
#include <AccelStepper.h>

#define driverPUL 23
#define driverDIR 24

const float seedWheelDia = 0.254; // in meters
const float seedWheelRad = seedWheelDia / 2;

const int pulseRev = 200;

/* Speed in meters per second, wheel radius in meters */
float reqRPM(float speed_ms, float wheelRad);

/* Speed in meters per second, percentage "### %" */
float adjPotiSpeed(float speed_ms, float percentage);

#endif
