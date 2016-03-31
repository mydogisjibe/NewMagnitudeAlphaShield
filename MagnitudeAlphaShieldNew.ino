//Code by Brent Luker
#include "Accelerometer.h"
#include "Magnetometer.h"
#include "GPS.h"
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
GPS g;
TempHumid th;
Barometer b;

#define NUM_SENSORS 5

//This array is used when all the sensors are more or less doing the same thing.
Sensor* sensors[NUM_SENSORS] = {&a, &m,&g, &th,&b};
void setup() {
  Serial.begin(9600);
  
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_A_PIN, false);
  digitalWrite(LED_B_PIN, false);
  
  for(int8_t i=0; i<NUM_SENSORS; i++)
    sensors[i]->initSensor();
}

//Creates 1 sec delay at begining
unsigned long lastLog = INITIAL_DELAY;
void loop() {
  if( lastLog + READ_DELAY < millis()){
  
    for(int8_t i=0; i<NUM_SENSORS; i++)
      sensors[i]->updateData();
    
    // Data logging
    Serial.print( F("HIH-6130Temp: "));
    Serial.print( th.temp );
    Serial.print( F(",humidity: "));
    Serial.print( th.humidity );
    Serial.print( F(",bmp180Temp: "));
    Serial.print( b.temperature );
    Serial.print( F(",pressure: "));
    Serial.print( b.pressure );
    Serial.print( F(",Altitude"));
    Serial.print( b.altitude);
    Serial.print( F("accel_x: " ) );
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
    Serial.print( F(",gps_lat: " ) );
    Serial.print( g.lat,6);
    Serial.print( F(",gps_lon: " ) );
    Serial.print( g.lon,6);
    Serial.print( F(",gps_alt: " ) );
    Serial.print( g.alt);
    Serial.print( F(",gps_speed(mps): " ) );
    Serial.print( g.speed_mps);
    Serial.println();
    
  }
}
