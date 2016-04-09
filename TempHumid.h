// Code by Brent Luker
#include <Arduino.h>
#include "Sensor.h"
class TempHumid: public Sensor
{
public:
  uint8_t status = 0;
  //Temperature is in degrees C. Humidity is a %
  float humidity, temp;
  void initSensor();
  void updateData();
};

