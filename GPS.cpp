#include "GPS.h"
#include "SoftwareSerial.h"
 #include <Arduino.h>

#define GPS_RX_PIN 8
#define GPS_TX_PIN 9
#define AIRBORNE_MODE

#define _GPRMC_TERM   "GPRMC"
#define _GPGGA_TERM   "GPGGA"

SoftwareSerial gpsSerial( GPS_RX_PIN, GPS_TX_PIN );//GPS

const float GPS::GPS_INVALID_F_ANGLE = 1000.0;
const float GPS::GPS_INVALID_F_ALTITUDE = 1000000.0;
const float GPS::GPS_INVALID_F_SPEED = -1.0;

GPS::GPS()//GPS + needed variables
  :  _time(GPS_INVALID_TIME)
  ,  _date(GPS_INVALID_DATE)
  ,  _latitude(GPS_INVALID_ANGLE)
  ,  _longitude(GPS_INVALID_ANGLE)
  ,  _altitude(GPS_INVALID_ALTITUDE)
  ,  _speed(GPS_INVALID_SPEED)
  ,  _hdop(GPS_INVALID_HDOP)
  ,  _last_time_fix(GPS_INVALID_FIX_TIME)
  ,  _last_position_fix(GPS_INVALID_FIX_TIME)
  ,  _parity(0)
  ,  _is_checksum_term(false)
  ,  _sentence_type(_GPS_SENTENCE_OTHER)
  ,  _term_number(0)
  ,  _term_offset(0)
  ,  _gps_data_good(false)
{
  _term[0] = '\0';
} 

void GPS::initSensor(){//initialize sensor
   gpsSerial.begin( 9600 );
  #ifdef AIRBORNE_MODE
    initializeGPS();
  #endif
}

void GPS::updateData(){//get GPS data

  //encode GPS data
  unsigned long start = millis();
  do {
    while (gpsSerial.available())
      encode(gpsSerial.read());
  } while (millis() - start < 0);
  
  age = GPS_INVALID_AGE;//fix age
  
  f_get_position(&lat, &lon, &age);//position
  get_datetime(&date,&time,&age);//date+time
  alt = f_altitude();//altitude
  speed_mps = f_speed_mps();//speed

  //Invalid data
  if (age == GPS_INVALID_AGE) {//no/old GPS fix
    if(Serial)
      Serial.print(F("**No GPS Fix**"));
  } else if (age > 10000) {
    Serial.print(F("**GPS Fix is older than 10 sec. Possible stale data**"));
  }
  if (lat == GPS_INVALID_F_ANGLE) {
    if(Serial)
      Serial.print(F("**GPS: Invalid Lat**"));
    lat = 0.0;//default value
  }
  if (lon == GPS_INVALID_F_ANGLE) {
    if(Serial)
      Serial.print(F("**GPS: Invalid Lon**"));
    lon = 0.0;//default value
  }  
  if (alt == GPS::GPS_INVALID_F_ALTITUDE) {
    if(Serial)
      Serial.print(F("**GPS: Invalid Alt**"));
    alt = 0.0;//default value
  }  
  if (speed_mps == GPS::GPS_INVALID_F_SPEED) {
    if(Serial)
      Serial.print(F("**GPS: Invalid Speed**"));
    speed_mps = 0.0;//default value
  }  
}

void GPS::f_get_position(float *latitude, float *longitude, unsigned long *fix_age){//float values for lat and lon
  long new_lat, new_lon;
  get_position(&new_lat, &new_lon, fix_age);
  //set public lat and lon variables
  lat = new_lat == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : (new_lat / 1000000.0);
  lon = new_lon == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : (new_lon / 1000000.0);
}

void GPS::get_position(long *latitude, long *longitude, unsigned long *fix_age){//long values for lat and lon
  if (latitude) *latitude = _latitude;
  if (longitude) *longitude = _longitude;
  if (fix_age) age = _last_position_fix == GPS_INVALID_FIX_TIME ? GPS_INVALID_AGE : millis() - _last_position_fix;
}

float GPS::f_altitude(){//float value for altitude
  return _altitude == GPS_INVALID_ALTITUDE ? GPS_INVALID_F_ALTITUDE : _altitude / 100.0;
}

float GPS::f_speed_mps(){//speed in mps (float)
  float sk = f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : 0.51444444 * sk; 
}

float GPS::f_speed_knots() {//speed in knots (float)
  return _speed == GPS_INVALID_SPEED ? GPS_INVALID_F_SPEED : _speed / 100.0;
}

// date as ddmmyy, time as hhmmsscc, and age in milliseconds
void GPS::get_datetime(unsigned long *date, unsigned long *time, unsigned long *age){
  if (date) *date = _date;
  if (time) *time = _time;
  if (age) *age = _last_time_fix == GPS_INVALID_FIX_TIME ? 
   GPS_INVALID_AGE : millis() - _last_time_fix;
}

bool GPS::encode(char c){//encode one character from GPS
  bool valid_sentence = false;
  switch(c){
    case ',': _parity ^= c;// term terminators
    case '\r':
    case '\n':
    case '*':
      if (_term_offset < sizeof(_term)){
        _term[_term_offset] = 0;
        valid_sentence = term_complete();
      }
      ++_term_number;
      _term_offset = 0;
      _is_checksum_term = c == '*';
      return valid_sentence;
      
    case '$': // sentence beginning
      _term_number = _term_offset = 0;
      _parity = 0;
      _sentence_type = _GPS_SENTENCE_OTHER;
      _is_checksum_term = false;
      _gps_data_good = false;
      return valid_sentence;
  }

  // ordinary characters
  if (_term_offset < sizeof(_term) - 1)_term[_term_offset++] = c;
  if (!_is_checksum_term) _parity ^= c;

  return valid_sentence;
}

void GPS::initializeGPS() {// set GPS to airborne < 1G mode
  // Adapted from:
  // https://github.com/mjholmes/basic_tracker/blob/master/Code/gps.ino
  // This sets the GPS to Airborne < 1G mode. We don't expect to have a GPS fix
  // during launch, only when falling. So we want the more accurate airborne
  // mode.

  // Make sure the GPS is in airborne mode. See u-blox protocol spec
  // section 31.10.2 for details of the CFG-NAV5 command and section 24
  // (page 82) for UBX packet structure. u-center binary console can be
  // used to sniff this data rather than having to work it all out.
  uint8_t cfgNav5[] = {
    0xB5, 0x62, 0x06, 0x24, // Sync chars mu and b plus class and id values
    0x24, 0x00, // Length of data (in bytes, little endian) - 36dec
    0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, // Data
    0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
    0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00,
    0x64, 0x00, 0x2C, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x16, 0xDC // 16bit Checksum, 8bit Fletcher (RFC1146) calculated over class,
  };           // id, length and data (not sync chars).

  for (int i = 0; i < sizeof(cfgNav5)/sizeof(uint8_t); i++) {
    gpsSerial.write(cfgNav5[i]);
  }
}

bool GPS::term_complete(){//managing data
  if (_is_checksum_term){
    byte checksum = 16 * from_hex(_term[0]) + from_hex(_term[1]);
    if (checksum == _parity){
      if (_gps_data_good){//good data
        _last_time_fix = _new_time_fix;
        _last_position_fix = _new_position_fix;

        switch(_sentence_type){//setting variables
          case _GPS_SENTENCE_GPRMC:
            _time      = _new_time;
            _date      = _new_date;
            _latitude  = _new_latitude;
            _longitude = _new_longitude;
            _speed     = _new_speed;
            break;
          case _GPS_SENTENCE_GPGGA:
            _altitude  = _new_altitude;
            _time      = _new_time;
            _latitude  = _new_latitude;
            _longitude = _new_longitude;
            _numsats   = _new_numsats;
            _hdop      = _new_hdop;
            break;
        }
        return true;
      }
    }
    return false;
  }

  // the first term determines the sentence type
  if (_term_number == 0){
    if (!gpsstrcmp(_term, _GPRMC_TERM)) _sentence_type = _GPS_SENTENCE_GPRMC;
    else if (!gpsstrcmp(_term, _GPGGA_TERM)) _sentence_type = _GPS_SENTENCE_GPGGA;
    else _sentence_type = _GPS_SENTENCE_OTHER;
    return false;
  }

  if (_sentence_type != _GPS_SENTENCE_OTHER && _term[0]){
    
    switch(COMBINE(_sentence_type, _term_number)){
      case COMBINE(_GPS_SENTENCE_GPRMC, 1): // Time in both sentences
      case COMBINE(_GPS_SENTENCE_GPGGA, 1):
        _new_time = parse_decimal();
        _new_time_fix = millis();
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 2): // GPRMC validity
        _gps_data_good = _term[0] == 'A';
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 3): // Latitude
      case COMBINE(_GPS_SENTENCE_GPGGA, 2):
        _new_latitude = parse_degrees();
        _new_position_fix = millis();
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 4): // N/S
      case COMBINE(_GPS_SENTENCE_GPGGA, 3):
        if (_term[0] == 'S')
          _new_latitude = -_new_latitude;
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 5): // Longitude
      case COMBINE(_GPS_SENTENCE_GPGGA, 4):
        _new_longitude = parse_degrees();
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 6): // E/W
      case COMBINE(_GPS_SENTENCE_GPGGA, 5):
        if (_term[0] == 'W')
          _new_longitude = -_new_longitude;
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 7): // Speed (GPRMC)
        _new_speed = parse_decimal();
        break;
        
      case COMBINE(_GPS_SENTENCE_GPRMC, 9): // Date (GPRMC)
        _new_date = gpsatol(_term);
        break;
        
      case COMBINE(_GPS_SENTENCE_GPGGA, 6): // Fix data (GPGGA)
        _gps_data_good = _term[0] > '0';
        break;
        
      case COMBINE(_GPS_SENTENCE_GPGGA, 8): // HDOP
        _new_hdop = parse_decimal();
        break;
        
      case COMBINE(_GPS_SENTENCE_GPGGA, 9): // Altitude (GPGGA)
        _new_altitude = parse_decimal();
        break;     
    } 
  }
  return false;
}

int GPS::from_hex(char a) {//converting character from hex
  if (a >= 'A' && a <= 'F')return a - 'A' + 10;
  else if (a >= 'a' && a <= 'f')return a - 'a' + 10;
  else return a - '0';
}

int GPS::gpsstrcmp(const char *str1, const char *str2){
  while (*str1 && *str1 == *str2)++str1, ++str2;
  return *str1;
}

unsigned long GPS::parse_decimal(){
  char *p = _term;
  bool isneg = *p == '-';
  if (isneg) ++p;
  unsigned long ret = 100UL * gpsatol(p);
  while (gpsisdigit(*p)) ++p;
  if (*p == '.'){
    if (gpsisdigit(p[1])){
      ret += 10 * (p[1] - '0');
      if (gpsisdigit(p[2]))ret += p[2] - '0';
    }
  }
  return isneg ? -ret : ret;
}

// Parse a string in the form ddmm.mmmmmmm...
unsigned long GPS::parse_degrees(){
  char *p;
  unsigned long left_of_decimal = gpsatol(_term);
  unsigned long hundred1000ths_of_minute = (left_of_decimal % 100UL) * 100000UL;
  for (p=_term; gpsisdigit(*p); ++p);
  if (*p == '.'){
    unsigned long mult = 10000;
    while (gpsisdigit(*++p)){
      hundred1000ths_of_minute += mult * (*p - '0');
      mult /= 10;
    }
  }
  return (left_of_decimal / 100) * 1000000 + (hundred1000ths_of_minute + 3) / 6;
}

long GPS::gpsatol(const char *str){
  long ret = 0;
  while (gpsisdigit(*str))ret = 10 * ret + *str++ - '0';
  return ret;
}
