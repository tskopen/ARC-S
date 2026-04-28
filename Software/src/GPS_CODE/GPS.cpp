#include "GPS.h"

SoftwareSerial gpsSerial(RXPin, TXPin); // RX, TX for SoftwareSerial

String lineBuffer;

void GPSClass::init()
{
  Serial.begin(SERIAL_BAUD);
  // short wait for Serial (helps on some boards)
  unsigned long start = millis();
  while (!Serial && millis() - start < 2000) {}

  gpsSerial.begin(GPS_BAUD);
  Serial.println("[GPS] started");
}

void GPSClass::rawNMEAOutput()
{
  while (gpsSerial.available() > 0)
    {
      int b = gpsSerial.read();
      if (b < 0) break;

      // Build NMEA line until newline
      if (b == '\n') 
      {
        // trim possible '\r'
        if (lineBuffer.length() && lineBuffer.charAt(lineBuffer.length() - 1) == '\r') 
        {
          lineBuffer.remove(lineBuffer.length() - 1);
        }

        // Print timestamp + NMEA sentence
        Serial.print("[");
        Serial.print(millis());
        Serial.print("] ");
        Serial.println(lineBuffer);
        lineBuffer = "";
      } 
      else 
      {
        lineBuffer += (char)b;
        // prevent runaway buffer
        if (lineBuffer.length() > 120) 
        {
          // print partial line with notice and reset
          Serial.print("[");
          Serial.print(millis());
          Serial.print("] ");
          Serial.println(lineBuffer);
          lineBuffer = "";
        }
      }
    }
}