#include "Header_Files/stepper.h"

float mphToMs(float speed_mph){
    float convVal = 2.237;
    float speed_ms = speed_mph / convVal;
    return speed_ms;
}

float reqRPM(float speed_ms, float wheelDiameter){
    float n = speed_ms / (PI * wheelDiameter) * 60;
    return n;
}

float adjPotiSpeed(float speed_ms, float percentage){
    float newSpeed = speed_ms * (percentage / 100);
    return newSpeed;
}

float RPMtoPPS(float rpm, int pulsePerRev){
    float reqPPS = rpm * pulsePerRev / 60;
    return reqPPS; 
}