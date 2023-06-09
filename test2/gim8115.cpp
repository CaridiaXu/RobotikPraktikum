#include "gim8115.h"
#include <SPI.h>  //Serial Peripherial Interface (SPI) Library


int gim8115::setMotormode(MCP_CAN& CAN){
  unsigned char len = 0;
  unsigned char buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFC;

  while(CAN_OK != CAN.sendMsgBuf(id, 0, 8, buf))
{
 //Serial.println("Entering Motor Mode Failed!");
 //Serial.println("Initializing Motor again");
 delay(100);
}
//Serial.println("Motor Mode Enabled!");
  return 1;
}

int gim8115::exitMotormode(MCP_CAN& CAN){
  unsigned char len = 0;
  unsigned char buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFD;
    
    while(CAN_OK != CAN.sendMsgBuf(id, 0, 8, buf))
{
 //Serial.println("Exiting Motor Mode Failed!");
 //Serial.println("Exiting Motor again");
 delay(100);
}
//Serial.println("Motor Mode Disabled");
  return 1;
}

int gim8115::setZero(MCP_CAN& CAN){
  unsigned char len = 0;
  unsigned char buf[8];
  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFF;
  buf[3] = 0xFF;
  buf[4] = 0xFF;
  buf[5] = 0xFF;
  buf[6] = 0xFF;
  buf[7] = 0xFE;
    
    while(CAN_OK != CAN.sendMsgBuf(id, 0, 8, buf))
{
 //Serial.println("Zero Setting Failed!");
 //Serial.println("Zero Setting again");
 delay(100);
}
//Serial.println("Zero Setted");
  return 1;
}


float gim8115::normalSet(MCP_CAN& CAN, float tarPos, float tarVel, float tarTor){
//Position//
  unsigned int pos_b;
  if (tarPos>pos_max)
  {
    tarPos = pos_max;
  }
  else if (tarPos<pos_min)
  {
    tarPos = pos_min;
  }
  tarPos = tarPos + 95.5;
  pos_b = 65535 * tarPos / 191;
  float pos_16h = pos_b / 256;
  float pos_16l = pos_b % 256;
  unsigned char pos_16h_hex = pos_16h;
  unsigned char pos_16l_hex = pos_16l;

  //----------------------------------------------------------------------------//
  //Velocity//
  unsigned int vel_b;
  if (tarVel>vel_max)
  {
    tarVel = vel_max;
  }
  else if (tarVel<vel_min)
  {
    tarVel = vel_min;
  }
  tarVel = tarVel + 45;
  vel_b = 4095 * tarVel / 90;
  float vel_16h = vel_b / 16;
  float vel_16l = vel_b % 16;
  unsigned char vel_16h_hex = vel_16h;
  unsigned char vel_16l_hex = vel_16l;
  //----------------------------------------------------------------------------//
  //Torque//
  unsigned int tor_b;
  if (tarTor>tor_max)
  {
    tarTor = tor_max;
  }
  else if (tarTor<tor_min)
  {
    tarTor = tor_min;
  }
  tarTor = tarTor + 18;
  tor_b = 4095 * tarTor / 36;
  float tor_16h = tor_b / 256;
  float tor_16l = tor_b % 256;
  unsigned char tor_16h_hex = tor_16h;
  unsigned char tor_16l_hex = tor_16l;
  //----------------------------------------------------------------------------//
  //Sending data//
  unsigned char len = 0;
  unsigned char buf[8];
  buf[0] = pos_16h;
  buf[1] = pos_16l;
  buf[2] = vel_16h;
  buf[3] = vel_16l*16 + kp_16h_hex;
  buf[4] = kp_16l_hex;
  buf[5] = kd_16h_hex;
  buf[6] = kd_16l_hex*16 + tor_16h_hex;
  buf[7] = tor_16l_hex;

  byte sndStat =  CAN.sendMsgBuf(id, 0, 8, buf);

  if(sndStat == CAN_OK){
	//Serial.println("Message Sent Successfully!");
  } else{
	//Serial.println("Error Sending Message!");
	}

  //----------------------------------------------------------------------------//
  //Receiving data//
  long unsigned int rxId;
  char msgString2[128];  // Array to store serial string
  byte buf_received[6];
  char print_pos[15];   //Position string
  char print_vel[15];   //Velocity string
  char print_cur[15];   //Current string


    CAN.readMsgBuf(&rxId ,&len, buf_received);  //CAN BUS reading

    //Collecting bits from CAN_BUS signal
   unsigned int pos_motor = ((buf_received[1]<<8)|buf_received[2]); //Position reading
   unsigned int vel_motor = (buf_received[3]<<4)|((buf_received[4])>>4);    //Velocity reading
   unsigned int cur_motor = ((buf_received[4]&0xF)<<8)|buf_received[5]; //Current reading

   //Converting to readable data//

   float pos_f = (float) pos_motor;
   pos_f = (pos_f*191/65535)-95.5;
   float vel_f = (float) vel_motor;
   vel_f = (vel_f*90/4095)-45;
   float cur_f = (float) cur_motor;
   cur_f = (cur_f*36/4095)-18;      

    //Tranform to string to be printable with sprintf
   dtostrf(pos_f, 6, 3, print_pos);
   dtostrf(vel_f, 6, 3, print_vel);
   dtostrf(cur_f, 6, 3, print_cur);

  //sprintf(msgString2,"Motor Id: %d  Position: %s  Velocity: %s  Current: %s", buf_received[0],  print_pos, print_vel, print_cur);
  //Serial.println(msgString2);
  //Serial.println("desired_Position: ");
  Serial.write((unsigned int) pos_f);
  
  return 1;
}
