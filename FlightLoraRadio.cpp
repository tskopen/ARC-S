#include <LoRa.h>

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
// https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
double frequency = 915E6;
int lora8CSN = 7;
int lora7MISO = 6;
int lora5MOSI = 5;
int lora6SCK = 4;
/*
//void setModulation(MOD_2FSK)

Status setFrequency(double frequency)

Status setFrequencyDeviation(double dev)
void setChannel(uint8_t ch)
Status setChannelSpacing(double sp)
Status setDataRate(double drate)

Status setRxBandwidth(double bw);
void setOutputPower(int8_t power)
// Status transmit(uint8_t *data, size_t length, uint8_t addr = 0)
Status receive(uint8_t *data, size_t length, size_t *read = nullptr, uint8_t addr = 0)
int8_t getRSSI()
uint8_t getLQI()



Status setPreambleLength(uint8_t length)
void setSyncWord(uint16_t sync)
void setPacketLengthMode(PacketLengthMode mode, uint8_t length = 255)
void setAddressFilteringMode(AddressFilteringMode mode)
void setCrc(bool enable)
void setDataWhitening(bool enable)
Status setManchester(bool enable)
Status setFEC(bool enable)
*/