#include "Arduino.h"
#include "Globals.h"
#include "FunctionDeclaration.h"

// if a case is entered this function is called and the String[] term gets printed
void PrintChoosenCase(char String[])
{
    Serial.println(String);
    Serial.println("(Press '9' to leave this menu)\n");
}

//------------ Function to leave the choosen program -----------
// if the button '9' is pushed, the menu gets back to the selection
int menuLeft;
int inputVariable;

bool LeaveProgram()
{
  inputVariable = ReadInput();
  if (inputVariable == 9)
  {
    initializeMeasurements = false;

    menuChoice = 0; // MenuChoice set to zero --> SwitchCase "MenuChoice" is active   
    menuLeft = true;
    Serial.print("\n");
    Serial.println("--- Menu left ---");

    
  }
  else
  { 
    menuLeft = false;
  }
  return menuLeft;
}

//------------ Function to replace delay() -----------
//------------ Variables -------------
int startTime;
int pastTime;
bool timeExpired = true;

bool delayTimeExpired(int delayTime){

  int timeNow = millis();

  if (timeExpired == true){
    startTime = timeNow;
    timeExpired = false;
  }
  
  pastTime = timeNow - startTime;

  if (pastTime >= delayTime){
    timeExpired = true;
  }

  return timeExpired;
}

//----------- Function to read the serial input ----------------
int ReadInput()
{
  int valueRead;
  while (Serial.available() > 0)
  {
    // Serial.print("Serial.available > 0"); // only for testing, if program stays in this loop
    valueRead = Serial.parseInt();
  }

  // part where the keypad is read
  int userInput = returnIntKeypad();
  if(userInput > 0){
    valueRead=userInput;
    // Serial.println(ValueRead); // for testing
  }
  else
  {
  }
  
  return valueRead;
}

//------------ Function to print the menu selection -----------
void printMenuInSerial()
{
    Serial.print("\n=========================================\n");
    Serial.print("Choose your program:\n");
    Serial.print("1: Change ride height\n");
    Serial.print("2: Hall sensor test\n");
    Serial.print("3: Different motor speed\n");
    Serial.print("4: Motor test\n");
    Serial.print("5: Poti test\n");
    Serial.print("6: Control motor with poti\n");
    Serial.print("7: Keypad test\n");
    Serial.print("8: TESTING\n");
    Serial.print("===========================================\n");
}