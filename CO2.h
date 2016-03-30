#include "Sensor.h"

#ifndef CO2_h
#define CO2_h

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
