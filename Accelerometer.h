// Code by Brent Luker
#include <ADXL345.h>
#include "Sensor.h"

#ifndef Accelerometer_h
#define Accelerometer_h

class Accelerometer: public Sensor
{
public:
  float x, y, z;
	void initSensor();
	void updateData();
private:
	ADXL345 accelerometer;

};


#endif
