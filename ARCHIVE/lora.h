void setupSDCard() 
{
  SPI.begin(SDCardCLK, SDCardMISO, SDCardMOSI, SDCardCS);
  //Pins are IO#

  int lora5MOSI = 18;
  int lora7MISO = 35;
  int lora6SCK = 12;
  int lora8CSN = 16;


  pinMode(lora5MOSI, OUTPUT);


  pinMode(lora7MISO, INPUT);


  pinMode(lora6SCK, OUTPUT);


  pinMode(lora8CSN, OUTPUT);

}
