#include "Arduino.h"
#include "CO2.h"
#include "SoftwareSerial.h"

#define CO2_RX 12
#define CO2_TX 13

// Commands
byte cmd_read_CO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //type [0]
byte cmd_read_Temp[] = {0xFE, 0X44, 0X00, 0X12, 0X02, 0X94, 0X45};  //type [1]
byte cmd_read_RH[] = {0xFE, 0x44, 0x00, 0x14, 0x02, 0x97, 0xE5 }; //type [2]
byte cmd_init[] = {0xFE, 0X41, 0X00, 0X60, 0X01, 0X35, 0XE8, 0x53}; //type [3]

SoftwareSerial sensor(CO2_RX, CO2_TX);
void CO2::initSensor(){
  sensor.begin(9600);
}

void CO2::updateData(){
  double co2 = sendRequest(0,7,3);
}


int CO2::sendRequest(int reqType, int respSize, int respInd)
{
  long Val=-255;
  int cmdTimeout =0;
  while(sensor.available() == 0)  //send read command in a loop until we see a response
  { 
    switch(reqType)
    {
      case 0:
        sensor.write(cmd_read_CO2,7);
        break;
      case 1:
        sensor.write(cmd_read_Temp,7);
        break;
      case 2:
        sensor.write(cmd_read_RH,7);
        break;
      case 3:
        sensor.write(cmd_init,8);
        break;
      default:
        return -256;
        break;
    }
    cmdTimeout++;
    if(cmdTimeout > 20)
      return -203;
    delay(130);          //give some time after each request for a response
 }
  
  int timeOut=0;  //initialize a timeout counter
  
  while(sensor.available() < 7) //loop through until we have are 7-byte response
  { 
    if(timeOut > 40);     //after 40 loops, break out and try again
      break;
    timeOut++;
    delay(5);
  }
  
  if(sensor.available() == 7)   //if we have our 7-byte response get value
       Val = getResp(respSize,respInd);
  else              //if we dont i.e. our request timed out
  {
    Val = -300;
    while(sensor.available() > 0) //loop through and flush any bytes we did receiver so they dont throw the next packet off track
    {
        Val++;
        sensor.read();
    }
  }
  return Val;
}

long CO2::getResp(int size, int strt)
{
  byte packet[size];
    for(int i=0; i<size; i++)
    {
    packet[i] = sensor.read();                   //create array from packet
    }
  
    int high = packet[strt];                        //high byte for value is 4th byte in packet in the packet
    int low = packet[strt+1];                         //low byte for value is 5th byte in the packet

  
    unsigned long val = high*256 + low;                //combine high byte and low byte
    return val;
}
