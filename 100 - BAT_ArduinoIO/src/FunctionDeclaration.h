#ifndef Function_Declaration_h
#define Function_Declaration_h

////////////////////////////////////////////
//------ Declarate motor functions -------//
////////////////////////////////////////////
// function to test the motor
void MotorTest();

// function to calculate the motor speed regarding a potentiometer, returns the PWM speed
int MotorPotiCalculatePWM();

// function to control the motor PWM speed and direction with the potentiometer 
void MotorControlPoti();

// function to control the motor with the PWM speed "goTo_R_PWM_Speed"
// a higher "RampDelayTime" in [ms] makes the ramp slower and softer
// recommended values between 1 and 10 ms
void R_MotorGoToSpeed(int RampDelayTime, int goTo_R_PWM_Speed);

void L_MotorGoToSpeed(int RampDelayTime, int goTo_L_PWM_Speed);

// function to stop the motor
// 'rampDelayTime' is the value between the PWM steps decreasing
// recommended values between 1 and 10 ms
void MotorStop(int rampDelayTime);


////////////////////////////////////////////
//---- Declarate measurement functions ---//
////////////////////////////////////////////

// function to test the hall sensor
void HallSensorTest();

// function returns the measured motor current
float getMotorCurrent();

// function returns the measured motor voltage
float getMotorVoltage();

// returns the RPM of the motor measured with the hall sensor
// in u/min
int getMotorRPM();

// function to implement all the measurements in the automatic mode
void executeMeasurements();

// function to handle and write the collected data to Python 
void sendData();


////////////////////////////////////////////
//------ Declarate common functions ------//
////////////////////////////////////////////

// read from serial monitor and from the keypad
int ReadInput();

// print menu in serial monitor
void printMenuInSerial();

// print the choosen case
void PrintChoosenCase(char String[]);

// leave menu if button '9' is pressed
bool LeaveProgram();

// function to avoid using delay(), delayTime in milliseconds
bool delayTimeExpired(int delayTime);

////////////////////////////////////////////////
//------ Declarate rideHeight functions ------//
////////////////////////////////////////////////

// returns the number of increments which are required until the system is in position
int requiredIncrements();

// counts and returns the number of steps from the hall sensor to control the ride height
int getNumberOfSteps( bool reset );

// returns the status of the position ( 'true' if the position is reached, 'false' if not )
bool goToUpperPosition();
bool goToLowerPosition();

////////////////////////////////////////////
//------ Declarate Keypad functions ------//
////////////////////////////////////////////

// prints the button pressed on the keypad
void PrintKeypad();

// returns the value pressed on the keypad
int returnIntKeypad();

#endif