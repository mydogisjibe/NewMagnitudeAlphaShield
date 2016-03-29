//Code by Jiahao Kuang
#include "SFE_BMP180.h"
#include <Wire.h>
#include "Barometer.h"

void Barometer::initSensor(){
  barometer.begin();
  if(p0 == -1){
    double T,P;
    char status = barometer.startTemperature();
    if(status != 0){
      delay(status);
      status = barometer.getTemperature(T);
      if(status != 0){
        status = barometer.startPressure(2);
        if(status != 0){
          delay(status);
          status = barometer.getPressure(P,T);
          if(status != 0){
            p0 = P;
          }else Serial.println("failed to get pressure reading.");
        }else Serial.println("failed to start pressure reading.");
      }else Serial.println("failed to get temperture reading.");
    }else Serial.println("failed to start temperture reading.");
  }
}

void Barometer::updateData(){
  double T,P;
  char status = barometer.startTemperature();
  if(status != 0){
    delay(status);
    status = barometer.getTemperature(T);
    if(status != 0){
      status = barometer.startPressure(2);
      if(status != 0){
        delay(status);
        status = barometer.getPressure(P,T);
        if(status != 0){
          pressure = P;
          temperature = T;
          altitude = barometer.altitude(P,p0);
        }else Serial.println("failed to get pressure reading.");
      }else Serial.println("failed to start pressure reading.");
    }else Serial.println("failed to get temperture reading.");
  }else Serial.println("failed to start temperture reading.");
}
