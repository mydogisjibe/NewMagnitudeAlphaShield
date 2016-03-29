#include "ADXL345.h"
#include "Sensor.h"

#ifndef Accelerometer_h
#define Accelerometer_h

class CO2: public Sensor
{
public:
  float CO2;
	void initSensor();
	void updateData();
private:
	ADXL345 accelerometer;

};


#endif
