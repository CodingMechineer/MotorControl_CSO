#include "Arduino.h"
#include "Globals.h"
#include "FunctionDeclaration.h"

//------------ Variables -------------
int R_MotorPWM; // PWM value of the output for clockwise rotation
int L_MotorPWM; // PWM value of the output for counterclockwise rotation

//------------ Function for motor test -----------
void MotorTest()
{

  ////////////////////////////////////////
  int PauseBetweenSteps = 5; // time in ms
  ////////////////////////////////////////

  if (ReadInput() == 17) // 17 is the number for "A" on the keypad
  {
    for (R_MotorPWM = 0; R_MotorPWM <= 255; R_MotorPWM = R_MotorPWM + 1)
    { // clockwise rotation, speed up
      analogWrite(R_PWM, R_MotorPWM);
      analogWrite(L_PWM, 0);
      getMotorRPM();

      while (delayTimeExpired(PauseBetweenSteps) == false)
      {
        getMotorRPM(); // NOT WORKING UNTIL NOW
      }
    }

    while (delayTimeExpired(1000) == false) // 2000 ms delay until the motor gets slower again
    {
      getMotorRPM();
    }

    for (R_MotorPWM = 255; R_MotorPWM >= 0; R_MotorPWM = R_MotorPWM - 1)
    { // clockwise rotation, speed down
      analogWrite(R_PWM, R_MotorPWM);
      analogWrite(L_PWM, 0);
      getMotorRPM();

      while (delayTimeExpired(PauseBetweenSteps) == false)
      {
        getMotorRPM(); // NOT WORKING UNTIL NOW
      }
    }
  }

  else
  {
    analogWrite(R_PWM, 0);
  }
}

//------------ Function to increase the motor speed in clockwise direction -----------
void R_MotorGoToSpeed(int RampDelayTime, int goTo_R_PWM_Speed)
{
  if (R_MotorPWM <= goTo_R_PWM_Speed)
  {
    executeMeasurements();
    // Serial.println("ENTERED"); // for debugging
    for (; R_MotorPWM < goTo_R_PWM_Speed; R_MotorPWM = R_MotorPWM + 1)
    {
      analogWrite(R_PWM, R_MotorPWM);
      analogWrite(L_PWM, 0);
      // ---------- measurements to execute: -------------
      executeMeasurements();
      // -------------------------------------------------
      while (delayTimeExpired(RampDelayTime) == false)
      {
        // ---------- measurements to execute: -------------
        executeMeasurements();
        // -------------------------------------------------
      }
    }
  }
  else if (R_MotorPWM > goTo_R_PWM_Speed)
  {
    for (; R_MotorPWM > goTo_R_PWM_Speed; R_MotorPWM = R_MotorPWM - 1)
    {
      analogWrite(R_PWM, R_MotorPWM);
      analogWrite(L_PWM, 0);
      // ---------- measurements to execute: -------------
      executeMeasurements();
      // -------------------------------------------------
      while (delayTimeExpired(RampDelayTime) == false)
      {
        // ---------- measurements to execute: -------------
        executeMeasurements();
        // -------------------------------------------------
      }
    }
  }
  else
  {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
  }
}

//------------ Function to increase the motor speed in counter clockwise direction -----------
// a higher "RampDelayTime" value makes the ramp slower and softer
void L_MotorGoToSpeed(int RampDelayTime, int goTo_L_PWM_Speed)
{
  if (L_MotorPWM <= goTo_L_PWM_Speed)
  {
    // ---------- measurements to execute: -------------
    executeMeasurements();
    // -------------------------------------------------
    // Serial.println("ENTERED"); // for debugging
    for (; L_MotorPWM < goTo_L_PWM_Speed; L_MotorPWM = L_MotorPWM + 1)
    {
      analogWrite(R_PWM, 0);
      analogWrite(L_PWM, L_MotorPWM);
      // ---------- measurements to execute: -------------
      executeMeasurements();
      // -------------------------------------------------
      while (delayTimeExpired(RampDelayTime) == false)
      {
        // ---------- measurements to execute: -------------
        executeMeasurements();
        // -------------------------------------------------
      }
    }
  }
  else if (L_MotorPWM > goTo_L_PWM_Speed)
  {
    for (; L_MotorPWM > goTo_L_PWM_Speed; L_MotorPWM = L_MotorPWM - 1)
    {
      analogWrite(R_PWM, 0);
      analogWrite(L_PWM, L_MotorPWM);
      // ---------- measurements to execute: -------------
      executeMeasurements();
      // -------------------------------------------------
      while (delayTimeExpired(RampDelayTime) == false)
      {
        // ---------- measurements to execute: -------------
        executeMeasurements();
        // -------------------------------------------------
      }
    }
  }
  else
  {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
  }
}

//--------- Function to stop the motor to PWM 0 with a ramp from any speed -------------------
// a higher "RampDelayTime" value makes the ramp slower and softer
void MotorStop(int rampDelayTime)
{
  if (R_MotorPWM > 0) // stop if the motor is turning in clockwise direction
  {
    for (; R_MotorPWM >= 0; R_MotorPWM = R_MotorPWM - 1)
    {
      analogWrite(R_PWM, R_MotorPWM);
      analogWrite(L_PWM, 0);
      delay(rampDelayTime);
    }
  }
  else
  {
    R_MotorPWM = 0;
  }

  if (L_MotorPWM > 0) // stop if the motor is turning in counter-clockwise direction
  {

    for (; L_MotorPWM >= 0; L_MotorPWM = L_MotorPWM - 1)
    {
      analogWrite(R_PWM, 0);
      analogWrite(L_PWM, L_MotorPWM);
      delay(rampDelayTime);
    }
  }
  else
  {
    L_MotorPWM = 0;
  }
}

//----------- Function to calculate the motor speed from a potentiometer ---------------
//---------------------------- Returns the PWM speed -----------------------------------
//---------- Variables -----------
int readPotiInput;
int processedPotiInputRight;
int processedPotiInputLeft;
int writePWM;

// constants
int motorStopZone = 60;
float maxPWM = 255;
int minReadInput = 0;
int maxReadInput = 1024;

// gradient calculations
int minMotorTurnRigth = maxReadInput / 2 + motorStopZone; // calculates the sport
int maxMotorTurnLeft = maxReadInput / 2 - motorStopZone;
float gradientPoti = (maxPWM / (maxReadInput - minMotorTurnRigth)); // delta y / delta x, Motor turns in one direction if the analog is between maxReadInput and minPositivePoti

int MotorPotiCalculatePWM()
{ // possible values range from poti: 0 - 1024
  readPotiInput = analogRead(PotiInput);

  if (readPotiInput >= maxMotorTurnLeft && readPotiInput <= minMotorTurnRigth) // Values in the stop zone --> writePWM = 0
  {
    writePWM = 0;
  }
  else
  {
    if (readPotiInput > minMotorTurnRigth) // condition for positive PWM signal, turn right
    {
      processedPotiInputRight = readPotiInput - minMotorTurnRigth; // substract minPositivePoti from readPotiInput, therefore the PWM calculation starts with 0
      writePWM = processedPotiInputRight * gradientPoti;
    }

    else if (readPotiInput < maxMotorTurnLeft) // condtition for negative PWM signal, turn left
    {
      processedPotiInputLeft = readPotiInput - maxMotorTurnLeft; // substract maxNegativePoti from readPotiInput, therefore the PWM calculation starts with 0
      writePWM = processedPotiInputLeft * gradientPoti;          // write PWM gets negative --> negative values are processed later
    }
    else
    {
    }
  }
  return writePWM;
}

//----------- Function to control the motor with a a potentiometer ---------------
void MotorControlPoti()
{
  // executeMeasurements();
  // sendData();

  // ####### only for testing ##########
  // if( delayTimeExpired(100))
  // {
  // Serial.print("Motor current function: ");
  // Serial.print(getMotorCurrent());
  // Serial.print("\n");
  // }

  if (MotorPotiCalculatePWM() > 0) // motor turn right
  {
    analogWrite(R_PWM, MotorPotiCalculatePWM());
    analogWrite(L_PWM, 0);
  }
  else if (MotorPotiCalculatePWM() < 0) // motor turn left
  {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, abs(MotorPotiCalculatePWM()));
  }
  else
  {
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
  }
}