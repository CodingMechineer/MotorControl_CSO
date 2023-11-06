#include <Arduino.h>
#include "99_globals.h"

// #include <Canbus.h>
// #include <defaults.h>
// #include <global.h>
// #include <mcp2515.h>
// #include <mcp2515_defs.h>


// char UserInput;
// int data;
// char buffer[456];  //Data will be temporarily stored to this buffer before being written to the file

// void setup(){
// Serial.begin(115200);
// Serial.println("CAN-Bus Demo");

// if(Canbus.init(CANSPEED_250))  /* Initialise MCP2515 CAN controller at the specified speed */
//   {
//     Serial.println("CAN Init ok");
//   } else
//   {
//     Serial.println("Can't init CAN");
//   } 
   
//   delay(1000); 

// Serial.println("Please choose a menu option.");
// Serial.println("1.Speed");
// Serial.println("2.RPM");
// Serial.println("3.Throttle");
// Serial.println("4.Coolant Temperature");
// Serial.println("5.O2 Voltage");
// Serial.println("6.MAF Sensor");

// }

// void loop(){

// while(Serial.available()){
//    UserInput = Serial.read();

// if (UserInput=='1'){
//  Canbus.ecu_req(VEHICLE_SPEED, buffer);
//  Serial.print("Vehicle Speed: ");
//  Serial.println(buffer);
//  delay(500);
// }
// else if (UserInput=='2'){
//  Canbus.ecu_req(ENGINE_RPM, buffer);
//  Serial.print("Engine RPM: ");
//  Serial.println(buffer);
//  delay(500);

// }
// else if (UserInput=='3'){
//  Canbus.ecu_req(THROTTLE, buffer);
//  Serial.print("Throttle: ");
//  Serial.println(buffer);
//  delay(500);

// }
// else if (UserInput=='4'){
//  Canbus.ecu_req(ENGINE_COOLANT_TEMP, buffer);
//  Serial.print("Engine Coolant Temp: ");
//  Serial.println(buffer);
//  delay(500);

// }
// else if (UserInput=='5'){
//  Canbus.ecu_req(O2_VOLTAGE, buffer);
//  Serial.print("O2 Voltage: ");
//  Serial.println(buffer);
//  delay(500);

// }
// else if (UserInput=='6'){
//  Canbus.ecu_req(MAF_SENSOR, buffer);
//  Serial.print("MAF Sensor: ");
//  Serial.println(buffer);
//  delay(500);

// }
// else
// {
//   Serial.println(UserInput);
//   Serial.println("Not a valid input.");
//   Serial.println("Please enter a valid option.");
// }

// }
// }







// -------------------------------------------------------------------------







// /****************************************************************************
// CAN Read Demo for the SparkFun CAN Bus Shield. 

// Written by Stephen McCoy. 
// Original tutorial available here: http://www.instructables.com/id/CAN-Bus-Sniffing-and-Broadcasting-with-Arduino
// Used with permission 2016. License CC By SA. 

// Distributed as-is; no warranty is given.
// *************************************************************************/

// #include <Canbus.h>
// #include <defaults.h>
// #include <global.h>
// #include <mcp2515.h>
// #include <mcp2515_defs.h>

// //********************************Setup Loop*********************************//

// void setup() {
//   Serial.begin(115200); // For debug use
//   Serial.println("CAN Read - Testing receival of CAN Bus message");  
//   delay(1000);
  
//   if(Canbus.init(CANSPEED_250))  //Initialise MCP2515 CAN controller at the specified speed
//     Serial.println("CAN Init ok");
//   else
//     Serial.println("Can't init CAN");
    
//   delay(1000);
// }

// //********************************Main Loop*********************************//

// void loop(){

//   tCAN message;
// if (mcp2515_check_message()) 
// 	{
//     if (mcp2515_get_message(&message)) 
// 	{
//         //if(message.id == 0x620 and message.data[2] == 0xFF)  //uncomment when you want to filter
//              //{
               
//                Serial.print("ID: ");
//                Serial.print(message.id,HEX);
//                Serial.print(", ");
//                Serial.print("Data: ");
//                Serial.print(message.header.length,DEC);
//                for(int i=0;i<message.header.length;i++) 
//                 {	
//                   Serial.print(message.data[i],HEX);
//                   Serial.print(" ");
//                 }
//                Serial.println("");
//              //}
//            }}

// }










// --------------------------------------------------------------------









// // CAN Receive Example
// //

// #include <mcp_can.h>
// #include <SPI.h>

// long unsigned int rxId;
// unsigned char len = 0;
// unsigned char rxBuf[8];
// char msgString[128];                        // Array to store serial string

// #define CAN0_INT 2                              // Set INT to pin 2
// MCP_CAN CAN0(10);                               // Set CS to pin 10


// void setup()
// {
//   Serial.begin(115200);
  
//   // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
//   if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)
//     Serial.println("MCP2515 Initialized Successfully!");
//   else
//     Serial.println("Error Initializing MCP2515...");
  
//   CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

//   pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
//   Serial.println("MCP2515 Library Receive Example...");
// }

// void loop()
// {
//   if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
//   {
//     CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
//     if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
//       sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
//     else
//       sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
//     Serial.print(msgString);
  
//     if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
//       sprintf(msgString, " REMOTE REQUEST FRAME");
//       Serial.print(msgString);
//     } else {
//       for(byte i = 0; i<len; i++){
//         sprintf(msgString, " 0x%.2X", rxBuf[i]);
//         Serial.print(msgString);
//       }
//     }
        
//     Serial.println();
//   }
// }

// /*********************************************************************************************************
//   END FILE
// *********************************************************************************************************/












// --------------------------------------------------------------------










#include <SPI.h>
#include <ISOBUS.h>

char spn_buffer[512];  //Data will be temporarily stored to this buffer before being written to the filechar buffer[512];  //Data will be temporarily stored to this buffer before being written to the file

ISOBUSMessage receiveMessage;

void setup()
{
  ISOBUS.begin(CAN_SPEED_250000);
  ISOBUS.setMode (CAN_MODE_NORMAL);
  Serial.begin(115200);

  // initialize digital pin 2 and 3 as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop()
{
  /* Receive data if available */
  if (ISOBUS.available()) {
    receiveMessage = ISOBUS.getMessageISOBUS(EEC1_PGN, EngineSpeed_SPN, spn_buffer);
    if (receiveMessage.status == 0)
    {
      Serial.print(receiveMessage.status);
      Serial.print(",");
      Serial.print(receiveMessage.id);
      Serial.print(",");
      Serial.print(receiveMessage.pgn);
      Serial.print(",");
      Serial.print(receiveMessage.spn_data);
      Serial.print(",");
      Serial.println(spn_buffer);

      // control outputs based on engine speed
      if (receiveMessage.spn_data > 850) {
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
      }
      else {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
      }

    }
  }
}
