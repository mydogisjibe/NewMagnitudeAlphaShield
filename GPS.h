#define uint8_t byte
#include "Sensor.h"
#include "SoftwareSerial.h"
 #include <Arduino.h>

#ifndef GPS_h
#define GPS_h

#define COMBINE(sentence_type, term_number) (((unsigned)(sentence_type) << 5) | term_number)

class GPS: public Sensor
{
public: //used by MagnitudeAlphaShieldNew.ino
  GPS();
  float lat;//latitude
  float lon;//longitdue
  float alt;//altitude
  unsigned long date;
  unsigned long time;
  float speed_mps;
  unsigned long age;//fix age
  //functions
  void initSensor();
  void updateData();
  
private:
  //invalid valuds
  enum {
    GPS_INVALID_AGE = 0xFFFFFFFF,      GPS_INVALID_ANGLE = 999999999, 
    GPS_INVALID_ALTITUDE = 999999999,  GPS_INVALID_DATE = 0,
    GPS_INVALID_TIME = 0xFFFFFFFF,     GPS_INVALID_SPEED = 999999999, 
    GPS_INVALID_FIX_TIME = 0xFFFFFFFF,
    GPS_INVALID_HDOP = 0xFFFFFFFF
  };
  static const float GPS_INVALID_F_ANGLE, GPS_INVALID_F_ALTITUDE, GPS_INVALID_F_SPEED;
  
  void initializeGPS();//initialize airborne mode
  bool encode(char c); // process one character received from GPS
  
  //get values from GPS
  void get_datetime(unsigned long *date, unsigned long *time, unsigned long *age);
  void get_position(long *latitude, long *longitude,unsigned long *age);
  inline long altitude() { return _altitude; };
  inline unsigned long speed() { return _speed; };
  
  // horizontal dilution of precision in 100ths
  inline unsigned long hdop() { return _hdop; };

  //get float values
  void f_get_position(float *latitude, float *longitude, unsigned long *fix_age = 0);
  float f_altitude();
  float f_speed_knots();
  float f_speed_mps();
  
  enum {_GPS_SENTENCE_GPGGA, _GPS_SENTENCE_GPRMC, _GPS_SENTENCE_OTHER};

  // properties
  unsigned long _time, _new_time;
  unsigned long _date, _new_date;
  long _latitude, _new_latitude;
  long _longitude, _new_longitude;
  long _altitude, _new_altitude;
  unsigned long  _speed, _new_speed;
  unsigned long  _hdop, _new_hdop;
  unsigned short _numsats, _new_numsats;

  unsigned long _last_time_fix, _new_time_fix;
  unsigned long _last_position_fix, _new_position_fix;
  bool gpsisdigit(char c) { return c >= '0' && c <= '9'; }

  // parsing state variables
  byte _parity;
  bool _is_checksum_term;
  char _term[15];
  byte _sentence_type;
  byte _term_number;
  byte _term_offset;
  bool _gps_data_good;

  bool term_complete();
  int from_hex(char a);

  unsigned long _encoded_characters;
  unsigned short _good_sentences;
  unsigned short _failed_checksum;
  unsigned short _passed_checksum;
  long gpsatol(const char *str);
  int gpsstrcmp(const char *str1, const char *str2);
  unsigned long parse_decimal();
  unsigned long parse_degrees();
  
};
#endif
