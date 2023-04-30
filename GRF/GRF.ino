#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252
#include <SPI.h>                    // SPI library
#include <TimerOne.h>


//Ethercat 
EasyCAT EASYCAT(9);  //EasyCAT SPI chip select. Standard is pin 9


  //Force Sensors
   int Force_sensor_1_pin = A2;
   int Force_sensor_2_pin = A3;
   int Force_sensor_3_pin = A4;
   int Force_sensor_4_pin = A5;
   int Force_sensor_5_pin = A1;
   int VoltageRead  = 0;

   //Range
   int A_z = 5;
   int B_z = 6; 

   int Force_1 = 0;
   int Force_2 = 0;
   int Force_3 = 0;
   int Force_4 = 0;
   float Total_force = 0;
   int tare = 1;
   float range = 1/0.9;
   int des_pos = 0;

  //Control Pins
  int Operate = 4;

  //Pin Motor comunication
  int motor_pin = 7;
  
void setup(){
  Serial.begin(230400); //Begin Serial port
  
  //Pin Configurations
  //---------------------------------------------------------------------------------------------------------------------------//
  //Operating Pin
  pinMode(Operate, OUTPUT);

  //Pin Motor comunication
  pinMode(motor_pin, OUTPUT);
  //Range Pins
  pinMode(A_z, OUTPUT);
  pinMode(B_z, OUTPUT);
  //Range
  digitalWrite(A_z,LOW);
  digitalWrite(B_z,HIGH);
  delay(100);
  digitalWrite(Operate,HIGH);

  //Ethercat
    if (EASYCAT.Init() == true)                                     // initialization
  {                                                               // succesfully completed
//Serial.print ("initialized");                                 //
  }                                                               //
  
  else                                                            // initialization failed   
  {                                                               // the EasyCAT board was not recognized
    //Serial.print ("initialization failed");                       //     
    //Serial.print('\n');
                                                                  // The most common reason is that the SPI 
                                                                  // chip select choosen on the board doesn't 
                                                                  // match the one choosen by the firmware
                                                                  
    pinMode(13, OUTPUT);                                          // stay in loop for ever
                                                                  // with the Arduino led blinking
    while(1)                                                      //
    {                                                             //   
      digitalWrite (13, LOW);                                     // 
      delay(500);                                                 //   
      digitalWrite (13, HIGH);                                    //  
      delay(500);                                                 // 
    }                                                             // 
   
  } 
   // Initialize Timer1 to run every 0.5 seconds (twice per second)
  Timer1.initialize(5000);
  // Execute for each interruption the function parpadearLED()
  Timer1.attachInterrupt(Reading);
  
}

void loop(){
  EASYCAT.MainTask();       // execute the EasyCAT task

  //Configuration();          //Inputs from laptop
    Reading();

  Force_1 = analogRead(Force_sensor_1_pin);
  Force_2 = analogRead(Force_sensor_2_pin);
  Force_3 = analogRead(Force_sensor_3_pin);
  Force_4 = analogRead(Force_sensor_4_pin);
  if (analogRead(Force_sensor_5_pin) >50)
  {
    VoltageRead = 1;
    }
  else
  {
    VoltageRead = 0;
    }
    
 //Serial.println(Force_1);
 //Serial.println('\n');
  Sending();
    
    
}
void Reading () 
{
   if (EASYCAT.BufferOut.Byte[0] != 1)
  {
      digitalWrite(Operate,LOW); 
      delay(10);
      digitalWrite(Operate,HIGH);
    }
  des_pos=EASYCAT.BufferOut.Byte[1];
  Serial.write(des_pos);
//  if (EASYCAT.BufferOut.Byte[2] == 1)
//  {
//    digitalWrite(motor_pin,HIGH)
//    }
//    else
//    {
//          digitalWrite(motor_pin,LOW)
//      }
    
  }


void Sending () 
{
     unsigned int pos = Serial.read();

   EASYCAT.BufferIn.Byte[8] = pos;   
  //Force 1
   EASYCAT.BufferIn.Byte[0] = (Force_1 >> 8);    
   
   EASYCAT.BufferIn.Byte[1] = (Force_1 & 255);

  //Force 2
   EASYCAT.BufferIn.Byte[2] = (Force_2 >> 8);    
   
   EASYCAT.BufferIn.Byte[3] = (Force_2 & 255);

  //Force 3
   EASYCAT.BufferIn.Byte[4] = (Force_3 >> 8);    
   
   EASYCAT.BufferIn.Byte[5] = (Force_3 & 255);

  //Force 4
   EASYCAT.BufferIn.Byte[6] = (Force_4 >> 8);    
   
   EASYCAT.BufferIn.Byte[7] = (Force_4 & 255);

   // Delete this part later
   EASYCAT.BufferIn.Byte[13] = (VoltageRead);

 
   
   
  }


//void Configuration ()
//{
//  if (Serial.available() > 0) {
//    char input = Serial.read();
//    
//    if(input == 'T'){
//      digitalWrite(Operate,LOW); 
//      delay(100);
//      digitalWrite(Operate,HIGH);
//      }
//      else if(input == 'S'){ qq
//      digitalWrite(Operate, LOW);
//      }
//     else if(input == '1'){
//      digitalWrite(A_z,HIGH);
//      digitalWrite(B_z,HIGH);
//      range = 1/19;
//      }
//     else if(input == '2'){
//      digitalWrite(A_z,LOW);
//      digitalWrite(B_z,HIGH);
//      range = 1/3.8;
//      }
//     else if(input == '3'){
//      digitalWrite(A_z,HIGH);
//      digitalWrite(B_z,LOW);
//      range = 1/1.9;
//      }
//     else if(input == '4'){
//      digitalWrite(A_z,LOW);
//      digitalWrite(B_z,LOW);
//      range = 1/0.9;
//      }
//      
//    }
//    }
