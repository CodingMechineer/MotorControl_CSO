#ifndef GLOBALS_H
#define GLOBALS_H

#define SERIAL_SPEED 115200

//----------- Pin's BTS7960 Motor Driver ----------------
//const int R_IS = ???; // Forward Drive, Side current alarm output, !!! NOT CONNECTED !!!
//const int L_IS = ???; // Reverse Drive, Side current alarm output, !!! NOT CONNECTED !!!
//const int R_EN = ???; // Forward Drive Enable Input, Active High / Low Disable, !!! set HIGH with breadboard !!!
//const int L_EN = ???; // Reverse Drive Enable Input, Active High / Low Disable, !!! set HIGH with breadboard !!!

//////////////////////////////////////////////////////////////
//      ***** Contacts and cable colors H-Bridge *****      //
//                                                          // 
//      R_PWM   --> green                                   //
//      L_PWM   --> yellow                                  //
//      R_EN    --> red     --> 5V                          //
//      L_EN    --> orange  --> 5V                          //
//      R_IS    -->                                         //
//      L_IS    -->                                         //
//      VCC     --> brown   --> 5V                          //
//      GND     --> black   --> 0V                          //
//                                                          //
//////////////////////////////////////////////////////////////
const int R_PWM = 5; // change to another PWM output --> NOT 5 or 6
const int L_PWM = 3;

extern int R_MotorPWM; // PWM value of the output for clockwise rotation
extern int L_MotorPWM; // PWM value of the output for counterclockwise rotation

//---------- Pin's ---------------------
const int sensorTop = 14;
const int sensorBottom = 15;
const int sensorRotation = 4;
const int PotiInput = A2;
const int currentSensorInput = A3;
const int voltageSensorInput = A2; // not active!
const int encoderCLK = 2;
const int encoderDT = A4;
const int encoderSW = A5;

//---------- Variables menu -------------
extern int menuChoice;
extern bool initializeProgram;

extern int valueReadInput;

//----- Variables plotting command ------
extern bool initializeMeasurements;


//----- variable needs to get accessed from everywhere -------
extern int stepsCounted;

#endif