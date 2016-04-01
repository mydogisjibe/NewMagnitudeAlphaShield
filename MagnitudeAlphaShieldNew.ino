//Code by Brent Luker
#include <SoftwareSerial.h>

#include "Accelerometer.h"
#include "Magnetometer.h"
#include "GPS.h"
#include "Barometer.h"
#include "TempHumid.h"
#include "CO2.h"


//Time in millis between each reading of the sensors, and
//how long to wait after bootup.
#define READ_DELAY 1000
#define INITIAL_DELAY 1000
// How long to log data to serial
#define SAMPLE_TIME 30000
// LED pins on magnitude board
#define LED_A_PIN 10
#define LED_B_PIN 11

#define LOGGER_TX 6
#define LOGGER_RX 7

Accelerometer a;
Magnetometer m;
GPS g;
TempHumid th;
Barometer b;
CO2 c;

<<<<<<< HEAD
SoftwareSerial logger(LOGGER_TX, LOGGER_RX);

#define NUM_SENSORS 5
=======
#define NUM_SENSORS 6
>>>>>>> CO2

//This array is used when all the sensors are more or less doing the same thing.
Sensor* sensors[NUM_SENSORS] = {&a, &m,&g, &th,&b, &c};
void setup() {
  Serial.begin(9600);
  logger.begin(9600);
  
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_A_PIN, false);
  digitalWrite(LED_B_PIN, false);
  
  for(int8_t i=0; i<NUM_SENSORS; i++)
    sensors[i]->initSensor();

  Serial.print(F("Logging sample data for 30 seconds..."));
}

//Creates 1 sec delay at begining
boolean isSampling = true;
unsigned long lastLog = INITIAL_DELAY;
void loop() {
  if( lastLog + READ_DELAY < millis()){
  lastLog = millis();
<<<<<<< HEAD
    for(int8_t i=0; i<NUM_SENSORS; i++)
      sensors[i]->updateData();
    
    // Data logging
    if(isSampling){
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
      Serial.print( F(",Date: " ) );
      Serial.print( g.date);
      Serial.print( F(",Time: " ) );
      Serial.print( g.time);
      Serial.println();
    }
    else {
      logger.print( F("HIH-6130Temp: "));
      logger.print( th.temp );
      logger.print( F(",humidity: "));
      logger.print( th.humidity );
      logger.print( F(",bmp180Temp: "));
      logger.print( b.temperature );
      logger.print( F(",pressure: "));
      logger.print( b.pressure );
      logger.print( F(",Altitude"));
      logger.print( b.altitude);
      logger.print( F("accel_x: " ) );
      logger.print( a.x );
      logger.print( F(",accel_y: " ) );
      logger.print( a.y );
      logger.print( F(",accel_z: " ) );
      logger.print( a.z );
      logger.print( F(",mag_x: " ) );
      logger.print( m.x );
      logger.print( F(",mag_y: " ) );
      logger.print( m.y );
      logger.print( F(",mag_z: " ) );
      logger.print( m.z);
      logger.print( F(",gps_lat: " ) );
      logger.print( g.lat,6);
      logger.print( F(",gps_lon: " ) );
      logger.print( g.lon,6);
      logger.print( F(",gps_alt: " ) );
      logger.print( g.alt);
      logger.print( F(",gps_speed(mps): " ) );
      logger.print( g.speed_mps);
      logger.print( F(",Date: " ) );
      logger.print( g.date);
      logger.print( F(",Time: " ) );
      logger.print( g.time);
      logger.print(F("\r\n"));
    }
=======
  
  for(int8_t i=0; i<NUM_SENSORS; i++)
    sensors[i]->updateData();

  // Data logging
  if( lastLog < millis()){
    Serial.print( F("CO2: "));
    Serial.print(c.co2);
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
>>>>>>> CO2
    
    if(isSampling && millis() > SAMPLE_TIME){
      Serial.print(F("Sampling is done. Arduino will now log data to logger. You may unplug the serial cable."));
      Serial.end();
      isSampling = false;
    }
  }
}
