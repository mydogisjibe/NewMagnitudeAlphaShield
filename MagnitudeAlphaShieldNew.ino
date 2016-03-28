//Code by Brent Luker
#include "Accelerometer.h"
#include "Magnetometer.h"
#include "Barometer.h"
#include "TempHumid.h"

//Time in millis between each reading of the sensors, and
//how long to wait after bootup.
#define READ_DELAY 100
#define INITIAL_DELAY 1000

// LED pins on magnitude board
#define LED_A_PIN 10
#define LED_B_PIN 11

Accelerometer a;
Magnetometer m;
TempHumid th;
Barometer b;

// Change this every time a sensor is added or removed from the code.
#define NUM_SENSORS 4

//This array is used when all the sensors are more or less doing the same thing.
Sensor* sensors[NUM_SENSORS] = {&a, &b, &m, &th};
void setup() {
  Serial.begin(9600);
  
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_A_PIN, false);
  digitalWrite(LED_B_PIN, false);
  
  for(int8_t i=0; i<NUM_SENSORS; i++)
    sensors[i]->initSensor();
}

unsigned long lastLog = INITIAL_DELAY;
void loop() {
  
  // Wait until enough time has passed before doing next read
  while(lastLog + READ_DELAY > millis());
  lastLog = millis();
  
  for(int8_t i=0; i<NUM_SENSORS; i++)
    sensors[i]->updateData();

  // Data logging
  if( lastLog < millis()){
    Serial.print( F("th02Temp: "));
    Serial.print( th.temp );
    Serial.print( F(",humidity: "));
    Serial.print( th.humidity );
	  Serial.print( F(",bmp180Temp: "));
    Serial.print( b.temperature );
	  Serial.print( F(",pressure: "));
    Serial.print( b.pressure );
    Serial.print( F(",Altitude"));
    Serial.print( b.altitude);
    Serial.print( F(",accel_x: " ) );
    Serial.print( a.x );
    Serial.print( F(",accel_y: " ) );
    Serial.print( a.y );
    Serial.print( F(",accel_z: " ) );
    Serial.print( a.z );
    Serial.print( F(",mag_x: " ) );
    Serial.print( m.x );
    Serial.print( F(",mag_y: " ) );
    Serial.print( m.y );
    Serial.print( F(",mag_z: " ) );
    Serial.print( m.z);
    Serial.println();
  }
}
