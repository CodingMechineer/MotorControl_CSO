#include "Header_Files/stepper.h"

float reqRPM(float speed_ms, float wheelRad){
    float n = speed_ms / (2 * PI * wheelRad);
    return n;
}

float adjPotiSpeed(float speed_ms, float percentage){
    float newSpeed = speed_ms * (percentage / 100);
    return newSpeed;
}