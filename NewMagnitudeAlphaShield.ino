//Code by Brent Luker
#include "Accelerometer.h"
#include "Magnetometer.h"
#include "GPS.h"

// LED pins on magnitude board
#define LED_A_PIN 10
#define LED_B_PIN 11

Accelerometer a;
Magnetometer m;
GPS g;

#define NUM_SENSORS 3

//This array is used when all the sensors are more or less doing the same thing.
Sensor* sensors[NUM_SENSORS] = {&a, &m,&g};
void setup() {
  Serial.begin(9600);
  
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_A_PIN, false);
  digitalWrite(LED_B_PIN, false);
  
  for(byte i=0; i<NUM_SENSORS; i++)
    sensors[i]->initSensor();
}

//Creates 1 sec delay at begining
int lastLog = 1000;
void loop() {
  delay(100);
  
  for(byte i=0; i<NUM_SENSORS; i++)
    sensors[i]->updateData();

  // Data logging
  if( lastLog < millis()){
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
    Serial.print( g.lat);
    Serial.print( F(",gps_lon: " ) );
    Serial.print( g.lon);
    Serial.print( F(",gps_alt: " ) );
    Serial.print( g.alt);
    Serial.print( F(",gps_speed(mps): " ) );
    Serial.print( g.speed_mps);
    Serial.println();
    
  }
}
