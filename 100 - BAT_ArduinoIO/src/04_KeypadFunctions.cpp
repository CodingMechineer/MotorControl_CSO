#include "Arduino.h"
#include "KeypadDeclaration.h"

void PrintKeypad()
{
    char key = keypad.getKey();

    if (key > 0)
    {
        Serial.println(key);
        Serial.println(int(key));
    }
}


int returnIntKeypad()
{
    char key = keypad.getKey();
    int number = int(key) - 48; // also possible: " minus '0' " // transforms the char variable to a int variable
    
    return number;
}