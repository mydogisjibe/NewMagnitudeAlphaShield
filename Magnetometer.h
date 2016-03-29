#include "Sensor.h"

class Magnetometer: public Sensor
{
public:
  float x, y, z;
  void initSensor();
  void updateData();

};
