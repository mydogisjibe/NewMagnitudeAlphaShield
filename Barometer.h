#include <SFE_BMP180.h>
#include "Sensor.h"

#ifndef Barometer_h
#define Barometer_h

class Barometer: public Sensor
{
public:
  double pressure, altitude, temperature;

//  set p0 to local air pressure to get a better reading,
//  leave as -1 to let sensor take the inital pressure 
//  reading as the base height.
  double p0 = -1;
  void initSensor();
  void updateData();
private:
  SFE_BMP180 barometer;
  
};


#endif

