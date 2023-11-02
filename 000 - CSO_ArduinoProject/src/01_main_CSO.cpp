#include <Arduino.h>
#include "99_globals.h"

// put function declarations here:

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Hello World");
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  


}