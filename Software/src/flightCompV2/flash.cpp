#include <Arduino.h>
#include <SPI.h>
#include <SPIFFS.h>

#define FLASH_CS    36
#define FLASH_CLK   35
#define FLASH_MISO  37
#define FLASH_MOSI  34

struct LogEntry {
    uint32_t timestamp_ms;
    int16_t roll100;    // roll*100
    int16_t pitch100;   // pitch*100
    int16_t yaw100;     // yaw*100
    int16_t altitude100;// optional altitude*100
};

File logFile;

void setupFlash() 
{
    pinMode(FLASH_CS, OUTPUT);
    digitalWrite(FLASH_CS, HIGH);

    SPI.begin(FLASH_CLK, FLASH_MISO, FLASH_MOSI);

    Serial.println("SPI bus ready");

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed!");
        return;
    }

    Serial.println("SPIFFS ready");

}

void logData(float roll, float pitch, float yaw, float alt) 
{
    if (!logFile) return;

    LogEntry entry;
    entry.timestamp_ms = millis();
    entry.roll100      = (int16_t)(roll * 100);
    entry.pitch100     = (int16_t)(pitch * 100);
    entry.yaw100       = (int16_t)(yaw * 100);
    entry.altitude100  = (int16_t)(alt * 100);


    logFile.write((uint8_t*)&entry, sizeof(LogEntry));
}

// Call this before shutdown or after the flight to flush data
void closeFlash() 
{
    if (logFile) 
    {
        logFile.flush();
        logFile.close();
    }
}

void startLogging() 
{
    char filename[32];
    sprintf(filename, "/flight_%lu.bin", millis()); // unique per flight

    logFile = SPIFFS.open(filename, FILE_WRITE);
    if (!logFile) {
        Serial.println("Failed to open log file");
        return;
    }

    Serial.print("Logging started: "); Serial.println(filename);
}

void transmitLogs() 
{
    if (!SPIFFS.begin(true)) 
    {
        Serial.println("SPIFFS mount failed!");
        return;
    }

    File logFile = SPIFFS.open("/flight.bin", FILE_READ);
    if (!logFile) 
    {
        Serial.println("Failed to open flight.bin");
        return;
    }

    LogEntry entry;
    while (logFile.available() >= sizeof(LogEntry)) 
    {
        logFile.read((uint8_t*)&entry, sizeof(LogEntry));

        // Convert back to float
        float roll     = entry.roll100 / 100.0;
        float pitch    = entry.pitch100 / 100.0;
        float yaw      = entry.yaw100 / 100.0;
        float altitude = entry.altitude100 / 100.0;

        // Transmit CSV over Serial
        Serial.printf("%lu,%.2f,%.2f,%.2f,%.2f\n",
            entry.timestamp_ms, roll, pitch, yaw, altitude);
    }

    logFile.close();
    Serial.println("Transmission complete");
}