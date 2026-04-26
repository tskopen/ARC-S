#include <Arduino.h>
#include <SoftwareSerial.h>

const uint8_t TXPin = 43;
const uint8_t RXPin = 44; 

const unsigned long SERIAL_BAUD = 115200;
const unsigned long GPS_BAUD = 9600;

//Precise Orbit Determination

struct PODStruct
{
  double latitude, longitude;
  float alt;
  uint32_t timestampMillis; // when fix was read
  float hdop;
  uint8_t sats;
  bool valid;
};

class GPSClass
{
  private:
    PODStruct POD;
    
  public:
    void rawNMEAOutput();
    void init();
};