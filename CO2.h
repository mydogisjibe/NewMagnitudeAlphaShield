#include "Sensor.h"

#ifndef CO2_h
#define CO2_h

class CO2: public Sensor
{
public:
  float co2;
	void initSensor();
	void updateData();
private:
  int sendRequest(int reqType, int respSize, int respInd);
  long getResp(int size, int strt);

};


#endif


