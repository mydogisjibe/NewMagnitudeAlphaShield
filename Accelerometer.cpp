//Code by Brent Luker
#include <ADXL345.h>
#include "Accelerometer.h"

// Check datasheet for other possible ranges
#define ADXL345_RANGE 2
#define ADXL345_G_PER_LSB 0.0039


void Accelerometer::initSensor(){
	accelerometer.powerOn();
	accelerometer.setRangeSetting(ADXL345_RANGE);
	accelerometer.setFullResBit(true); //this will ensure 3.9 mg / LSB
}

void Accelerometer::updateData(){
	int ax, ay, az;
	accelerometer.readAccel(&ax, &ay, &az);
	x = ((int16_t) ax) * ADXL345_G_PER_LSB;
	y = ((int16_t) ay) * ADXL345_G_PER_LSB;
	z = ((int16_t) az) * ADXL345_G_PER_LSB;
}
