//Code by Brent Luker
#ifndef Sensor_h
#define Sensor_h

class Sensor
{
public:
  virtual void initSensor() = 0;
  virtual void updateData() = 0;
};

#endif

