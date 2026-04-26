#include "GPS.h"

GPSClass GPS;

void setup() 
{
  GPS.init();
}

void loop() 
{
  GPS.rawNMEAOutput();
}
