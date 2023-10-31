//---------------- Includes --------------------
#include "Arduino.h"
#include "FunctionDeclaration.h"
#include "Globals.h"
#include <ky-040.h>

//---- for debugging ----
// #include "avr8-stub.h"
// #include "app_api.h" // only needed with flash breakpoints

//-------------- Variables --------------------
int menuChoice = 0;
int valueReadInput; // variable to assign the value from the readInput() function

//-------------- Menu prints ------------------
bool menuPrintState = true;    // to print the menu to choose the different programs
bool initializeProgram = true; // to initialize every program in the switch case

//------------- Measurements -----------------
bool initializeMeasurements = true;

//------------- Testing variables -------------
int newVariable;

float testCurrent;
float testVoltage;

//------------- Variables for 'Change ride height' -------------
struct positions
{
  bool remaining = false;
  bool upperPosition = false;
  bool lowerPosition = false;
  bool movingUp = false;
  bool movingDown = false;
} statusRideHeight;

int switchCaseAutomatic = 0; // switch case starts with the 'remaining' menu
bool initializeCase = true;  // initializes the case in the first call

bool reachedPosition;



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//------------- Variables for encoder -------------
/* 
 * Define some rotary IDs. IDs can range from 0 to 254. 255 is reserved for the active one
 * ( CURRENT_ID )
 */
#define ROTARY_ID1           1 
#define ROTARY_ID2          10 
/*
 * Create encoders. 
 * Define how the encoder is connected to the Arduino. ENCODER_CLK is the interrupt pin
 */
// First Encoder
#define ENCODER_CLK1        encoderCLK      // This pin must have a minimum 0.47 uF capacitor
#define ENCODER_DT1         encoderDT      // data pin
#define ENCODER_SW1         encoderSW      // switch pin (active LOW)
#define MAX_ROTARIES1        2      // this example defines two rotaries for this encoder
ky040 encoder1(ENCODER_CLK1, ENCODER_DT1, ENCODER_SW1, MAX_ROTARIES1 );


void Error ( int encoderNum ) {
    Serial.print("ERROR creating encoder "); Serial.print(encoderNum); Serial.println("!");
    Serial.println("Possible malloc error, the rotary ID is incorrect, or too many rotaries");
    while(1);   // Halt and catch fire
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



/*+++++++++++++++++++++++++++++++++++++++++++++
++---------------- Setup --------------------++
+++++++++++++++++++++++++++++++++++++++++++++*/
void setup()
{

  Serial.begin(SERIAL_SPEED);

  //------------ declarate OUTPUT Pin's ---------
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  // ### EXPAND not connected pins ###
  // pinMode(R_IS, OUTPUT);  // not connected on arduino pin
  // pinMode(L_IS, OUTPUT);  // not connected on arduino pin
  // pinMode(R_EN, OUTPUT);  // not connected on arduino pin
  // pinMode(L_EN, OUTPUT);  // not connected on arduino pin

  //------------ declarate INPUT Pin's ----------
  pinMode(PotiInput, INPUT);
  pinMode(currentSensorInput, INPUT);
  // pinMode(voltageSensorInput, INPUT);
  pinMode(sensorTop, INPUT);
  pinMode(sensorBottom, INPUT);
  pinMode(sensorRotation, INPUT);

  //--------------- Set outputs ---------------
  digitalWrite(L_PWM, 0);
  digitalWrite(R_PWM, 0);

  // ### EXPAND not connected pins
  // digitalWrite(R_IS, LOW);    // not connected on arduino pin
  // digitalWrite(L_IS, LOW);    // not connected on arduino pin
  // digitalWrite(R_EN, HIGH);   // not connected on arduino pin
  // digitalWrite(L_EN, HIGH);   // not connected on arduino pin

  //--------------- Initialization for 'Change ride height' mode ----------------
  statusRideHeight.lowerPosition = true; // Initialize so the system is able to start



  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  //--------------- Encoder ----------------
  // NOTE: The AddRotaryCounter function is declared a void type if SMALLEST_CODESIZE if defined in ky040.h
  
  // Add the two rotaries for the first encoder.
  // Define a rotary to go from -20 to 20 in increments of 2. Initialize it to 10. Allow it to rollover

  if ( ! encoder1.AddRotaryCounter(ROTARY_ID1, 10, -20, 20, 2, true ) ) {
    Error(1);
  }

  // Define a rotary to go from 50 to 100 in increments of 10. Initialize it to 70. no rollover    
  if ( ! encoder1.AddRotaryCounter(ROTARY_ID2, 0, 0, 100, 1, false ) ) {
    Error(1);
  }

  // Make ROTARY_ID2 active (responds to encoder shaft movements)
  encoder1.SetRotary(ROTARY_ID2);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/*+++++++++++++++++++++++++++++++++++++++++++++
++---------------- Loop ---------------------++
+++++++++++++++++++++++++++++++++++++++++++++*/
void loop()
{
  switch (menuChoice)
  {
  ////////////////////////////////////////////
  //------------ Menu choice ---------------//
  ////////////////////////////////////////////
  case 0:
    initializeProgram = true;

    if (menuPrintState == true)
    {
      printMenuInSerial(); // prints the menu which can be choosen
      menuPrintState = false;
    }

    menuChoice = ReadInput(); // Chooses the menu with the PC keyboard or keypad

    if (menuChoice != 0)
    {
      menuPrintState = true;
    }

    break;

  /////////////////////////////////////////////////////////////////
  //------------------- Change ride height ----------------------//
  /////////////////////////////////////////////////////////////////
  case 1:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Change ride height got choosen:");
      Serial.println("Press '1' to change the position\n");
    }
    // #################################################################

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // make a reference drive to know where the system is
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    valueReadInput = ReadInput();
    if (valueReadInput == 20) // 20 is the value for D
    {
      MotorStop(1); // the motor stops within 1ms
      switchCaseAutomatic = 0;
    }

    switch (switchCaseAutomatic)
    {
    case 0: // remainPosition, write error if there's no signal from the hall sensor
      if (initializeCase == true)
      {
        Serial.println("Ride height is remaining in position");
        initializeCase = false;
        statusRideHeight.remaining = true;
      }

      if (statusRideHeight.remaining == true)
      {
        valueReadInput = ReadInput();
        //-------------------- condition goToUpperPosition ---------------------
        if (valueReadInput == 1 && statusRideHeight.lowerPosition == true && statusRideHeight.upperPosition == false && statusRideHeight.movingDown == false && statusRideHeight.movingUp == false)
        {
          Serial.println("switchCaseAutomatic = 1");
          switchCaseAutomatic = 1;
          statusRideHeight.remaining = false;
          statusRideHeight.lowerPosition = false;
          initializeCase = true;
        }
        //-------------------- condition goToLowerPosition ---------------------
        else if (valueReadInput == 1 && statusRideHeight.upperPosition == true && statusRideHeight.lowerPosition == false && statusRideHeight.movingDown == false && statusRideHeight.movingUp == false)
        {
          Serial.println("switchCaseAutomatic = 2");
          switchCaseAutomatic = 2;
          statusRideHeight.remaining = false;
          statusRideHeight.upperPosition = false;
          initializeCase = true;
        }
      }
      break;

    case 1: // goToUpperPosition, motor has to turn counter-clockwise to move up !!! L_PWM !!!
      if (initializeCase == true)
      {
        Serial.println("Ride height changes to upper position");
        statusRideHeight.movingUp = true;
        initializeCase = false;
      }
      // --------------------------------------------------------------
      reachedPosition = goToUpperPosition();

      if (reachedPosition == true)
      {
        Serial.println("Reached upper ride height");
        statusRideHeight.movingUp = false;
        statusRideHeight.upperPosition = true;
        switchCaseAutomatic = 0; // change to the "remaining" switch case
        initializeCase = true;
      }
      // --------------------------------------------------------------
      break;

    case 2: // goToLowerPosition, motor has to turn clockwise to move down !!! R_PWM !!!
      if (initializeCase == true)
      {
        Serial.println("Ride height changes to lower position");
        statusRideHeight.movingDown = true;
        initializeCase = false;
      }
      // --------------------------------------------------------------
      reachedPosition = goToLowerPosition();

      if (reachedPosition == true)
      {
        Serial.println("Reached lower ride height");
        statusRideHeight.movingDown = false;
        statusRideHeight.lowerPosition = true;
        switchCaseAutomatic = 0; // change to the "remaining" switch case
        initializeCase = true;
      }

      // --------------------------------------------------------------
      break;

    default:
      Serial.print("Default, what happened?\n");
      break;
    }

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  /////////////////////////////////////////////////////////////////
  //-------------------- Hall sensor test -----------------------//
  /////////////////////////////////////////////////////////////////
  case 2:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Hall sensor test got choosen:");
    }
    // #################################################################

    HallSensorTest();

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  //////////////////////////////////////////////////////////////////////
  //-------------------- Different motor speed  ----------------------//
  //////////////////////////////////////////////////////////////////////
  case 3:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Different motor speed got choosen:");
      Serial.print("Choose a motor speed with '1', '2' and '3'\n");
      Serial.print("Stop the motor with '8'\n");
    }
    // #################################################################

    valueReadInput = ReadInput();

    if (valueReadInput == 1)
    {
      Serial.println("Motor speed 1");
      R_MotorGoToSpeed(8, 50);
    }
    else if (valueReadInput == 2)
    {
      Serial.println("Motor speed 2");
      R_MotorGoToSpeed(8, 200);
    }
    else if (valueReadInput == 3)
    {
      Serial.println("Motor speed 3");
      R_MotorGoToSpeed(2, 255);
    }
    else if (valueReadInput == 8)
    {
      Serial.println("Motor stop");
      MotorStop(10);
    }
    else
    {
    }

    // #################################################################
    initializeProgram = false;
    if (LeaveProgram() == true)
    {
      MotorStop(3);
    }
    break;

  ////////////////////////////////////////////////////////////////
  //------------------------ Motor test ------------------------//
  ////////////////////////////////////////////////////////////////
  case 4:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Motor test got choosen:");
      Serial.println("Press 'A' to test the motor");
    }
    // #################################################################

    MotorTest();

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  ////////////////////////////////////////////////////////////////
  //------------------------ Poti test ------------------------//
  ////////////////////////////////////////////////////////////////
  case 5:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Poti test got choosen:");
    }
    // #################################################################

    if (delayTimeExpired(300) == true){
      Serial.println(MotorPotiCalculatePWM());
    }

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  ///////////////////////////////////////////////////////////
  //------------- Control motor with poti -----------------//
  ///////////////////////////////////////////////////////////
  case 6:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"Control motor with poti got choosen:");
    }
    // #################################################################

    sendData();

    MotorControlPoti();

    //---------------- Endlagenabschaltung mit dem Poti -------------------------
    // if (MotorPotiCalculatePWM() > 0 && digitalRead(sensorBottom) == true) // motor turn right
    // {
    //   R_MotorPWM = MotorPotiCalculatePWM();
    //   analogWrite(R_PWM, R_MotorPWM); 
    //   analogWrite(L_PWM, 0);
    // }
    // else if (MotorPotiCalculatePWM() < 0 && digitalRead(sensorTop) == true) // motor turn left
    // {
    //   L_MotorPWM = abs(MotorPotiCalculatePWM());
    //   analogWrite(R_PWM, 0);
    //   analogWrite(L_PWM, L_MotorPWM);
    // }
    // else
    // {
    //   L_MotorPWM = 0;
    //   R_MotorPWM = 0;
    //   MotorStop(3);
    // }
    

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  ///////////////////////////////////////////////////////////
  //-------------------- Keypad test ----------------------//
  ///////////////////////////////////////////////////////////
  case 7:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"KEYPAD TEST got choosen:");
    }
    // #################################################################

    PrintKeypad();

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  ///////////////////////////////////////////////////////////
  //----------------------- TESTING -----------------------//
  ///////////////////////////////////////////////////////////
  case 8:
    if (initializeProgram == true)
    {
      PrintChoosenCase((char *)"TESTING got choosen:");
    }
    // #################################################################

    static bool firstTime = true;   // Force an initial display of rotary values

    // On each encoder switch press, toggle between the two rotaries.
    // Use the IsActive call 
    if ( encoder1.SwitchPressed() or firstTime ) {
        Serial.print("Encoder 1 switch pressed. ");
        if ( encoder1.IsActive(ROTARY_ID1) ) {
            encoder1.SetRotary(ROTARY_ID2);
            Serial.println("ROTARY_ID2 now Active.");
        }
        else {
            encoder1.SetRotary(ROTARY_ID1);
            Serial.println("ROTARY_ID1 now Active.");
        }
        encoder1.SetChanged();   // force an update on active rotary
    }

    if ( encoder1.HasRotaryValueChanged() ) {
    Serial.print("Encoder 1 ROTARY_ID1: "); Serial.println(encoder1.GetRotaryValue(ROTARY_ID1));
    Serial.print("Encoder 1 ROTARY_ID2: "); Serial.println(encoder1.GetRotaryValue(ROTARY_ID2)); 
    
    Serial.println("---------------------------------------"); 
    }

    firstTime = false;    // Now only update display if a rotary value has changed.

    // #################################################################
    initializeProgram = false;
    LeaveProgram();
    break;

  default:
    Serial.print("Please choose a valid selection\n");
    menuChoice = 0;

    break;
  }
}