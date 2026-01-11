#define FLASH_CS   10
#define FLASH_CLK  11
#define FLASH_MISO 13
#define FLASH_MOSI 14

SPIClass spiFlash(VSPI); // SPI3

void setupFlash() {
  pinMode(FLASH_CS, OUTPUT);
  digitalWrite(FLASH_CS, HIGH);

  spiFlash.begin(FLASH_CLK, FLASH_MISO, FLASH_MOSI, FLASH_CS);

  Serial.println("External flash SPI bus ready");
}

