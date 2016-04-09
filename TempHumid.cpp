// Code by Brent Luker
#include "TempHumid.h"
#include "Wire.h"
#include <Arduino.h>
#define ADDRESS 0x27

void TempHumid::initSensor(){
  Wire.begin();
}

void TempHumid::updateData(){
  
  byte humHigh, humLow, tempHigh, tempLow;
  unsigned int humData, tempData;

  // Request read
  Wire.beginTransmission(ADDRESS);
  Wire.endTransmission();

  // If we didn't get 4 bytes from the sensor....
  if(Wire.requestFrom((byte) ADDRESS, (byte) 4) != 4){
    status = 4; // temp and humidity will be unchanged. Use our own error status.
    return;
  }

  humHigh  = Wire.read();
  humLow   = Wire.read();
  tempHigh = Wire.read();
  tempLow  = Wire.read();

  // Status can be
  // 0: Normal
  // 1: Stale - this data has already been read
  // 2: Command Mode - the sensor is in command mode
  // 3: Diagnostic - The sensor has had a diagnostic condition and data is
  //    invalid
  
  // Status is the top two bits of the high humidity byte
  status = (humHigh >> 6) & 0x03;
  if(status == 3) return; // temp and humidity will be unchanged

  // Keep the rest
  humHigh = humHigh & 0x3f;

  // OR in the low bytes
  humData  = humHigh  << 8 | humLow;
  tempData = tempHigh << 8 | tempLow;

  // The bottom two bits of the low temp byte are invalid, so we'll remove
  // those
  tempData = tempData >> 2;

  // Convert to floating point and save
  humidity = (float) humData * 6.10e-3; // 100 / (2^14 - 1)
  temp = (float) tempData * 1.007e-2 - 40.0; // 165 / (2^14 - 1)
}

