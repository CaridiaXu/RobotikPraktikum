#include <mcp_can.h>

#include <SPI.h>

//Serial Peripherial Interface (SPI) Library

#include "gim8115.h"  //Include object

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


void setup() {
  Serial.begin(230400); //Begin Serial port

  while(CAN_OK != CAN.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ))  //Begin the CAN Bus and set frequency to 8 MHz and baudrate of 1000kb/s  and the masks and filters disabled.
  {
    //Serial.println("CAN Bus initialization failed"); 
    //Serial.println("Initializing CAN Bus again");
    delay(100);
    }
    CAN.setMode(MCP_NORMAL);  // Change to normal mode to allow messages to be transmitted and received
  //Serial.println("CAN Bus init OK");
   motor.exitMotormode(CAN);
   motor.setMotormode(CAN);
   motor.setZero(CAN);


  //pinMode(CAN_INT, INPUT);  //Configuring the interrupt pin from the MCP(NOT USED)
}


void loop() {
//float pos = 1;
float pos = 0;
pos = Serial.read();
motor.normalSet(CAN, pos, 0, 0); //Put in order: CANObject, desirePosition(rad), maxVelocity(rad/s), maxTorque(rad/s^2)

}
