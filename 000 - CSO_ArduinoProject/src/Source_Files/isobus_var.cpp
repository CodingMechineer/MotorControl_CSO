#include "Header_Files/isobus_var.h"

void printMessage(ISOBUSMessage msg, char buffer[])
{
  if (msg.status == 0){
    Serial.print(msg.status);
    Serial.print(",");
    Serial.print(msg.id);
    Serial.print(",");
    Serial.print(msg.pgn);
    Serial.print(",");
    Serial.print(msg.spn_data);
    Serial.print(",");
    Serial.println(buffer);
  }
}