//Code by Brent Luker
#include <Wire.h>
#include "Magnetometer.h"

#define HMC5883L_ADDRESS 0x1E
#define HMC5883L_CONFIG_REG_B 0x01
#define HMC5883L_MODE_REG 0x02
#define HMC5883L_FIRST_DATA_REG 0x03
#define HMC5883L_CONT_MEASUREMENT_MODE 0x00
//Check datasheet for other possible values
#define HMC5883L_SCALE 0x01 // range: +-1.3 Gauss
#define HMC5883L_GAUSS_PER_LSB 0.0092 // range: +-1.3 Gauss


void Magnetometer::initSensor(){
  // From Sparkfun Example Code:
  // http://sfecdn.s3.amazonaws.com/datasheets/Sensors/Magneto/HMC5883.pde
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(HMC5883L_ADDRESS); // Open communication with HMC5883
  Wire.write(HMC5883L_MODE_REG); // Select mode register
  Wire.write(HMC5883L_CONT_MEASUREMENT_MODE); // Continuous measurement mode
  Wire.endTransmission();
  // Set scale - From data sheet
  Wire.beginTransmission(HMC5883L_ADDRESS); // Open communication with HMC5883
  uint8_t scaleValue = HMC5883L_SCALE;
  // This goes in CRB7, CRB6, and CRB5
  scaleValue = scaleValue << 5;
  Wire.write(HMC5883L_CONFIG_REG_B); // Select config register B
  Wire.write(scaleValue); // Set scale value
  Wire.endTransmission();
}

void Magnetometer::updateData(){

  
  // From Sparkfun Example Code:
  // http://sfecdn.s3.amazonaws.com/datasheets/Sensors/Magneto/HMC5883.pde

  // Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(HMC5883L_FIRST_DATA_REG); // Select register 3, X MSB register
  Wire.endTransmission();

  // Read data from each axis, 2 registers per axis
  int16_t xRaw, yRaw, zRaw;
  Wire.requestFrom(HMC5883L_ADDRESS, 6);
  xRaw = Wire.read()<<8; // X msb
  xRaw |= Wire.read();   // X lsb
  zRaw = Wire.read()<<8; // Z msb
  zRaw |= Wire.read();   // Z lsb
  yRaw = Wire.read()<<8; // Y msb
  yRaw |= Wire.read();   // Y lsb

  x = xRaw * HMC5883L_GAUSS_PER_LSB;
  y = yRaw * HMC5883L_GAUSS_PER_LSB;
  z = zRaw * HMC5883L_GAUSS_PER_LSB;
}

