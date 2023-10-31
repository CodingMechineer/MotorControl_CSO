#include "Arduino.h"
#include "Globals.h"
#include "FunctionDeclaration.h"

// --------- Function requiredIncrements ---------------
int numberOfIncrements;              // number of increments which the hallsensor needs to count before the stop position
float gearRatio;                     // gear ratio between the gears
const int incrementsPerRotation = 6; // six magnets on the gear for the hallsensor to indicate
const int pitchThread = 20;          // pitch of the thread in mm per rotation
const float heightChange = 50.0;     // 50 mm
const int correctionIncrement = 17;   // increments for correction, value gets added, positive value --> less movement
const int gearTeethSmall = 29;       // number of teeth from the small gear
const int gearTeethBig = 91;         // number of teeth from the big gear

int requiredIncrements()
{
    gearRatio = gearTeethBig / float(gearTeethSmall);
    numberOfIncrements = incrementsPerRotation * gearRatio * (heightChange / pitchThread) - correctionIncrement;

    return (numberOfIncrements);
}

//------------ Function to count the number of steps from of the motor --------------
//------------ Variables -------------
bool stepsSignalDetected = false;
int stepsCounted = 0;

int getNumberOfSteps(bool reset)
{
    if (reset == true)
    {
        stepsCounted = 0;
        reset = false;
    }

    if (digitalRead(sensorRotation) == false && stepsSignalDetected == false)
    {
        stepsSignalDetected = true;
        stepsCounted = stepsCounted + 1;

        Serial.print("detected steps: ");   // for debugging
        Serial.print(stepsCounted);         // for debugging
        Serial.print("/");                  // for debugging
        Serial.println(numberOfIncrements); // for debugging
    }
    else if (digitalRead(sensorRotation) == true && stepsSignalDetected == true)
    {
        stepsSignalDetected = false;
    }

    return stepsCounted;
}

/////////////////////////////////////////////////////
//------ Functions to change the ride height ------//
/////////////////////////////////////////////////////

bool resCount = false;
bool finished = false;

int rampDelayTimeFaster = 8;    // time in [ms], values between 1 - 15 ms
int rampDelayTimeSlower = 8;    // time in [ms], values between 1 - 15 ms
int rampDelayTimeStop = 2;      // // time in [ms], values between 1 - 15 ms
int L_PWM_SpeedAutomatic = 255; // PWM signal, values between 0 - 255
int R_PWM_SpeedAutomatic = 255; // PWM signal, values between 0 - 255
int L_PWM_SpeedSlow = 80;       // PWM signal, values between 0 - 255
int R_PWM_SpeedSlow = 80;       // PWM signal, values between 0 - 255

int incrementStartRamp = 2; // how many increments before the final position the ramp starts

bool goToUpperPosition()
{
    finished = false;
    // sendData();

    if (digitalRead(sensorTop) != false)
    {
        if (requiredIncrements() - incrementStartRamp > getNumberOfSteps(resCount))
        {
            L_MotorGoToSpeed(rampDelayTimeFaster, L_PWM_SpeedAutomatic);
        }
        else if (requiredIncrements() - incrementStartRamp <= getNumberOfSteps(resCount) && requiredIncrements() > getNumberOfSteps(resCount))
        {
            L_MotorGoToSpeed(rampDelayTimeSlower, L_PWM_SpeedSlow);
        }
    }

    if (requiredIncrements() <= getNumberOfSteps(resCount) || digitalRead(sensorTop) == false)
    {
        MotorStop(rampDelayTimeStop);
        resCount = true;
        finished = true;
    }

    if (resCount == true && finished == false)
    {
        resCount = false;
    }

    return finished;
}

bool goToLowerPosition()
{
    finished = false;
    // sendData();
    
    if (digitalRead(sensorBottom) != false)
    {
        if (requiredIncrements() - incrementStartRamp > getNumberOfSteps(resCount))
        {
            R_MotorGoToSpeed(rampDelayTimeFaster, R_PWM_SpeedAutomatic);
        }
        else if (requiredIncrements() - incrementStartRamp <= getNumberOfSteps(resCount) && requiredIncrements() > getNumberOfSteps(resCount))
        {
            R_MotorGoToSpeed(rampDelayTimeStop, R_PWM_SpeedSlow);
        }
    }

    if (requiredIncrements() <= getNumberOfSteps(resCount) || digitalRead(sensorBottom) == false)
    {
        MotorStop(rampDelayTimeStop);
        resCount = true;
        finished = true;
    }

    if (resCount == true && finished == false)
    {
        resCount = false;
    }

    return finished;
}
