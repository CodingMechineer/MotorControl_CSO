#include "Arduino.h"
#include "Globals.h"
#include "FunctionDeclaration.h"
#include <ACS712.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------- function to implement all the measurements ------------------
int momentaryMotorCurrent;
int momentaryMotorRPM;

void executeMeasurements()
{
  if (initializeMeasurements == true)
  {
    initializeMeasurements = false;
  }
  else
  {
  }

  getMotorRPM();

  // momentaryMotorCurrent = getMotorCurrent();

  // if (delayTimeExpired(500) == true) // prints every 0.5 seconds
  // {
  //   // Serial.println(String("Current: ") + momentaryMotorCurrent + " A");
  //   Serial.println( momentaryMotorCurrent );
  // }
  // else
  // {
  // }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//------------ Function to read inputs from the HallSensor -----------
bool readTop;
bool readTop_Prev;
bool readBottom;
bool readBottom_Prev;
bool readRPM;
bool readRPM_Prev;



void HallSensorTest()
{
  readTop = digitalRead(sensorTop);
  readBottom = digitalRead(sensorBottom);
  readRPM = digitalRead(sensorRotation);

  if(readTop != readTop_Prev){
    Serial.println(String("Sensor_Top: ") + readTop);
  }
  readTop_Prev = readTop;

  if(readBottom != readBottom_Prev){
  Serial.println(String("Sensor_Bottom: ") + readBottom);
  }
  readBottom_Prev = readBottom;

  if(readRPM != readRPM_Prev){
  Serial.println(String("Sensor_RPM: ") + readRPM);
  }
  readRPM_Prev = readRPM;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


//------- Function to calculate the motor rpm -----------
//------------ Variables -------------
bool rpmSignalDetected = false;
unsigned long TimeNow = 0;
unsigned long TimePrevious = 0;
int TimeDifference = 0;
int numberOfTriggers = 6; // number of magnets which indicates a trigger signal
float RoundsPerMillisecond = 0;
float RoundsPerMinute = 0;

//------------ Function to measure the rpm of the motor --------------
int getMotorRPM()
{
  if (digitalRead(sensorRotation) == false && rpmSignalDetected == false)
  {
    rpmSignalDetected = true;
    TimeNow = millis();
    TimeDifference = TimeNow - TimePrevious;
    TimePrevious = TimeNow;
    // Serial.print("TimeDifference:"); // for debugging
    // Serial.println(TimeDifference);
    RoundsPerMillisecond = 1.0 / TimeDifference; // 1.0 is very important, otherwise the values won't get calculated in float
    // Serial.print("RoundsPerMillisecond:"); // for debugging
    // Serial.println(RoundsPerMillisecond);
    RoundsPerMinute = RoundsPerMillisecond * 1000 * 60 / numberOfTriggers; // milliseconds * 1000 --> seconds, seconds * 60 --> minutes

    // ###########################################
    // Serial.print("RPM:"); // for debugging
    // Serial.println(int(RoundsPerMinute), 1); // 'int()' for python, otherwise error
    // ###########################################
  }

  else if (digitalRead(sensorRotation) == true && rpmSignalDetected == true)
  {
    rpmSignalDetected = false;
  }

  else
  {
  }

  return RoundsPerMinute;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------- Function returns the measured motor voltage -----------------
float inputVoltage = 26.0; // V --> measured with multimeter
int outputRange = 1024; // V --> measured with multimeter
float resolution = inputVoltage / outputRange;
float measuredInput;
float measuredVoltage;
                             
float U_Motor_Sum = 0;
float U_Motor_Avg = 0;

int numReadings_U = 30;
int index_U = 0; // index of the current reading

float getMotorVoltage()
{
  measuredInput = analogRead(voltageSensorInput);
  measuredVoltage = measuredInput * resolution;

  if (index_U < numReadings_U)
  {
    U_Motor_Sum = U_Motor_Sum + measuredVoltage;
    index_U++;
  }
  else if (index_U >= numReadings_U)
  {
    U_Motor_Avg = U_Motor_Sum / index_U;
    index_U = 0;
    U_Motor_Sum = 0;
  }

  // //----------- for debugging ----------------
  // if (delayTimeExpired(1000) == true)
  // {
  //   Serial.println(String("U_average\t = ") + U_Motor_Avg + " V");
  //   Serial.println(String("U\t\t = ") + measuredVoltage + " V");
  // }

  return U_Motor_Avg;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//------------ Function to return the measured motor current --------------
//------------ initializing of the current sensor ---------
ACS712 sensor(ACS712_30A, currentSensorInput); // Sensor model with 30 A maximal current, input Nr. "currentSensorInput" gets used
float I_Motor = 0;                             // value read from the sensor
float I_Motor_Sum = 0;
float I_Motor_Avg = 0;

int I_numReadings = 30;
int I_index = 0; // index of the current reading

float getMotorCurrent()
{
  if (initializeProgram == true)
  {
    sensor.calibrate();
    Serial.println("Sensor got calibrated");
    // Serial.println("Calibrate the sensor again by pressing 'D'");
  }

  I_Motor = sensor.getCurrentDC();
  if (I_index < I_numReadings)
  {
    I_Motor_Sum = I_Motor_Sum + I_Motor;
    I_index++;
  }
  else if (I_index >= I_numReadings)
  {
    I_Motor_Avg = I_Motor_Sum / I_index;
    I_index = 0;
    I_Motor_Sum = 0;
  }

  // valueReadInput = ReadInput();
  // if (valueReadInput == 20) // 20 ist the ASCII number for D
  // {
  //   sensor.calibrate();
  //   Serial.println("Sensor got calibrated");
  // }

  // //----------- for debugging ----------------
  // if (delayTimeExpired(1000) == true)
  // {
  //   Serial.println(String("I_average\t = ") + I_Motor_Avg + " A");
  //   Serial.println(String("I\t\t = ") + I_Motor + " A");
  // }

  return (I_Motor_Avg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// -------------- Function to handle and write the collected data -------------
//----- definition and declaration of the collectedData structure ------
struct data
{
  char startByte1;
  char startByte2;
  char startByte3;
  unsigned long time;
  unsigned int RPM;
  int current;
  unsigned int voltage;
};

data collectedData;
int factorInt = 100; // same factor needed in python!!!!

// int size_data = sizeof(data);

void sendData()
{
  collectedData.startByte1 = 'A';
  collectedData.startByte2 = 'X';
  collectedData.startByte3 = '0';
  collectedData.time = millis();
  collectedData.RPM = getMotorRPM();
  collectedData.current = getMotorCurrent() * factorInt; // conversion from float to int
  collectedData.voltage = getMotorVoltage() * factorInt; // conversion from float to int

  if (delayTimeExpired(400))
  { // prints the collectedData every 500 ms

    // Serial.print("Motor current sent: ");
    // Serial.print(collectedData.current);
    // Serial.print("\n");

    Serial.write((uint8_t *)&collectedData, sizeof(collectedData));
    Serial.print("\n");
  }
}

// ############## Tried to program the current measurement ###########################

// measureCurrent();

// float currentPerUnit; // analog input has the range 0 - 1024 --> Resolution of 1 step
// float resultingVoltageFromInput; // calculated pending voltage of the analog input
// int analogReadCurrent; // Variable for the analogRead of the input
// float measuredCurrent; // Calculated current which the sensor measures
// int numberOfInputSteps;
// int arduinoVoltage;
// float currentSensorSensitivity;

// numberOfInputSteps = 1024; // Resolution of the input
// arduinoVoltage = 5; // Voltage in V
// currentSensorSensitivity = 0.066; // 0.066 Volt / Ampere

// currentPerUnit = arduinoVoltage / numberOfInputSteps; // 0.0049 V per unit on the arduino input --> 1024 * 0.00488 V = 5 V

// analogReadCurrent = analogRead(currentSensorInput); // Sensor delivers 66mV / A
// resultingVoltageFromInput = analogReadCurrent * currentPerUnit; // in V

// measuredCurrent = resultingVoltageFromInput / currentSensorSensitivity;

// Serial.print("Input:");
// Serial.println(analogReadCurrent);

// Serial.print("Strom:");
// Serial.print(measuredCurrent);
// Serial.println("A");

// controllerAlarm = digitalRead(alarmMotorController);

// if ( controllerAlarm == HIGH ){
//   Serial.println("Motor alarm");
// }
