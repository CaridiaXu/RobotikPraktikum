#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252

#include <TimerOne.h>

#include <mcp_can.h>

#include <SPI.h>

//Serial Peripherial Interface (SPI) Library

#include "gim8115.h" //Include object

#define CAN_INT 2 //CAN BUS interrupt pin (NOT USED)

#define spiCSPin 10 //The MCP2515 CAN Bus is connected to pin 10

float frequence = 0.01;

float amplitude = 3.14;

volatile float pos = 0;

float angular_velocity = 1;

float pos_pertubration = 0;

float time_2 = 1 ;

int pin_interrupt = 2; //Pin 2 is the interrupt

volatile int state = LOW; // Giving the 

//CAN BUS initialization  

MCP_CAN CAN(spiCSPin);  //Setting the CS Pin number

//Motor initialization
gim8115 motor(0x01, 50, 0.5);

//Ethercat 
EasyCAT EASYCAT(9);  //EasyCAT SPI chip select. Standard is pin 9


void setup() {
  Serial.begin(230400); //Begin Serial port

  while(CAN_OK != CAN.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ))  //Begin the CAN Bus and set frequency to 8 MHz and baudrate of 1000kb/s  and the masks and filters disabled.
  {
    Serial.println("CAN Bus initialization failed"); 
    Serial.println("Initializing CAN Bus again");
    delay(100);
    }
    CAN.setMode(MCP_NORMAL);  // Change to normal mode to allow messages to be transmitted and received
   Serial.println("CAN Bus init OK");
   motor.exitMotormode(CAN);
   motor.setMotormode(CAN);
   motor.setZero(CAN);


  //pinMode(CAN_INT, INPUT);  //Configuring the interrupt pin from the MCP(NOT USED)

  //Ethercat

  while (EASYCAT.Init() != true)                                     // initialization
    {                                                               // succesfully completed
    Serial.println ("EASYCAT initialization failed"); 
    delay(500);                                //
    }                                                               //
  
    // else                                                            // initialization failed   
    // {                                                               // the EasyCAT board was not recognized
  Serial.println ("EASYCAT initialization succesful");  

 // Initialize Timer1 to run every 0.5 seconds (twice per second)
  Timer1.initialize(5000);
  // // // Execute for each interruption the function parpadearLED()
  Timer1.attachInterrupt(Reading);

}


void loop() {

  EASYCAT.MainTask();       // execute the EasyCAT task

  delay(10);

  pos = EASYCAT.BufferOut.Byte;
  motor.normalSet(CAN, pos, 2, 2);
  EASYCAT.BufferIn.Byte = pos;

  //Configuration();          //Inputs from laptop
  // Reading();

  // Sending();

//float pos = 1;

  //motor.
  //Put in order: CANObject, desirePosition(rad), maxVelocity(rad/s), maxTorque(rad/s^2)

}

// void Reading () 
// {
// //    if (EASYCAT.BufferOut.Byte[0] != 1)
// //   {
// //       digitalWrite(Operate,LOW); 
// //       delay(10);
// //       digitalWrite(Operate,HIGH);
// //     }
// //   des_pos=EASYCAT.BufferOut.Byte[1];
// //   Serial.write(c);
// //  if (EASYCAT.BufferOut.Byte[2] == 1)
// //  {
// //    digitalWrite(motor_pin,HIGH)
// //    }
// //    else
// //    {
// //          digitalWrite(motor_pin,LOW)
// //      }
// }